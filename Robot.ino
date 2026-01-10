/***************************************************************************************
 * Project:      Obstacle-Aware Arduino Robot (LCD UI)
 * File:         Robot.ino
 * Author:       Yaseen Mohamed
 * Date:         24 April 2024
 * Version:      v1.0
 *
 * Summary:
 *   Arduino robot firmware featuring:
 *   - 16×2 LCD UI (LiquidCrystal)
 *   - Ultrasonic distance sensing (TRIG/ECHO)
 *   - Single-button state control (idle ↔ active)
 *   - Forward motion + automatic right-turn avoidance when obstacles are close
 *
 * Notes:
 *   - Button input is configured as INPUT (not INPUT_PULLUP). Ensure you have a
 *     proper pull-up/pull-down resistor in hardware so the pin isn't floating.
 *   - Motor pins assume an H-bridge / motor driver is used (not direct motor drive).
 ***************************************************************************************/

// include the library code for the LCD:
#include <LiquidCrystal.h>

// LCD pin mapping (4-bit mode)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Ultrasonic sensor pin mapping
int trig = 8, echo = 9;

// Motor driver control pins (two motors, two direction pins each)
int motorR1 = 6, motorL1 = 7, motorR2 = 10, motorL2 = 13;

// Single "Enter" button pin
int buttonEnter = A5;

// State machine: 0 = idle, 1 = active
bool state = 0;

// Forward declarations
void stop();
void fw();
void bw();
void RR();
int  usdistcm();
bool checkbutton(int button);

/**
 * setup()
 * Purpose:
 *   One-time initialization for LCD, sensors, motors, and button I/O.
 */
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(motorL1, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR2, OUTPUT);

  // NOTE: INPUT requires external pull-up/pull-down resistor.
  // If your button is wired to ground, consider INPUT_PULLUP and invert logic.
  pinMode(buttonEnter, INPUT);
}

/**
 * loop()
 * Purpose:
 *   Main state machine:
 *   - Idle: stop motors and prompt user to press Enter
 *   - Active: drive forward, display distance, and avoid obstacles by turning right
 */
void loop() {

  // -------------------- IDLE MODE --------------------
  if (state == 0) {
    stop();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press ENTER");

    // Wait until the Enter button is pressed
    while (checkbutton(buttonEnter) == 0) {
      delay(50);
    }

    state = 1;  // move to active mode
  }

  // -------------------- ACTIVE MODE --------------------
  lcd.clear();
  fw();

  // Display live distance (cm) on the first line
  lcd.setCursor(0, 0);
  lcd.print(usdistcm());

  // Soft slow-down / short pause when approaching obstacles to ensure obstacle is detected and not a glitch
  if (usdistcm() < 10) {
    delay(100);
  }

  // If very close, repeatedly turn right until distance increases
  while (usdistcm() <= 7) {
    RR();
    lcd.clear();
    lcd.print("turning right");
    delay(500);
  }

  // If Enter is pressed again, return to idle mode
  if (checkbutton(buttonEnter) == 1) {
    state = 0;
  }
}

/**
 * stop()
 * What:
 *   Stops both motors by setting all motor driver control pins LOW.
 * Why:
 *   Used in idle mode and as a safe default state.
 */
void stop() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR2, LOW);
  digitalWrite(motorR1, LOW);
}

/**
 * fw()
 * What:
 *   Commands both motors to drive forward (based on motor wiring).
 * Why:
 *   Main motion behavior in active mode.
 */
void fw() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR2, HIGH);
  digitalWrite(motorR1, LOW);
}

/**
 * bw()
 * What:
 *   Commands both motors to drive backward (based on motor wiring).
 * Note:
 *   Not used in the current main loop, but kept as a helper for future behaviors.
 */
void bw() {
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR2, LOW);
  digitalWrite(motorR1, HIGH);
}

/**
 * RR()
 * What:
 *   Right-turn behavior (one motor forward, the other backward).
 * Why:
 *   Used as obstacle-avoidance response when the ultrasonic sensor detects a close object.
 */
void RR() {
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR2, LOW);
  digitalWrite(motorR1, HIGH);
}

/**
 * usdistcm()
 * What:
 *   Measures distance in centimeters using an ultrasonic sensor (TRIG/ECHO).
 * How:
 *   - Sends a 10 µs trigger pulse
 *   - Uses pulseIn() to time the echo duration
 *   - Converts time to distance (cm) using speed-of-sound approximation
 * Returns:
 *   Approximate distance in cm (integer).
 */
int usdistcm() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Conversion: distance(cm) ≈ duration(µs) * 0.017
  // (speed of sound ~ 343 m/s; round-trip accounted for)
  return 0.017 * pulseIn(echo, HIGH);
}

/**
 * checkbutton(button)
 * What:
 *   Detects a button press from a specific button and waits for release (simple debounce), and is run regularly in code loop to check for press.
 * How:
 *   - Checks for HIGH
 *   - Short delay for debounce
 *   - If still HIGH, blocks until button is released
 * Returns:
 *   true  (1) if a full press-and-release occurred
 *   false (0) otherwise
 *
 */
bool checkbutton(int button) {
  if (digitalRead(button) == 1)
    delay(2);

  if (digitalRead(button) == 1) {
    // Wait until the user releases the button
    while (digitalRead(button) == HIGH) {
      delay(50);
    }
    return 1;
  } else {
    return 0;
  }
}
