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
//front == flywheel side (true means going backwards)
vex::motor backleft(PORT1);
vex::motor backright(PORT2, true);
vex::motor frontleft(PORT9);
vex::motor frontright(PORT10, true);


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

int indexercontroller() {

  while(1){
    //Indexer
    if (master.ButtonL2.pressing()){
      indexer.spinFor(120, rotationUnits::deg, 100, velocityUnits::pct);
      indexer.spinFor(-130, rotationUnits::deg, 50, velocityUnits::pct);
    }
    else {
      indexer.stop();
    }
    
    //Reset indexer position (accomodating for the shitty spin mech)
    if (master.ButtonA.pressing()){
      indexer.spinFor(-2, rotationUnits::deg, 100, velocityUnits::pct);
    }
    else {
      indexer.stop();
    }
     this_thread::sleep_for(10);
  }
}

int sgn(float num) {
  if(num >= 0) {
    return 1;
  } 
  return 0;
}
bool flywheel_on = false;
void toggle_fw() {
  flywheel_on = !flywheel_on;
}
int main() {
    //int count = 0;

    //double flywheelRatio = 60/8;
    task test = task(indexercontroller);

    float previous_error = 0;
    float gain = .002;
    float tbh = 0;
    float target = 480;
    float output = 0;

    //master.ButtonL1.pressed(toggle_fw);
    
    while(1) {

      Brain.Screen.printAt( 10, 50, "%f", flywheel1.velocity(velocityUnits::rpm)); //%f//, (float)frontleft.rotation(rotationUnits::rev));
        // Allow other tasks to run
      
      //Wheels and movement
      backright.spin(directionType::fwd, master.Axis3.value() + master.Axis1.value() * .7, velocityUnits::pct);
      backleft.spin(directionType::fwd, master.Axis3.value() - master.Axis1.value() * .7, velocityUnits::pct);
      frontright.spin(directionType::fwd, master.Axis3.value() + master.Axis1.value() * .7, velocityUnits::pct);
      frontleft.spin(directionType::fwd, master.Axis3.value() - master.Axis1.value() * .7, velocityUnits::pct);

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
      if(master.ButtonL1.pressing()) {
        flywheel1.spin(fwd, 100, velocityUnits::pct);
        flywheel2.spin(fwd, 100, velocityUnits::pct);
      }
      else {
        flywheel1.stop();
        flywheel2.stop();
      }
     

      if(master.ButtonA.pressing()) {
        target += 5;
        master.Screen.print("%f", target);
      }
      if(master.ButtonB.pressing()) {
        target -= 5;
        master.Screen.print("%f", target);
      }

      //Brain.Screen.printAt( 10, 50, "Hello V5 %f", flywheel1.velocity(velocityUnits::rpm) * flywheelRatio );
      //Brain.Screen.printAt( 10, 80, "hi angel");
      //master.Screen.print("%f", flywheel1.velocity(velocityUnits::rpm));
      
      Brain.Screen.printAt( 10, 80, "play omori today");
      Brain.Screen.printAt( 10, 130, "add me on osu");


      //if(flywheel_on) {
        //tbh
      //  float error = target - flywheel1.velocity(velocityUnits::rpm);
        
      //  output += gain * error;
        
      //  if(sgn(error) != sgn(previous_error)) {
      //    output = .5 * (output + tbh);
      //    tbh = output;
      //    previous_error = error;
      //  }
        
      //  flywheel1.spin(directionType::fwd, output, voltageUnits::volt);
      // flywheel2.spin(directionType::fwd, output, voltageUnits::volt);
        
      //}
      //else{
      //  flywheel1.stop();
      //  flywheel2.stop();
      //}
        
      this_thread::sleep_for(10);


      
    }
}
