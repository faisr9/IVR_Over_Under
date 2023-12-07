#include "common_code/odom.h"
#include <cmath>
#include <vector>
#include <string>
#include "pros/adi.hpp"
#include "main.h"
#include "pros/imu.h"

/*
Authors: Ethan Lenning, Reid Faistl
Originally adapted from 2022-23 club repository
https://github.com/upai02/IVR-Spin-Up/blob/master/15-inch/spin-up-15/src/misc/PositionTracker.cpp

The Odom class acts as an odometer for the robot which calls it. A robot should instantiate its own Odom object. The goal of the odometer is to track the
robot's position at any given time, by tracking how far the tracking wheels move and what angle the robot is facing when the wheels are moved.

The tracking wheels cannot tell if the robot turns, so the heading (angle) needs to be updated during the process in order to calculate the actual signs
and magnitudes of the x and y movements. Thus, if it moves right 1m and then turns 180 degrees and moves "right" 1m (in the robot's frame of reference), 
currentTransverseValue will be the equivalent of 2m even though from an outside perspective it moved right 1m and then left 1m and is back at its origin.
This class has accounted for that, so that other code can assume the position of the robot is reliable.
*/

pros::Motor FrontTopRight(11, pros::E_MOTOR_GEARSET_06, true);
pros::Motor FrontBottomRight(12, pros::E_MOTOR_GEARSET_06);
pros::Motor BackRight(13, pros::E_MOTOR_GEARSET_06);
pros::Motor FrontTopLeft(20, pros::E_MOTOR_GEARSET_06);
pros::Motor FrontBottomLeft(19, pros::E_MOTOR_GEARSET_06, true);
pros::Motor BackLeft(18, pros::E_MOTOR_GEARSET_06, true);

pros::MotorGroup RightDrive({FrontTopRight, FrontBottomRight, BackRight});
pros::MotorGroup LeftDrive({FrontTopLeft, FrontBottomLeft, BackLeft});
Controller master(E_CONTROLLER_MASTER);
Imu imu(21);
Odom odometer(imu);

void initialize() {
	pros::lcd::initialize(); // Temp until custom GUI
	imu.reset();
    odometer.initTracker(0, 0, 0);
}

Odom::Odom(pros::IMU theImu): imu(theImu), vertical_track('E','D',false), horizontal_track('C','B',true) {

    transverseWheelRad = 1.96 * 0.0254 / 2; // transverse wheel tracks left to right movements
    radialWheelRad = 1.96 * 0.0254 / 2;     // radial wheel tracks forward and backward movements (has nothing to do with radians)
    lastTransverseValue = 0;                // these variables track what the last value was in order to determine how far the robot has moved
    lastRadialValue = 0;
    last_x_tracking_offset = 0;
    last_y_tracking_offset = 0;
    positionX = 0;                          
    positionY = 0;
    initHeading = 90;               // gets overwritten when initTracker is called so potentially redundant
    currentHeading = initHeading;   // ^ see above comment
    scale_factor_heading = 1.0;
    lastHeading = initHeading;
    imuRotation = 0;

}

/*pros::Task initTask(Odom odometer) {
        pros::Task odom_task(Odom::updatePosition);
        return odom_task;
}*/

double Odom::toMeters(double value, double wheelRadius) {   // Accepts a value (in ticks) and returns the corresponding amount of meters moved
    return ((value / TICKS_PER_ROTATION) * 2 * M_PI * wheelRadius);
    // (value / TICKS_PER_ROTATION) is the number of total revolutions
}

void Odom::initTracker(double initial_x, double initial_y, double initial_heading) {    // initializes the tracking variables so they can begin to be updated
    currentTransverseValue = toMeters(horizontal_track.get_value()*4.0, transverseWheelRad);
    currentRadialValue = toMeters(vertical_track.get_value(), radialWheelRad);
    
    positionX = initial_x;
    positionY = initial_y;
    initHeading = initial_heading;
    currentHeading = initHeading;
    imu.set_heading(initHeading);
    imu.set_rotation(0);
    last_x_tracking_offset = RADIAL_WHEEL_X_OFFSET * cos(initHeading * M_PI / 180.0);
    last_y_tracking_offset = RADIAL_WHEEL_X_OFFSET * sin(initHeading * M_PI / 180.0);
}

double Odom::headingCorrection (double currentRotation) {
    double correctedHeading = fmod((currentRotation*scale_factor_heading), 360.0) + initHeading;

    if (correctedHeading > 360) {
        correctedHeading = fmod(correctedHeading, 360);
    }

    if (correctedHeading < 0) {
        correctedHeading = 360 + correctedHeading;
    }

    return correctedHeading;
}

