import os
os.environ["TF_ENABLE_ONEDNN_OPTS"] = "0"
from keras.models import load_model
import joblib
import socket
import struct
import numpy as np
import pandas as pd
import threading
import time


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
    server_ip = "172.19.15.159"
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
    main()
