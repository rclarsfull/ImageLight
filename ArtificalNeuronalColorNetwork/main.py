import os
import sys

os.environ["TF_ENABLE_ONEDNN_OPTS"] = "0"
from keras.models import load_model
import joblib
import socket
import struct
import numpy as np
import pandas as pd
import threading
import time
from keras.optimizers import Adam,Nadam
from sklearn.preprocessing import MinMaxScaler
from keras.callbacks import LearningRateScheduler
import pandas as pd
import math
import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Dropout, BatchNormalization
from sklearn.model_selection import train_test_split
from keras.models import save_model, load_model
from keras.callbacks import EarlyStopping
import matplotlib.pyplot as plt
from sklearn.metrics import mean_absolute_error, r2_score


def lr_schedule(epoch):
    initial_lr = 0.01
    return 0.001
    if epoch < 10:
        return initial_lr
    elif epoch < 500:
        return initial_lr * 0.1
    else:
        return initial_lr * 0.01

def train():
    # Daten laden
    data = pd.read_excel("data.xlsx")

    # Eingabe und Ausgabe trennen
    X = data[['rot', 'gruen', 'blau']]
    y = data['helligkeit_candela']
    x_scaler = MinMaxScaler()
    x_scaler.feature_names_in_ = ['rot', 'gruen', 'blau']
    X_normalized = x_scaler.fit_transform(X)
    X_train, X_test, y_train, y_test = train_test_split(X_normalized, y, test_size=0.2, random_state=42)
    joblib.dump(x_scaler, 'scaler_model.joblib')

    # Modell erstellen und trainieren
    model = Sequential()
    model.add(Dense(128, input_dim=3, activation='relu'))  # 3 Eingangsneuronen für RGB-Werte
    model.add(Dropout(0.1))
    model.add(Dense(32, activation='relu'))
    model.add(Dense(1, activation='relu'))  # 1 Ausgangsneuron für die Helligkeit
    model.compile(optimizer=Nadam(learning_rate=lr_schedule(0)), loss='mean_squared_error')
    early_stopping = EarlyStopping(monitor='val_loss', patience=25, restore_best_weights=True)
    history = model.fit(X_train, y_train, epochs=10000, batch_size=32, validation_split=0.2, callbacks=[early_stopping, LearningRateScheduler(lr_schedule)])
    model.save('model.keras')

    # Plotten der Lernkurven
    plt.plot(history.history['loss'], label='Training Loss')
    plt.plot(history.history['val_loss'], label='Validation Loss')
    plt.title('Training and Validation Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.show()



    # Evaluierung des Modells
    loss = model.evaluate(X_test, y_test)
    predictions = model.predict(X_test)
    mae = mean_absolute_error(y_test, predictions)
    r2 = r2_score(y_test, predictions)

    print(f'Mean Absolute Error auf Testdaten: {mae}')
    print(f'Loss on test data: {loss}')
    print(f'Standardabweichung: {math.sqrt(loss)}')
    print(f'R2_Score: {r2}')
    plt.plot([0, max(y_test)], [0, max(y_test)], color='red', linestyle='--')

    colors = np.array(x_scaler.inverse_transform(X_test))
    avg_rgb = np.mean(colors, axis=1)
    sizes = avg_rgb * 5
    plt.scatter(y_test, predictions, c=colors / 255.0, s=sizes)
    plt.xlabel('Wahre Werte')
    plt.ylabel('Vorhersagen')
    plt.title('Vergleich Wahre Werte und Vorhersagen')
    plt.show()

    grautoene = []
    rottoene = []
    gruentoene = []
    blautoene = []
    for x in range(255):
        grautoene.append((x, x, x))
        rottoene.append((x, 0, 0))
        gruentoene.append((0, x, 0))
        blautoene.append((0, 0, x))

    feature_names = ['rot', 'gruen', 'blau']
    grautoene = pd.DataFrame(grautoene, columns=feature_names)
    rottoene = pd.DataFrame(rottoene, columns=feature_names)
    gruentoene = pd.DataFrame(gruentoene, columns=feature_names)
    blautoene = pd.DataFrame(blautoene, columns=feature_names)

    greyPredictions = model.predict(x_scaler.transform(grautoene))
    rotPredictions = model.predict(x_scaler.transform(rottoene))
    gruenPredictions = model.predict(x_scaler.transform(gruentoene))
    blauPredictions = model.predict(x_scaler.transform(blautoene))

    plt.plot(greyPredictions, color="grey", label='grey')
    plt.plot(rotPredictions, color="red", label='red')
    plt.plot(gruenPredictions, color="green", label='green')
    plt.plot(blauPredictions, color="blue", label='blue')
    plt.xlabel('Shades')
    plt.ylabel('Candela')
    plt.legend()
    plt.show()


def handle_client(connection, client_address, model, x_scaler, X_RESELUTION, Y_RESELUTION):
    try:
        print(f"Connection from {client_address}")

        start_time = time.time()  # Record the start time

        # Receive the binary data from the socket
        data_bytes = b""
        while len(data_bytes) < X_RESELUTION * Y_RESELUTION * 3 * 4:
            chunk = connection.recv(X_RESELUTION * Y_RESELUTION * 3 * 4 - len(data_bytes))
            if not chunk:
                print(f"Error: No Data received from {client_address}")
                return
            data_bytes += chunk
        print(f"Received {len(data_bytes)} bytes of data from {client_address}")
        print(f"Expected size: {X_RESELUTION * Y_RESELUTION * 3 * 4} bytes")
        # Unpack the binary data into a float array using struct
        float_array = struct.unpack(f'{X_RESELUTION * Y_RESELUTION * 3}f', data_bytes)

        # Convert the float array to a NumPy array if needed
        numpy_array = np.array(float_array, dtype=np.float32)
        rgbValues = []
        counter = 0
        while counter < numpy_array.size:
            rgbValues.append((numpy_array[counter], numpy_array[counter + 1], numpy_array[counter + 2]))
            counter += 3
        if len(rgbValues) != X_RESELUTION * Y_RESELUTION:
            print(f"Error: Data amount doesn't match Resolution for {client_address}")
            return

        
        batch_rgb_values = np.array(rgbValues)
        feature_names = ['rot', 'gruen', 'blau']
        rgbValues = pd.DataFrame(batch_rgb_values, columns=feature_names)

        predictions = model.predict(x_scaler.transform(batch_rgb_values))

        rounded_predictions = np.round(predictions)

        # Convert the rounded predictions to unsigned short values
        scaled_predictions = rounded_predictions.astype(np.uint16).flatten()

        for i in range(scaled_predictions.size):
            scaled_predictions[i] = (i % 1500) 
        print(scaled_predictions)

        # Convert scaled_predictions to bytes
        scaled_bytes = struct.pack(f'{X_RESELUTION * Y_RESELUTION}H', *scaled_predictions)

        # Send the data over the connection
        connection.sendall(scaled_bytes)
        print(f"Send {len(scaled_bytes)} bytes to {client_address}")

    except Exception as e:
        print(f"Error handling connection from {client_address}: {str(e)}")

    finally:
        # Calculate the duration and print it
        duration = time.time() - start_time
        print(f"Connection from {client_address} closed. Duration: {duration:.2f} seconds")

        # Close the connection
        connection.close()

def main():
    server_ip = "127.0.0.1"
    server_port = 12345
    X_RESELUTION = 2352
    Y_RESELUTION = 1568
    print(f"Server expects an image size of {X_RESELUTION}x{Y_RESELUTION}")

    model = load_model('model.keras')
    x_scaler = joblib.load('scaler_model.joblib')
    print("Models loaded")

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((server_ip, server_port))
    server_socket.listen(5)  # Allow up to 5 connections in the queue

    print(f"Server listening on {server_ip}:{server_port}")

    while True:
        connection, client_address = server_socket.accept()

        # Create a new thread to handle the connection
        client_handler = threading.Thread(target=handle_client, args=(connection, client_address, model, x_scaler, X_RESELUTION, Y_RESELUTION))
        client_handler.start()

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--train":
        train()
    else:
        main()