# Arduino Obstacle-Aware Robot (with LCD UI)

Arduino firmware for a two-motor robot featuring a **16×2 LCD interface**, **ultrasonic distance sensing**, and a single **Enter button** to control runtime behavior.

> The robot chassis/body is from a basic two-motor kit (mechanical frame only).  
> The firmware behavior, sensor/LCD integration, and pin-level control logic were implemented separately.

<img src="Prosthetic-Hand-Image.jpg" alt="Arduino Robot img 1" width="650"> <img src="Prosthetic-Hand-Image.jpg" alt="Arduino Robot img 2" width="650">

## What it does
- **Idle mode:** motors stop and the LCD prompts: `Press ENTER`
- **Active mode:** drives forward and displays **live distance (cm)** on the LCD
- **Obstacle avoidance:** if an object is detected within a close range, the robot repeatedly **turns right** and prints `turning right` until the distance increases (Ultrasonic sensor is to be facing forward, but is facing sideways in design due to breadboard design constraints)

## Demo
Check the demo file in the repo, and use this link to demo folder with all videos:
https://drive.google.com/file/d/1gGviGKnixNbjjOhSUq4s3DY6O76Xo-3T/view?usp=sharing

### Distance thresholds (currently used)
- If distance `< 10 cm` → small delay then check if it gets below 7 cm
- While distance `<= 7 cm` → keep turning right (`RR()`)

## Tech / Libraries
- Arduino (C++)
- **LiquidCrystal** (`#include <LiquidCrystal.h>`)
  - Included with the Arduino IDE by default (no manual install needed)

## Hardware
- Arduino UNO microcontroller
- 16×2 LCD (HD44780 or similar)
- Ultrasonic sensor (HC-SR04 or similar)
- 2 DC motors (5V)
- Push button (Enter)
- Breadboard or PCB
- Power source for Arduino (9V battery)

## Pin Mapping to Arduino (Could be modified to match code modifications)
### LCD (LiquidCrystal)
- RS = 12
- EN = 11
- D4 = 5
- D5 = 4
- D6 = 3
- D7 = 2

### Ultrasonic
- TRIG = 8
- ECHO = 9

### Motors (2 pins for each motor to control direction (speed could be controlled using Analog pins))
- motorR1 = 6
- motorL1 = 7
- motorR2 = 10
- motorL2 = 13

### Button
- Enter button = A5
The code treats a press as `HIGH` and uses basic debounce + wait-for-release logic, and is used to leave idle mode and get back to it.

## How to run
1. Connect the pins using the pinout above
2. Open `robot.ino` in the Arduino IDE
3. Select your board and port
4. Upload
5. Press the Enter button to start the active mode (and press again to return to idle)
