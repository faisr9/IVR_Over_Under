#include "common_code/odom.h"

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

Odom::Odom(pros::IMU &theImu, Generic_Rotation* transWheel, Generic_Rotation* radWheel): imu(theImu), transverseWheel(transWheel), radialWheel(radWheel) {
    positionX = 0;                          
    positionY = 0;
}

Odom::~Odom() {
    if (transverseWheel != nullptr) {
        delete transverseWheel;
        transverseWheel = nullptr;
    } 
    if (radialWheel != nullptr) {
        delete radialWheel;
        radialWheel = nullptr;
    }
}

// initializes the tracking variables so they can begin to be updated
void Odom::initTracker(double initial_x, double initial_y, double initial_heading) {
    if (transverseWheel) transverseWheel->initialize_sensor();
    radialWheel->initialize_sensor();

    positionX = initial_x;
    positionY = initial_y;
    initHeading = initial_heading;
    imu.set_heading(initHeading);
    last_heading = initHeading;
    RADIAL_WHEEL_TRANS_OFFSET = (radialWheel) ? radialWheel->get_rot_axis_offset() : 0.0;
    TRANSVERSE_WHEEL_RAD_OFFSET = (transverseWheel) ? transverseWheel->get_rot_axis_offset() : 0.0;
}

double Odom::headingCorrection (double currentRotation) {
    double correctedHeading = fmod(currentRotation, 360.0) + initHeading;

    if (correctedHeading > 360) {
        correctedHeading = fmod(correctedHeading, 360);
    }

    if (correctedHeading < 0) {
        correctedHeading = 360 + correctedHeading;
    }

    return correctedHeading;
}

// updatePosition does all the math with the heading and the sensor values to update the actual position coordinate
void Odom::updatePosition() {       
    double deltaTransverse = (transverseWheel) ? (*transverseWheel).get_meters_travelled() : 0;
    double deltaRadial = (*radialWheel).get_meters_travelled();

    double currentHeading = headingCorrection(imu.get_rotation());

    double cosine = cos(currentHeading * M_PI / 180.0);
    double sine = sin(currentHeading* M_PI / 180.0);

    double radialDeltaY = (deltaRadial) * cosine;
    double transverseDeltaY = -(deltaTransverse) * sine; // note the - sign
    double deltaY = radialDeltaY + transverseDeltaY;

    double radialDeltaX = (deltaRadial) * sine;
    double transverseDeltaX = (deltaTransverse) * cosine;
    double deltaX = radialDeltaX + transverseDeltaX;

    positionX += isnan(deltaX) ? 0 : deltaX;
    positionY += isnan(deltaY) ? 0 : deltaY;


    // Rotation correction code for odom wheels off its axis of rotation such that the wheel will
    // rotate when the robot spins on a point (which would cause error in the robot's position awareness,
    // if not for this code).
    double delta_theta = currentHeading - last_heading;

    // Handle when the angle jumps between 0 and 360. This code will fail if the robot crosses the 0/360 angle
    // and turns > 90 degrees between a single cycle, but that shouldn't happen
    if (delta_theta > 270) {
        delta_theta -= 360;
    } else if (delta_theta < -270) {
        delta_theta += 360;
    }

    // Use the arc length of a circle
    double transverse_circumference = 2 * M_PI * TRANSVERSE_WHEEL_RAD_OFFSET * (delta_theta / 360.0);
    double radial_circumference = 2 * M_PI * RADIAL_WHEEL_TRANS_OFFSET * (delta_theta / 360.0);

    double rot_delta_Y = transverse_circumference * sine + radial_circumference * cosine;
    double rot_delta_X = -transverse_circumference * cosine + radial_circumference * sine;

    positionX += isnan(rot_delta_X) ? 0 : rot_delta_X;
    positionY += isnan(rot_delta_Y) ? 0 : rot_delta_Y;

    last_heading = currentHeading;

    // if (transverseWheel) {
    //     pros::lcd::set_text(5, "Transverse Raw: " + std::to_string(transverseWheel->get_raw_data()));
    // }
    // if (radialWheel) {
    //     pros::lcd::set_text(6, "Radial Raw: " + std::to_string(radialWheel->get_raw_data()));
    // }
    pros::lcd::set_text(4, "IMU degrees: " + std::to_string(getHeading()));
    pros::lcd::set_text(2, "Position X: " + std::to_string(positionX));
    pros::lcd::set_text(3, "Position Y: " + std::to_string(positionY));
}

double Odom::getX() { return positionX; }
double Odom::getY() { return positionY; }
double Odom::getHeading() { return imu.get_heading(); }
double Odom::getTransverseValue() { 
    return (transverseWheel) ? transverseWheel->get_raw_data() : 0;
}
double Odom::getRadialValue() { return radialWheel->get_raw_data(); }
