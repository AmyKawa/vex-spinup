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
//vex::motor x();



int main() {
    int count = 0;

    double flywheelRatio = 60/8;

    while(1) {

      Brain.Screen.printAt( 10, 50, "Hello V5 %d", count++ );
        // Allow other tasks to run
      
      //Wheels and movement
      backleft.spin(directionType::fwd, master.Axis3.value() + master.Axis4.value(), velocityUnits::pct);
      backright.spin(directionType::fwd, master.Axis3.value() - master.Axis4.value(), velocityUnits::pct);
      frontleft.spin(directionType::fwd, master.Axis3.value() + master.Axis4.value(), velocityUnits::pct);
      frontright.spin(directionType::fwd, master.Axis3.value() - master.Axis4.value(), velocityUnits::pct);

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

      //Flywheel
      if (master.ButtonL1.pressing()){
        flywheel1.spin(directionType::fwd, 60, velocityUnits::pct);
        flywheel2.spin(directionType::fwd, 60, velocityUnits:: pct);
      }
      else {
        flywheel1.stop();
        flywheel2.stop();
      }


      Brain.Screen.printAt( 10, 50, "Hello V5 %f", flywheel1.velocity(velocityUnits::rpm) * flywheelRatio );
      Brain.Screen.printAt( 10, 80, "hi angel");
      this_thread::sleep_for(10);
    }
}
