# ImageLight

## Overview

**ImageLight** is a specialized tool for extracting luminance from images captured by a calibrated camera, presenting the results in a false-color image. It leverages an artificial neural network to approximate luminance based on RGB values.
<img width="400" alt="Screenshot App" src="https://github.com/rclarsfull/ImageLight/assets/19623214/a7656576-bb96-476f-b357-092ab680b947">

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
- **IP Address** defines the address where your server runs. When it runs locally, enter "127.0.0.1".
- **Port** defines the port on which the server runs.
- **Timeout** specifies how many milliseconds the client waits for a response from the server.
- **Language for '.csv' File Save** This option lets you decide the formatting for "." and "," that your country uses. For example, "1/2" in Germany would be "0,5", while in England it would be "0.5".
- **Calibrate Lens Correction** attempts to correct the Vignette of your lens. It's not perfect, but it reduces the Vignette effect of your lens. This requires an extra calibration step.
- **Calibration Mode** disables the connection to the server. In this mode, you can collect Linear RGB data from your image for the calibration process.


<img width="400" alt="Screenshot Settings" src="https://github.com/rclarsfull/ImageLight/assets/19623214/707bbc23-0c53-4fc0-ac8e-43b1b23fe941">

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

1. Calibrate Camera as described above for a Candela Range between 50 and 200.
2. Open a blank white image on a good TFT monitor that features an even brightness.
3. Take a photo with your camera from the center of the Monitor (In my case, the camera lens touches the monitor).
4. Drag&Drop the image into ImageLight. Now you should see the error caused by the Lens in the false color image.
5. Go to: Extra -> Settings -> Calibrate Lens Correction.
6. Now you need to do a full calibration. Don't reuse the RGB Values from the first calibration. You need to redo Step 4 in calibration for the data from the first calibration.
7. Expand you Dataset

  

