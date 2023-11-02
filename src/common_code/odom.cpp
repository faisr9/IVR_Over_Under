#include "common_code/odom.h"
#include <cmath>
#include <vector>
#include <string>
#include "pros/adi.hpp"
#include "main.h"
#include "pros/imu.h"

/*
Authors: Reid Faistl, Ethan Lenning
Originally adapted from 2022-23 club repository
https://github.com/upai02/IVR-Spin-Up/blob/master/15-inch/spin-up-15/src/misc/PositionTracker.cpp

The Odom class acts as an odometer for the robot which calls it. A robot should instantiate its own Odom object. The goal of the odometer is to track the
robot's position at any given time, by tracking how far the tracking wheels move and what angle the robot is facing when the wheels are moved.

The tracking wheels cannot tell if the robot turns, so the heading (angle) needs to be updated during the process in order to calculate the actual signs
and magnitudes of the x and y movements. Thus, if it moves right 1m and then turns 180 degrees and moves "right" 1m (in the robot's frame of reference), 
currentTransverseValue will be the equivalent of 2m even though from an outside perspective it moved right 1m and then left 1m and is back at its origin.
This class has accounted for that, so that other code can assume the position of the robot is reliable.
*/

Odom::Odom(pros::IMU theImu): imu(theImu), vertical_track(3,4,false), horizontal_track(1,2,true) {

    transverseWheelRad = 1.96 * 0.0254 / 2; // transverse wheel tracks left to right movements
    radialWheelRad = 1.96 * 0.0254 / 2;     // radial wheel tracks forward and backward movements (has nothing to do with radians)
    lastTransverseValue = 0;                // these variables track what the last value was in order to determine how far the robot has moved
    lastRadialValue = 0;
    // last_x_tracking_offset = 0;
    // last_y_tracking_offset = 0;
    positionX = 0;                          
    positionY = 0;
    initHeading = 90;               // gets overwritten when initTracker is called so potentially redundant
    currentHeading = initHeading;   // ^ see above comment
    scale_factor_heading = 1.0;

    pros::Task odom_task();
}

double Odom::toMeters(double value, double wheelRadius) {   // Accepts a value (in ticks) and returns the corresponding amount of meters moved
    return ((value / TICKS_PER_ROTATION) * 2 * M_PI * wheelRadius);
    // (value / TICKS_PER_ROTATION) is the number of total revolutions
}

void Odom::initTracker(double initial_x, double initial_y, double initial_heading) {    // initializes the tracking variables so they can begin to be updated
    double currentTransverseValue = toMeters(horizontal_track.get_value()*4.0, transverseWheelRad);
    double currentRadialValue = toMeters(vertical_track.get_value(), radialWheelRad);
    
    positionX = initial_x;
    positionY = initial_y;
    initHeading = initial_heading;
    currentHeading = initHeading;
    imu.set_heading(initHeading);
    // last_x_tracking_offset = RADIAL_TRACKING_WHEEL_OFFSET * cos(initHeading * M_PI / 180.0);
    // last_y_tracking_offset = RADIAL_TRACKING_WHEEL_OFFSET * sin(initHeading * M_PI / 180.0);
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
    imu.set_rotation(0);
    while (true) {
        // TEAL ROBOT:
        // double currentTransverseValue = toMeters(horizontal_track.get_value(), transverseWheelRad); //*1.0
        // double currentRadialValue = toMeters(vertical_track.get_value()*4.0, radialWheelRad); // *4.0

        // PINK ROBOT:
        double currentTransverseValue = toMeters(horizontal_track.get_value()*4.0, transverseWheelRad); //*4.0
        double currentRadialValue = toMeters(vertical_track.get_value(), radialWheelRad); // *1.0

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

        // double x_tracking_offset = TRANSVERSE_TRACKING_WHEEL_OFFSET * sine;
        // double y_tracking_offset = TRANSVERSE_TRACKING_WHEEL_OFFSET * cosine;

        // when pure rotating (x_tracking_offset - last_x_tracking_offset) should = deltaX

        positionX += isnan(deltaX) ? 0 : deltaX;
        positionY += isnan(deltaY) ? 0 : deltaY;
        // positionX += deltaX - (x_tracking_offset - last_x_tracking_offset);
        // positionY += deltaY + (y_tracking_offset - last_y_tracking_offset);

        // last_x_tracking_offset = x_tracking_offset;
        // last_y_tracking_offset = y_tracking_offset;

        pros::lcd::set_text(5, "Position X: " + std::to_string(positionX));
        pros::lcd::set_text(6, "Position Y: " + std::to_string(positionY));
        // pros::lcd::set_text(2, "Position X: " + std::to_string(horizontal_track.get_value()/1.0));
        // pros::lcd::set_text(3, "Position Y: " + std::to_string(vertical_track.get_value()/4.0));
        pros::lcd::set_text(7, "Heading: " + std::to_string(currentHeading));

        // pros::lcd::set_text(6, "Transverse Val: " + std::to_string(currentTransverseValue));
        // pros::lcd::set_text(4, "Radial Val: " + std::to_string(currentRadialValue));
        
        // std::cout << "cur heading: " << currentHeading << std::endl;
        // std::cout << "x: " << positionX << std::endl;
        // std::cout << "y: " << positionY << std::endl;
        // std::cout << "ht_get_value: " << horizontal_track.get_value() << std::endl;
        // std::cout << "vt_get_value: " << vertical_track.get_value() << std::endl;

        pros::delay(30);
    }
}

double Odom::getX() { return positionX; }
double Odom::getY() { return positionY; }