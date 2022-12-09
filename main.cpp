/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\127-alee                                         */
/*    Created:      Fri Nov 18 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
vex::controller master(controllerType::primary);

// define your global instances of motors and other devices here
//wheels
//front == flywheel side
vex::motor backleft(PORT1, true);
vex::motor backright(PORT2);
vex::motor frontleft(PORT9, true);
vex::motor frontright(PORT10);


//flywheel, 1 is closer to the front
vex::motor flywheel1(PORT6, gearSetting::ratio6_1);
vex::motor flywheel2(PORT8, gearSetting::ratio6_1, true);

//other
vex::motor intake(PORT13, true);
vex::motor indexer(PORT19, true);

vex::motor test(PORT12, true);

double wheelWidth = 4.125;
double wheelCirc = wheelWidth * 3.14;

void pdrivedistance(double distance, double kP) {
  double revolutions = distance / wheelCirc;
  bool done = false;
  while(!done) {
    Brain.Screen.printAt( 10, 50, "Hello V5 %f", (float)frontleft.rotation(rotationUnits::rev));
    double error = revolutions - frontleft.rotation(rotationUnits::rev); 
    double power = error * kP;
    frontleft.spin(directionType::fwd, power, velocityUnits::pct);
    frontright.spin(directionType::fwd, power, velocityUnits::pct);
    backleft.spin(directionType::fwd, power, velocityUnits::pct);
    backright.spin(directionType::fwd, power, velocityUnits::pct);
  }

}
void drivedistance(double distance, double velocity) { //inches
  double revolutions = distance / wheelCirc;

  frontleft.rotateFor(directionType::fwd, revolutions, rotationUnits::rev, velocity, velocityUnits::pct, false);
  backleft.rotateFor(directionType::fwd, revolutions, rotationUnits::rev, velocity, velocityUnits::pct, false);
  frontright.rotateFor(directionType::fwd, revolutions, rotationUnits::rev, velocity, velocityUnits::pct, false);
  backright.rotateFor(directionType::fwd, revolutions, rotationUnits::rev, velocity, velocityUnits::pct, true);

}
int main() {
    int count = 0;

    double flywheelRatio = 60/8;

    while(1) {

      Brain.Screen.printAt( 10, 50, "Hello V5 %f", (float)frontleft.rotation(rotationUnits::rev));
        // Allow other tasks to run
      
      //Wheels and movement
      backleft.spin(directionType::fwd, master.Axis3.value() + master.Axis4.value(), velocityUnits::pct);
      backright.spin(directionType::fwd, master.Axis3.value() - master.Axis4.value(), velocityUnits::pct);
      frontleft.spin(directionType::fwd, master.Axis3.value() + master.Axis4.value(), velocityUnits::pct);
      frontright.spin(directionType::fwd, master.Axis3.value() - master.Axis4.value(), velocityUnits::pct);
    
    if (master.ButtonA.pressing()){
      pdrivedistance(12, 100);
    }
      //Intake
      if (master.ButtonR1.pressing()){
        intake.spin(directionType::fwd, 100, velocityUnits::pct);
      }
      else if (master.ButtonR2.pressing()){
        intake.spin(directionType::rev, 100, velocityUnits::pct);
      }
      else {
        intake.stop();
      }

    if (master.ButtonL2.pressing()){
      indexer.spinFor(90, rotationUnits::deg, 75, velocityUnits::pct);
      indexer.spinFor(-90, rotationUnits::deg, 50, velocityUnits::pct);
    }
    else {
      indexer.stop();
    }

      //Flywheel
      if (master.ButtonL1.pressing()){
        flywheel1.spin(directionType::fwd, 100, velocityUnits::pct);
        flywheel2.spin(directionType::fwd, 100, velocityUnits:: pct);
      }
      else {
        flywheel1.stop();
        flywheel2.stop();
      }


      //Brain.Screen.printAt( 10, 50, "Hello V5 %f", flywheel1.velocity(velocityUnits::rpm) * flywheelRatio );
      Brain.Screen.printAt( 10, 80, "hi angel");
      this_thread::sleep_for(10);
    }
}