void Odom::updatePosition() {       // updatePosition does all the math with the heading and the sensor values to update the actual position coordinate
    //imu.set_rotation(0);
    //while (true) {

        // PINK ROBOT:
        currentTransverseValue = toMeters(horizontal_track.get_value(), transverseWheelRad); //*4.0
        currentRadialValue = toMeters(vertical_track.get_value(), radialWheelRad); // *1.0
        pros::lcd::print(4,"Radraw: %lf",vertical_track.get_value());
        pros::lcd::print(5,"Transraw: %lf",horizontal_track.get_value());

        currentHeading = headingCorrection(imu.get_rotation());

        // std::cout << "Current Heading: " << currentHeading << std::endl;

        double cosine = cos(currentHeading * M_PI / 180.0);
        double sine = sin(currentHeading* M_PI / 180.0);

        double radialDeltaY = (currentRadialValue - lastRadialValue) * cosine;
        double transverseDeltaY = -(currentTransverseValue - lastTransverseValue) * sine; // note the - sign
        double deltaY = radialDeltaY + transverseDeltaY;

        double radialDeltaX = (currentRadialValue - lastRadialValue) * sine;
        double transverseDeltaX = (currentTransverseValue - lastTransverseValue) * cosine;
        double deltaX = radialDeltaX + transverseDeltaX;

        // pros::lcd::set_text(2, "Delta X: " + std::to_string(deltaX));
        // pros::lcd::set_text(3, "Delta Y: " + std::to_string(deltaY));

        lastRadialValue = currentRadialValue;
        lastTransverseValue = currentTransverseValue;

        // pros::lcd::set_text(2, "Position X: " + std::to_string(positionX));
        // pros::lcd::set_text(3, "Position Y: " + std::to_string(positionY));

        x_tracking_offset = TRANSVERSE_WHEEL_Y_OFFSET * sine;
        y_tracking_offset = TRANSVERSE_WHEEL_Y_OFFSET * cosine;

        // when pure rotating (x_tracking_offset - last_x_tracking_offset) should = deltaX

        positionX += isnan(deltaX) ? 0 : deltaX;
        positionY += isnan(deltaY) ? 0 : deltaY;
        positionX -= isnan(x_tracking_offset - last_x_tracking_offset) ? 0 : (x_tracking_offset - last_x_tracking_offset);
        positionY += isnan(y_tracking_offset - last_y_tracking_offset) ? 0 : (y_tracking_offset - last_y_tracking_offset);

        last_x_tracking_offset = x_tracking_offset;
        last_y_tracking_offset = y_tracking_offset;

        // pros::lcd::set_text(5, "Position X: " + std::to_string(positionX));
        // pros::lcd::set_text(6, "Position Y: " + std::to_string(positionY));
        // pros::lcd::set_text(2, "Position X: " + std::to_string(horizontal_track.get_value()/1.0));
        // pros::lcd::set_text(3, "Position Y: " + std::to_string(vertical_track.get_value()/4.0));
        // pros::lcd::set_text(7, "Heading: " + std::to_string(currentHeading));

        // pros::lcd::set_text(6, "Transverse Val: " + std::to_string(currentTransverseValue));
        // pros::lcd::set_text(4, "Radial Val: " + std::to_string(currentRadialValue));
        
        // std::cout << "cur heading: " << currentHeading << std::endl;
        // std::cout << "x: " << positionX << std::endl;
        // std::cout << "y: " << positionY << std::endl;
        // std::cout << "ht_get_value: " << horizontal_track.get_value() << std::endl;
        // std::cout << "vt_get_value: " << vertical_track.get_value() << std::endl;

    //    pros::delay(30);
    //} 

    // BELOW IS THE CORRECTED-OFFSET TRACKING

    /*lastHeading = currentHeading;                   // stores previous movement values before updating them
    lastTransverseValue = currentTransverseValue;
    lastRadialValue = currentRadialValue;

    imuRotation = imu.get_rotation();
    currentHeading = imu.get_heading();             // updates current values for how far each wheel turned and overall robot rotation
    currentTransverseValue = toMeters(horizontal_track.get_value(), transverseWheelRad);
    currentRadialValue = toMeters(vertical_track.get_value(), radialWheelRad);
    avgHeading = (currentHeading + lastHeading)/2;

    currentTransverseValue -= (sin(TRANSVERSE_THETA) * imuRotation);    // corrects current values to account for the fact that
    currentRadialValue -= (sin(RADIAL_THETA) * imuRotation);            // rotating the robot turns the tracking wheels too
    // at this point, imuRotation stores rotation since last cycle, currentHeading stores the overall current heading, and the wheel values store how far
    // each wheel has moved linearly during that time (how much they moved after we accounted for robot rotation)

    double deltaX = (cos(avgHeading) * currentTransverseValue) + (sin(avgHeading) * currentRadialValue);
    double deltaY = (sin(avgHeading) * currentTransverseValue) + (cos(avgHeading) * currentRadialValue);
    positionX += isnan(deltaX) ? 0 : deltaX; // updates position values
    positionY += isnan(deltaY) ? 0 : deltaY;*/
}

double Odom::getX() { return positionX; }
double Odom::getY() { return positionY; }
double Odom::getHeading() { return currentHeading; }
double Odom::getTransverseValue() { return currentTransverseValue; }
double Odom::getRadialValue() { return currentRadialValue; }

void opcontrol() {
    
    /*Motor front_left(8);
    front_left.set_reversed(true);
    Motor front_right(9);
    Motor back_left(7);
    back_left.set_reversed(true);
    Motor back_right(10);*/   

    imu.reset();
    delay(5000);

    while(true) {
        
        int forward = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y); // sets forward to left analog's up/down input
        int steer = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);  // sets steer to right analog's left/right input

        LeftDrive.move(forward+steer);
        RightDrive.move(forward-steer);

        odometer.updatePosition();
        pros::lcd::print(1,"X: %lf",odometer.getX());
        pros::lcd::print(2,"Y: %lf",odometer.getY());
        pros::lcd::print(3,"Heading: %lf",odometer.getHeading());
        pros::lcd::print(6,"1 meter: %lf",odometer.toMeters(2302.94, 0.0248));
        delay(30);
    }
}