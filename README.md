# Alpha Hearing

## Disclaimer
**This project is not a real medical device and is intended solely as a project and proof of concept. It should not be used as a substitute for professional medical advice, diagnosis, or treatment. Please consult a licensed medical professional for any concerns regarding your hearing health.**

## Visuals
![Device Image](/images/device.png)
![Circuit Diagram](/images/circuit.png)

## Introduction
Alpha Hearing, also known as Portable Hearing Test (PHT), is a device designed to conduct self-diagnostic hearing tests. The device plays audio tones from an SD card and logs the decibel level at which the user detects the sound, providing a quick assessment of the user's hearing capabilities. This device is the first functioning prototype created as a 2nd Year Electrical Engineering Project.

## Features
- **Portable and Easy to Use**: The device is handheld and operates with simple button presses.
- **Medical Instrument**: Designed to help individuals assess their hearing health.
- **Affordable**: Built with cost-effective components to make hearing tests accessible to everyone.

## Usage
1. Plug in a high-quality headset into the 3.5mm audio jack.
2. Power on the device.
3. Press the ‘SELECT’ button to start the hearing test.
4. Press 'RESET' at any point if you make a mistake, it will reset the test.
5. Press the ‘SELECT’ button when you hear a tone.
6. The result will be displayed via LED indicators.

## Installation
### Hardware
- PIC16F1778 Microcontroller
- SD Card
- Headphones
- 3.5mm Audio Jack
- BreadBoard
- LEDs (Green, Yellow, Red)
- Amplifier and Bandpass Filter Circuit
- Voltage Regulator Circuit
- 9-Volt Battery
- 3D Printed Enclosure

### Software
1. Clone the repository:
    ```sh
    git clone https://github.com/sami-abd/Alpha-Hearing.git
    ```
2. Open the project in MPLAB IDE.
3. Load the necessary libraries and compile the code.
4. Upload the compiled code to the PIC16F1778 microcontroller.

## Contributors
- Nizar Abdul-Halim
- Sami Abdelhalem
- Mustafa Sheikh


