#include <AccelStepper.h>
#include <MultiStepper.h>
#include "Wire.h"
#include "math.h"

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

class Point {
public:
  // Constructor
  Point(long x, long y, long z) : 
    x_(x), y_(y), z_(z) {}

  // Overloaded constructor that creates a point at the origin
  Point() : x_(0), y_(0), z_(0) {}

  // Getters for the coordinates
  long getX() { return x_; }
  long getY() { return y_; }
  long getZ() { return z_; }

  // Moves from any position to this point with the pen raised
  // as the bird flies (i.e. euclidian distance)
  void moveToThisPointRaised() {
    // Raise the pen off the page, this is its own
    // step so we don't get accidental dragging
    long raised[3] { 
      stepper_x.currentPosition(), 
      stepper_y.currentPosition(), 
      -100
    };
    steppers.moveTo(raised);
    steppers.runSpeedToPosition();
    delay(500);

    // Now go to position with pen raised
    this->raisePen();
    steppers.moveTo(this->toArray());
    steppers.runSpeedToPosition();
    delay(500);
  }

  // Moves from any position to this point with the pen lowered
  // as the bird flies (i.e. euclidian distance)
  void moveToThisPointLowered() {
    // Lower the pen off the page, this is its own
    // step so we don't lower the pen too late
    long raised[3] {
      stepper_x.currentPosition(), 
      stepper_y.currentPosition(), 
      0
    };
    steppers.moveTo(raised);
    steppers.runSpeedToPosition();
    delay(500);

    // Now go to position with pen lowered
    this->lowerPen();
    steppers.moveTo(this->toArray());
    steppers.runSpeedToPosition();
    delay(500);
  }

private:
  long x_;  // negative for left and positive for right
  long y_;  // negative for out and positive for in
  long z_;  // negative for up and positive for down

  // Converts the struct to an array
  long positions[3];
  long* toArray() {
    positions[0] = x_;
    positions[1] = y_;
    positions[2] = z_; 
    return positions;
  }

  // Edits the z-position of this point to lift the pen
  void raisePen() {
    z_ = -100;
  }

  // Returns the z-position of this point to its default state
  void lowerPen() {
    z_ = 0;
  }
  
};

class Line {
public:
  // Constructor which creates a Line out of two poitns
  Line(Point p1, Point p2) : p1_(p1), p2_(p2) {
    // Get the distance between the two points that make 
    // up the line with Pythagoras' Theorem
    long a = p1_.getX() - p2_.getX();
    long b = p1_.getY() - p2_.getY();
    len_ = sqrt(sq(a) + sq(b));

    // Use trig to get the theta in radians
    theta_ = atan2(b, a);  
  }

  // Overloaded Constructor which creates a Line of a
  // given length specified by `len` and `theta`
  Line(Point p1, long len, float theta) : p1_(p1), len_(len), theta_(theta) {
    long adjacent = len * cos(theta);
    long x = p1_.getX() + adjacent;
    long opposite = len * sin(theta);
    long y = p1_.getY() + opposite;
    Point p2(x, y, 0);
    p2_ = p2;
  }

  /* 
   *  Draws a line by controlling the stepper motors. The drawing
   *  surface is abstracted to be a 2D cartesian plane such as
   *  below:
   *  
   *   y
   *   |
   *   |         o p2
   *   |
   *   |  o p1
   *   +------------- x
   */
  void drawLine() {
//    p1_.moveToThisPointRaised();
    p2_.moveToThisPointLowered(); 
  }

  // Getters for the points
  Point* getP1() { return &p1_; }
  Point* getP2() { return &p2_; }
  float getTheta() { return theta_; }

private:
  Point p1_;
  Point p2_;
  long len_;
  float theta_;
  
};


/*
 * Draws an angle by controlling the stepper motors. The drawing
 * surface is abstracted to be a 2D cartesian plane where an angle
 * would look like this:
 * 
 *   y
 *   | p1 o
 *   |    |     
 *   | p2 o------o p3   
 *   |     
 *   +------------- x
 *   
 * This function DOES NOT calculate the angle, rather it just draws
 * the lines that make up the angle based on the input points
 */
//void drawAngle(Point p1, Point p2, Point p3) {
//  drawLine(p1, p2);
//  drawLine(p2, p3);
//}

//void drawAngle(Point start, float theta, ) {
//  
//}

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

  Serial.begin(9600);
}

Point createShape1(Point* start, float shift)  {
  // 3.48 * 40 ~ 139
  // 2.20 * 40 ~ 88
  Line l1(*start, 139, 7*PI/20 + shift);
  Line l2(*(l1.getP2()), 88, 33*PI/20 + shift);
  l1.drawLine();
  l2.drawLine();
  return *(l2.getP2());
}

Point createShape2(Point* start, float shift) {
  // 4.24 * 40 ~ 168
  // 3.60 * 40 ~ 144
  // 2.07 * 40 ~ 83
  Line l1(*start, 168, 7*PI/20 + shift);
  Line l2(*(l1.getP2()), 144, 33*PI/20 + shift);
  Line l3(*(l2.getP2()), 83, PI + shift);
  l1.drawLine();
  l2.drawLine();
  l3.drawLine();
  return *(l3.getP2());
}

void loop() {
  // Positions are relative to someone looking at the printer with the
  // controller in the back
             
  // +----------------------+
  // |                      |
  // | ~150                 |
  // |                      |
  // |      ~230            |
  // +----------------------+ 

//  stepper_x.moveTo(10);
//  while (stepper_x.currentPosition() != 10)
//    stepper_x.run();
//  stepper_y.moveTo(-10);
//  while (stepper_y.currentPosition() != -10)
//    stepper_y.run();
//  stepper_z.moveTo(-100);
//  while (stepper_z.currentPosition() != 10)
//    stepper_z.run();

////////////// Shape Outline ///////////////////
//  Point origin(0, 0, 0);
//  Line lo1(origin, Point(-12, 22, 0));
//  Line lo2(*(lo1.getP2()), 168, 7*PI/20);
//  Line lo3(*(lo2.getP2()), -25, PI);
//  Line lo4(*(lo3.getP2()), -168, 13*PI/20);
//  Line lo5(*(lo4.getP2()), 25, - lo1.getTheta() - PI);
//  Line lo6(*(lo5.getP2()), 156, PI);
//  lo1.drawLine();
//  lo2.drawLine();
//  lo3.drawLine();
//  lo4.drawLine();
//  lo5.drawLine();
//  lo6.drawLine();
///////////// Shape Innards ///////////////////
//  Line li1(*(lo6.getP2()), 139, 7*PI/20);
//  Line li2(*(li1.getP2()), 88, 33*PI/20);
//  li1.drawLine();
//  li2.drawLine();
//  lo2.getP2()->moveToThisPointRaised();
//  Line li3(*(lo2.getP2()), 144, 33*PI/20);
//  Line li4(*(li3.getP2()), 83, PI);
//  li3.drawLine();
//  li4.drawLine();
//  lo4.getP2()->moveToThisPointRaised();
//  Line li5(*(lo4.getP2()), 131, PI);
//  Line li6(*(li5.getP2()), 88, 7*PI/20);
//  li5.drawLine();
//  li6.drawLine();
  exit(0);
}
