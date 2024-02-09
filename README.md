# ImageLight

## Overview

**ImageLight** is a specialized tool for extracting luminance from images captured by a calibrated camera, presenting the results in a false-color image. It leverages an artificial neural network to approximate luminance based on RGB values.

## Usage

### Program Initiation

The tool can be launched through a batch script, initiating a server for the neural network that approximates images. Users need to enter the server address and a port (Standard setting: IP: 127.0.0.1, port: 12345). The script also launches a client where users can drag and drop the image for conversion (Images must have a resolution of 2352 x 1568 pixels).

### Client Interface
The Client Window has a Frame with two tabs, one for the original image and one for the generated false-color image. Users can draw windows, each showing an ID and average luminance. The accuracy of this approximation is dependent on the quantity, quality, and diversity of calibration data. Under The Frame are two sliders; with them, you can change the color spectrum for the false-color image to your liking. By default, they use the highest and lowest values of your image.

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
   - Repeat this progress for the whole spectrum of candela you are trying to measure, ensuring a minimum of 100 entries with varied brightness settings for satisfactory results.

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

5. **Hyperparameter Adjustment:**
   - Adjust hyperparameters in the "main.py" file if your dataset significantly differs from the provided one.
   - Pay attention to values in the LearnRateScheduler, choosing smaller numbers for longer but potentially better performance or larger numbers for shorter training times.
