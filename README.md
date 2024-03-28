# ImageLight

## Overview

**ImageLight** is a specialized tool for extracting luminance from images captured by a calibrated camera, presenting the results in a false-color image. It leverages an artificial neural network to approximate luminance based on RGB values.

<img width="400" alt="Screenshot App" src="https://github.com/rclarsfull/ImageLight/assets/19623214/a7656576-bb96-476f-b357-092ab680b947">

## Installing Python 3.9 and ANNServer Application

This README provides instructions for installing Python 3.9 with PATH enabled, extracting the ANNServer application, and then installing it using `pip install`.

### Installing Python 3.9 with PATH enabled

1. **Download Python 3.9:**
   
   Visit the [official Python website](https://www.python.org/downloads/) and download the installer for Python 3.9 suitable for your operating system (Windows, macOS, or Linux).
   Watch out [3.9.13](https://www.python.org/downloads/release/python-3913/) is th last version released with an installer.

2. **Run the Installer:**

   Double-click the downloaded installer to run it. Follow the on-screen instructions provided by the installer.

3. **Enable PATH (for Windows only):**

   During the installation process, make sure to check the box that says "Add Python 3.9 to PATH." This will allow you to run Python and pip commands from the command line without specifying the full path to the executable.

4. **Verify Installation:**

   Open a terminal or command prompt and type the following command to verify that Python 3.9 has been installed successfully:

   ```
   python --version
   ```

   You should see output similar to `Python 3.9.x`.

## Extracting the ANNServer Application

1. **Download and Extract the Application:**

   Prior to installing ImageLight, download the application from the gihub releases. The downloaded folder should be extracted using WinRAR. Navigate to the folder containing the extracted files.

## Installing ANNServer using pip

1. **Navigate to the Application Directory:**

   Open the extracted folder containing the ANNServer application. 

2. **Open Command Prompt:**

   Inside the "ImageLight vx.x.x" folder, go to the ArtificalNeuronalColorNetwork folder, locate an empty space (not on any file) and hold down the "Shift" key. Right-click and select "Open PowerShell window here" from the context menu.

3. **Install the Application:**

   In the PowerShell window, use the `pip install` command followed by a dot (`.`) to install the ANNServer application from the current directory. For example:

   ```
   pip install .
   ```

   This command will install the ANNServer application along with its dependencies.
   For Windows you can alternatively use the `installANNServer.bat` Script.


## Start the ANNServer:

   Once installed, you can start the ANNServer by typing `ANNServer` in the console and hitting Enter. This command will initiate the server.
   ```
   ANNServer
   ```

   For Windows you can alternatively use the `run.bat` Script.


## Training the ANNServer:

   To train the ANNServer, type `TrainANNServer` in the console and hit Enter. This command will trigger the training process.
   ```
   TrainANNServer
   ```

   For Windows you can alternatively use the `Train ANN.bat` Script.

Following these steps should enable you to install Python 3.9 with PATH enabled, extract the ImageLight application, and then install it using `pip install`. 
If you have any questions or need further assistance, feel free to reach out to the application's developer for support.


## Usage

### Program Initiation

The tool can be launched through a batch script, this is initializing a server for the neural network that approximates the luminance and an client interface. Users need to enter the server address and a port (Standard setting: IP: 127.0.0.1, port: 12345) in the cmd to start the server.
![Screenshot Server](https://github.com/rclarsfull/ImageLight/assets/19623214/02637495-36bd-4ee3-a019-093a40104db8)


Ignore the depricated warnings.

The script also launches a client where users can drag and drop the image for conversion (Images must have a resolution of 2352 x 1568 pixels).


### Client Interface
The client window has a frame with two tabs, one for the original image and one for the generated false-color image. Users can draw windows, each showing an ID and average luminance. The accuracy of this approximation is dependent on the quantity, quality, and diversity of calibration data. Under The Frame are two sliders, with them, you can change the color spectrum for the false-color image to your liking. By default, they use the highest and lowest values of your image.

<img width="400" alt="Screenshot ClientInterface" src="https://github.com/rclarsfull/ImageLight/assets/19623214/83c28aed-8c9f-477e-ad38-56207888b94f">

### Client Settings

- **Label on every x Value in Scale** defines which numbers will be shown in the Legend on the right. It shows every value that is divided by x and has no remaining number.
- **Max Candela** defines the highest value for which your ANN (Artificial Neural Network) was trained.
- **Exposure Time Trained** defines the exposure time you used in you training Images as you trained the artififcal network.
- **Exposure Time Image** defines the exposure time used in the Images you want to take Messurments from. (Use with caution!! Make sure that your network ist trained for the modified luminance. The app calculates modifieres for candala  based on the relation betwean the exposuretimes. `(EXPOSURE_TIME_TRAINED/EXPOSURE_TIME_IMAGE) * candela`)
- **IP Address** defines the address where your server runs. When it runs locally, enter "127.0.0.1".
- **Port** defines the port on which the server runs.
- **Timeout** specifies how many milliseconds the client waits for a response from the server. (Increase this if you get a completely green false-color image)
- **Language for '.csv' File Save** This option lets you decide the formatting for "." and "," that your country uses. For example, "1/2" in Germany would be "0,5", while in England it would be "0.5".
- **Calibrate Lens Correction** attempts to correct the Vignette of your lens. It's not perfect, but it reduces the Vignette effect of your lens. This requires an extra calibration step.
- **Calibration Mode** disables the connection to the server. In this mode, you can collect Linear RGB data from your image for the calibration process.

<img width="400" alt="Screenshot 2024-02-23 164221" src="https://github.com/rclarsfull/ImageLight/assets/19623214/93cbef21-921f-4c1d-b3e7-b26bbbf7f6b2">



## Calibration

### Required Equipment for Calibration

- **Camera:** Fixed focal length lens, preferably fisheye
- **Luminance meter:** We used: [LS-150](https://www.konicaminolta.de/de-de/hardware/messgeraete/lichtmessung/leuchtdichtemessgeraete/ls-150-ls-160)
- **Tripod:** Stable support for capturing images
- **Room:** Darkened space to eliminate measurement errors due to changing sunlight
- **Display:** Projector or color chart for controlled lighting conditions

### Workflow for Calibration

1. **Measurement Setup:**
   - Conduct data collection in a room with a projector and fully controlled lighting.
   - Block any sunlight for precise measurements.
   - Set up a tripod perpendicular to the screen.

2. **Capture Images:**
   - Capture images in manual mode, adjusting settings to avoid overexposure or underexposure in the relevant measurement range.
   - Deactivate all automatic features of the camera like auto-iso, auto-light-correction. Only autofocus should be activated.
   - Do not change these settings anymore. Your calibration only works for one configuration.
   - Take a picture.
   - Swap the camera for the luminance meter without moving the tripod, maintaining consistent angles.

3. **Record Luminance Measurements:**
   - Record measurements at key points in the image, ensuring the same angles for accurate comparisons.

4. **Data Matching:**
   - Activate the "Calibration mode" in the settings of ImageLight to gather data without sending it to the neural network.
   - Drag & Drop these images to Image Light, draw measurement boxes on the same key points, and export the data as a CSV file.
   - Match the luminance values to the corresponding points and collect them in one large Excel sheet.

5. **Collect more Data:**
   - Repeat this progress for the whole spectrum of candela you are trying to measure, ensuring a minimum of 100 entries with varied brightness settings for satisfactory results. Use as many different colors as possible. Diversity is key.

## Training

1. **Data Preparation:**
   - Delete all old entries in "data.xlsx".
   - Copy all training data into the "data.xlsx" file.

2. **Training Initiation:**
   - Run the "train.bat" batch script to initiate network training.

3. **Training Duration:**
   - Training duration varies (5-20 minutes).

4. **Performance Analysis:**
   - After completion, performance parameters are displayed.

5. **If needet: Hyperparameter Adjustment:**
   - Adjust hyperparameters in the "main.py" file if your dataset significantly differs from the provided one.
   - Pay attention to values in the LearnRateScheduler, choosing smaller numbers for longer but potentially better performance or larger numbers for shorter training times.

## Lens Correction

The Lens Correction tries to mitigate the Vignette of the Lens.
You need an already calibrated Camera to Correct the Vignette of the Lens.

1. Calibrate Camera as described above for a Candela Range between 50 and 200(typical brightness for an normal monitor).
2. Open a blank white image on a good TFT monitor that features an even brightness.
3. Take a photo with your camera from the center of the Monitor (In my case, the camera lens touches the monitor).
4. Drag&Drop the image into ImageLight. Now you should see the error caused by the Lens in the false color image.
5. Go to: Extra -> Settings -> Calibrate Lens Correction.
6. Now you need to do a full calibration. Don't reuse the RGB Values from the first calibration. You need to redo Step 4 in calibration for the data from the first calibration.
7. Expand you Dataset

  

