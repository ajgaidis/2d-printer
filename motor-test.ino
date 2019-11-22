#include <AccelStepper.h>
#include <MultiStepper.h>
#include "Wire.h"

#define X_STEP_PIN             37
#define X_DIR_PIN              48
#define X_MIN_PIN              12
#define X_MAX_PIN              -1
#define X_ENABLE_PIN           29
#define X_MS1_PIN              40
#define X_MS2_PIN              41

#define Y_STEP_PIN             36
#define Y_DIR_PIN              49
#define Y_MIN_PIN              11
#define Y_MAX_PIN              -1
#define Y_ENABLE_PIN           28
#define Y_MS1_PIN              69
#define Y_MS2_PIN              39

#define Z_STEP_PIN             35
#define Z_DIR_PIN              47
#define Z_MIN_PIN              10
#define Z_MAX_PIN              23
#define Z_ENABLE_PIN           27
#define Z_MS1_PIN              68
#define Z_MS2_PIN              67

#define E0_STEP_PIN            34
#define E0_DIR_PIN             43
#define E0_ENABLE_PIN          26
#define E0_MS1_PIN             65
#define E0_MS2_PIN             66

#define MOTOR_CURRENT_PWM_XY_PIN 46
#define MOTOR_CURRENT_PWM_Z_PIN  45
#define MOTOR_CURRENT_PWM_E_PIN  44
#define LED_PIN             13

AccelStepper stepper_x(1, X_STEP_PIN, X_DIR_PIN); // 1 = Driver
AccelStepper stepper_y(1, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepper_z(1, Z_STEP_PIN, Z_DIR_PIN);

MultiStepper steppers;

void setup() {  

  analogWrite(46,135);
  analogWrite(45,135);
  analogWrite(44,135);
  
  stepper_x.setMaxSpeed(100);
  stepper_x.setSpeed(100);
  stepper_x.setAcceleration(1000);
  stepper_x.setEnablePin(X_ENABLE_PIN);
  stepper_x.setPinsInverted(false, false, true); //invert logic of enable pin
  stepper_x.enableOutputs();

  stepper_y.setMaxSpeed(100);
  stepper_y.setSpeed(100);
  stepper_y.setAcceleration(1000);
  stepper_y.setEnablePin(Y_ENABLE_PIN);
  stepper_y.setPinsInverted(false, false, true); //invert logic of enable pin
  stepper_y.enableOutputs();

  stepper_z.setMaxSpeed(100);
  stepper_z.setSpeed(100);
  stepper_z.setAcceleration(1000);
  stepper_z.setEnablePin(Z_ENABLE_PIN);
  stepper_z.setPinsInverted(false, false, true); //invert logic of enable pin
  stepper_z.enableOutputs();

  steppers.addStepper(stepper_x);
  steppers.addStepper(stepper_y);
  steppers.addStepper(stepper_z);
}

int count = 0;

void loop() {
  // Positions are relative to someone looking at the printer with the
  // controller in the back
  
  // The current Z direction is negative for up and positive for down
  // The current Y direction is negative for out and positive for in
  // The current X direction is negative for left and positive for right
             
  // +----------------------+
  // |                      |
  // | ~150                 |
  // |                      |
  // |      ~230            |
  // +----------------------+ 

  long positions[3]; /* Array of desired stepper positions */

//  positions[0] = count;
//  positions[1] = 150;
//  positions[2] = 0;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); /* Blocks until all motors are in position */
//  delay(1000);
//
//  count -= 10;
//
//  positions[0] = count;
//  positions[1] = 150;
//  positions[2] = 0;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); /* Blocks until all motors are in position */
//  delay(1000);
//
//  positions[0] = count;
//  positions[1] = 0;
//  positions[2] = 0;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); /* Blocks until all motors are in position */
//  delay(1000);
//
//  count -= 10;
//
//  positions[0] = count;
//  positions[1] = 0;
//  positions[2] = 0;
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); /* Blocks until all motors are in position */
//  delay(1000);


  positions[0] = 0;
  positions[1] = 0;
  positions[2] = 0;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); /* Blocks until all motors are in position */
  delay(1000);

//  stepper_x.moveTo(-50);
//  while (stepper_x.currentPosition() != -50)
//    stepper_x.run();
//  stepper_z.moveTo(-100);
//  while (stepper_z.currentPosition() != -100)
//    stepper_z.run();
}
