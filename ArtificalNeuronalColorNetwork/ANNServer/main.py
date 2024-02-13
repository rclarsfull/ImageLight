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
from keras.layers import Dense, Dropout, BatchNormalization, LeakyReLU
from sklearn.model_selection import train_test_split
from keras.models import save_model, load_model
from keras.callbacks import EarlyStopping
import matplotlib.pyplot as plt
from sklearn.metrics import mean_absolute_error, r2_score, mean_squared_error
import pkg_resources
import signal

client_handlers = []
def signal_handler(sig, frame):
    print("Exiting...")
    # Wait for all active threads to finish
    for thread in client_handlers:
        thread.join()
    print("All threads terminated.")
    sys.exit(0)

def lr_schedule(epoch):
    initial_lr = 0.01
    #return 0.001
    if epoch < 100:
        return initial_lr
    elif epoch < 200:
        return initial_lr * 0.1
    elif epoch < 800:
        return initial_lr * 0.01
    else:
        return initial_lr * 0.005

def train():
    # Daten laden
    data = pd.read_excel(pkg_resources.resource_filename('ANNServer', 'data.xlsx'))


    # Eingabe und Ausgabe trennen
    X = data[['rot', 'gruen', 'blau']]
    y = data['helligkeit_candela']
    x_scaler = MinMaxScaler()
    x_scaler.feature_names_in_ = ['rot', 'gruen', 'blau']
    X_normalized = x_scaler.fit_transform(X)
    X_train, X_test, y_train, y_test = train_test_split(X_normalized, y, test_size=0.28, random_state=42)
    joblib.dump(x_scaler, pkg_resources.resource_filename('ANNServer', 'scaler_model.joblib'))


    # Modell erstellen und trainieren
    model = Sequential()
    model.add(Dense(128, input_dim=3, activation='relu'))  # 3 Eingangsneuronen für RGB-Werte
    model.add(Dense(16, activation='tanh'))
    model.add(Dropout(0.05))
    model.add(Dense(16, activation=LeakyReLU(alpha=0.02)))
    model.add(Dropout(0.05))
    model.add(Dense(1, activation='relu'))  # 1 Ausgangsneuron für die Helligkeit
    model.compile(optimizer=Adam(learning_rate=lr_schedule(0)), loss='mean_squared_error')
    early_stopping = EarlyStopping(monitor='val_loss', patience=200 , restore_best_weights=True)
    history = model.fit(X_train, y_train, epochs=70000, batch_size=64, validation_split=0.2, callbacks=[early_stopping, LearningRateScheduler(lr_schedule)])
    model.save(pkg_resources.resource_filename('ANNServer', 'model.keras'))

    # Plotten der Lernkurven
    plt.plot(history.history['loss'], label='Training Loss')
    plt.plot(history.history['val_loss'], label='Validation Loss')
    plt.title('Training and Validation Loss')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.show()



    # Evaluierung des Modells
    predictions = model.predict(X_test)
    mae = mean_absolute_error(y_test, predictions)
    mse = mean_squared_error(y_test, predictions)
    r2 = r2_score(y_test, predictions)

    percentage_errors = np.abs((y_test - predictions.flatten()) / y_test) * 100
    for x in range(len(y_test)):
        if x in y_test.index:
            print(f'actual: {y_test[x]} target: {predictions.flatten()[x]} Error in %: {percentage_errors[x]} Color: {pow(np.array(x_scaler.inverse_transform(X_test)[x]), 1 / 2.2)}')
    print(percentage_errors)
    average_percentage_error = np.mean(percentage_errors)

    print(f'Mean Absolute Error auf Testdaten: {mae}')
    print(f'Loss on test data: {mse}')
    print(f'Standardabweichung: {math.sqrt(mse)}')
    print(f'R2_Score: {r2}')
    print(f'Average Percentage Error: {average_percentage_error}%')
    plt.plot([0, max(y_test)], [0, max(y_test)], color='red', linestyle='--')

    colors = pow(np.array(x_scaler.inverse_transform(X_test)),1/2.2)
    plt.scatter(y_test, predictions, c=colors)
    plt.xlabel('Wahre Werte')
    plt.ylabel('Vorhersagen')
    plt.title('Vergleich Wahre Werte und Vorhersagen')
    plt.show()

    grautoene = []
    rottoene = []
    gruentoene = []
    blautoene = []
    gelbtoene = []
    lilatoene = []
    tuerkiestoene = []
    for x in range(1000):
        grautoene.append((x/1000, x/1000, x/1000))
        rottoene.append((x/1000, 0, 0))
        gruentoene.append((0, x/1000, 0))
        blautoene.append((0, 0, x/1000))
        gelbtoene.append((x/1000, x/1000, 0))
        lilatoene.append((x/1000, 0, x/1000))
        tuerkiestoene.append((0, x/1000, x/1000))

    feature_names = ['rot', 'gruen', 'blau']
    grautoene = pd.DataFrame(grautoene, columns=feature_names)
    rottoene = pd.DataFrame(rottoene, columns=feature_names)
    gruentoene = pd.DataFrame(gruentoene, columns=feature_names)
    blautoene = pd.DataFrame(blautoene, columns=feature_names)
    gelbtoene = pd.DataFrame(gelbtoene, columns=feature_names)
    lilatoene = pd.DataFrame(lilatoene, columns=feature_names)
    tuerkiestoene = pd.DataFrame(tuerkiestoene, columns=feature_names)

    greyPredictions = model.predict(x_scaler.transform(grautoene))
    rotPredictions = model.predict(x_scaler.transform(rottoene))
    gruenPredictions = model.predict(x_scaler.transform(gruentoene))
    blauPredictions = model.predict(x_scaler.transform(blautoene))
    gelbPredictions = model.predict(x_scaler.transform(gelbtoene))
    lilaPredictions = model.predict(x_scaler.transform(lilatoene))
    tuerkiesPredictions = model.predict(x_scaler.transform(tuerkiestoene))

    plt.plot(greyPredictions, color="grey", label='grey')
    plt.plot(rotPredictions, color="red", label='red')
    plt.plot(gruenPredictions, color="green", label='green')
    plt.plot(blauPredictions, color="blue", label='blue')
    plt.plot(gelbPredictions, color="yellow", label='yellow')
    plt.plot(lilaPredictions, color="purple", label='purple')
    plt.plot(tuerkiesPredictions, color="turquoise", label='turquoise')
    plt.xlabel('Shades')
    plt.ylabel('Candela')
    plt.legend()
    plt.show()


