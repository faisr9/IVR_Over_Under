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
    last_x_tracking_offset = 0;
    last_y_tracking_offset = 0;
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
    last_x_tracking_offset = RADIAL_WHEEL_TRANS_OFFSET * cos(initHeading * M_PI / 180.0);
    last_y_tracking_offset = RADIAL_WHEEL_TRANS_OFFSET * sin(initHeading * M_PI / 180.0);
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



    // when pure rotating (x_tracking_offset - last_x_tracking_offset) should = deltaX
    // offset code

    // KEVIN CODE
    // double delta_theta = currentHeading - last_heading;
    // double rot_delta_trans = 2 * sin(delta_theta / 2.0) * ((deltaTransverse / delta_theta) + TRANSVERSE_WHEEL_RAD_OFFSET);
    // double rot_delta_radial = 2 * sin(delta_theta / 2.0) * ((deltaRadial / delta_theta) - RADIAL_WHEEL_TRANS_OFFSET);

    // double deltaY = rot_delta_radial * sine + rot_delta_trans * cosine;
    // double deltaX = rot_delta_radial * cosine - rot_delta_trans * sine;
    // does not seem to work? Idk but there is significant change in value when turning

    // positionX += isnan(deltaX) ? 0 : deltaX;
    // positionY += isnan(deltaY) ? 0 : deltaY;

    // note that this does not account for the rotation in the wheels it's purely based on the IMU but fundamentally
    // that should still be fine

    // x_tracking_offset = TRANSVERSE_WHEEL_RAD_OFFSET * sine;
    // y_tracking_offset = TRANSVERSE_WHEEL_RAD_OFFSET * cosine;

    // positionX -= isnan(x_tracking_offset - last_x_tracking_offset) ? 0 : (x_tracking_offset - last_x_tracking_offset);
    // positionY += isnan(y_tracking_offset - last_y_tracking_offset) ? 0 : (y_tracking_offset - last_y_tracking_offset);

    // last_x_tracking_offset = x_tracking_offset;
    // last_y_tracking_offset = y_tracking_offset;

    
    // TAKE 3 CODE
    // transverse only for now
    double delta_theta = currentHeading - last_heading; // 10 to 350 (=340), really -20
    if (delta_theta > 340) {
        delta_theta -= 360;
    }

    if (delta_theta < -340) {
        delta_theta += 360;
    }

    double transverse_circumference = 2 * M_PI * TRANSVERSE_WHEEL_RAD_OFFSET * (delta_theta / 360.0);
    double rot_delta_Y = transverse_circumference * sine;
    double rot_delta_X = transverse_circumference * cosine;

    positionX += isnan(rot_delta_X) ? 0 : rot_delta_X;
    positionY += isnan(rot_delta_Y) ? 0 : rot_delta_Y;

    // CONCLUSION RIGHT NOW IS THAT IT DOES APPEAR TO BE WORKING RELATIVELY WELL
    // BUT THERE MIGHT BE A SLIGHT SCALING ISSUE 

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
    
    pros::lcd::set_text(5, "Delta Theta: " + std::to_string(currentHeading));
    pros::lcd::set_text(6, "transDeltY: " + std::to_string((int) (transverseDeltaY * 100000)));
    pros::lcd::set_text(7, "rot_delta_y: " + std::to_string((int) (rot_delta_Y * 100000)));
    pros::lcd::set_text(1, "rot_trans_diff: " + std::to_string((int) (100000 * (rot_delta_Y - transverseDeltaY))));




    // if (transverseWheel) {
    //     pros::lcd::set_text(2, "Horizontal Track: " + std::to_string(transverseWheel->get_raw_data()));
    // }
    // pros::lcd::set_text(3, "Vertical Track: " + std::to_string(radialWheel->get_raw_data()));
}

double Odom::getX() { return positionX; }
double Odom::getY() { return positionY; }
double Odom::getHeading() { return imu.get_heading(); }
double Odom::getTransverseValue() { 
    return (transverseWheel) ? transverseWheel->get_raw_data() : 0;
}
double Odom::getRadialValue() { return radialWheel->get_raw_data(); }