def handle_client(connection, client_address, model, x_scaler):
    try:
        print(f"Connection from {client_address}")

        start_time = time.time()  # Record the start time
        dataSize = 2352 * 1568 * 3 * 4# float size = 4
        data_bytes = b""
        while len(data_bytes) < dataSize:
            chunk = connection.recv(dataSize - len(data_bytes))
            if not chunk:
                print(f"Error: No enough Data received from {client_address}")
                return
            data_bytes += chunk
        print(f"Received {len(data_bytes)} bytes of data from {client_address}")

        numpy_array = np.frombuffer(data_bytes, dtype=np.float32)
        feature_names = ['rot', 'gruen', 'blau']
        rgbValues = pd.DataFrame(numpy_array.reshape(-1, 3), columns=feature_names)
        if len(rgbValues) != dataSize/(3*4):
            print(f"Error: Data amount doesn't match Resolution for {client_address}")
            return
        featured_rgbValues = pd.DataFrame(rgbValues, columns=feature_names)
        prediction = np.round(model.predict(x_scaler.transform(featured_rgbValues), batch_size=int(3687936/5))).astype('uint16').flatten().tobytes()
        connection.sendall(prediction)
        print(f"Send {len(prediction)} bytes to {client_address}")

    except Exception as e:
        print(f"Error handling connection from {client_address}: {str(e)}")

    finally:
        # Calculate the duration and print it
        duration = time.time() - start_time
        print(f"Connection from {client_address} closed. Duration: {duration:.2f} seconds")

        # Close the connection
        connection.close()

def main():
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    if(input("Do you want to run the server local? [y/n] ") == "y"):
        server_ip = "127.0.0.1"
    else:
        server_ip = input("What ist your ip address? ");
    server_port = int(input("On what port should the server run? "))
    model_file = pkg_resources.resource_filename('ANNServer', 'model.keras')
    scaler_file = pkg_resources.resource_filename('ANNServer', 'scaler_model.joblib')
    print(model_file)
    if os.path.exists(model_file) and os.path.exists(scaler_file):
        model = load_model(model_file)
        x_scaler = joblib.load(scaler_file)
        print("Models loaded")
    else:
        print("Model files not found. Exiting.")
        return

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((server_ip, server_port))
    server_socket.listen(5)  # Allow up to 5 connections in the queue
    print(f"Server listening on {server_ip}:{server_port}")

    while True:
        connection, client_address = server_socket.accept()

        # Create a new thread to handle the connection
        client_handler = threading.Thread(target=handle_client, args=(connection, client_address, model, x_scaler))
        client_handler.start()
        client_handlers.append(client_handler)

        for handler in client_handlers:
            if not handler.is_alive():
                handler.join()
                client_handlers.remove(handler)

        time.sleep(0.1)


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--train":
        train()
    else:
        if len(sys.argv) > 1 and sys.argv[1] == "--help":
            print("Use for starting the server: python3.exe server.py")
            print("Use for training the ANN: python3.exe server.py --train")
            print("Use for open this help: python3.exe server.py --help")
        else:
            main()