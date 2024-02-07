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

Odom::Odom(pros::IMU &theImu, Generic_Rotation* transverseWheel, Generic_Rotation* radialWheel): imu(theImu) {

    //transverseWheelRad = 1.96 * 0.0254 / 2; // transverse wheel tracks left to right movements
    //radialWheelRad = 1.96 * 0.0254 / 2;     // radial wheel tracks forward and backward movements (has nothing to do with radians)
    lastTransverseValue = 0;                // these variables track what the last value was in order to determine how far the robot has moved
    lastRadialValue = 0;
    last_x_tracking_offset = 0;
    last_y_tracking_offset = 0;
    positionX = 0;                          
    positionY = 0;
    scale_factor_heading = 1.0;
    imuRotation = 0;
    //vertical_track.reset();
    //horizontal_track.reset();

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


/*pros::Task initTask(Odom odometer) {
        pros::Task odom_task(Odom::updatePosition);
        return odom_task;
}*/

// initializes the tracking variables so they can begin to be updated
void Odom::initTracker(double initial_x, double initial_y, double initial_heading) {  
    currentTransverseValue = (*transverseWheel).get_meters_travelled();
    currentRadialValue = (*radialWheel).get_meters_travelled();
    positionX = initial_x;
    positionY = initial_y;
    initHeading = initial_heading;
    currentHeading = initHeading;
    imu.set_heading(initHeading);
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
    // //imu.set_rotation(0);
    // //while (true) {

    // PINK ROBOT:
    // currentTransverseValue = toMeters(horizontal_track.get_value(), transverseWheelRad);
    // currentRadialValue = toMeters(vertical_track.get_value(), radialWheelRad);

    // currentHeading = headingCorrection(imu.get_rotation());

    // // std::cout << "Current Heading: " << currentHeading << std::endl;

    // double cosine = cos(currentHeading * M_PI / 180.0);
    // double sine = sin(currentHeading* M_PI / 180.0);

    // double radialDeltaY = (currentRadialValue - lastRadialValue) * cosine;
    // double transverseDeltaY = -(currentTransverseValue - lastTransverseValue) * sine; // note the - sign
    // double deltaY = radialDeltaY + transverseDeltaY;

    // double radialDeltaX = (currentRadialValue - lastRadialValue) * sine;
    // double transverseDeltaX = (currentTransverseValue - lastTransverseValue) * cosine;
    // double deltaX = radialDeltaX + transverseDeltaX;

    // // pros::lcd::set_text(2, "Delta X: " + std::to_string(deltaX));
    // // pros::lcd::set_text(3, "Delta Y: " + std::to_string(deltaY));

    // lastRadialValue = currentRadialValue;
    // lastTransverseValue = currentTransverseValue;

    // // pros::lcd::set_text(2, "Position X: " + std::to_string(positionX));
    // // pros::lcd::set_text(3, "Position Y: " + std::to_string(positionY));

    // x_tracking_offset = TRANSVERSE_WHEEL_Y_OFFSET * sine;
    // y_tracking_offset = TRANSVERSE_WHEEL_Y_OFFSET * cosine;

    // // when pure rotating (x_tracking_offset - last_x_tracking_offset) should = deltaX

    // positionX += isnan(deltaX) ? 0 : deltaX;
    // positionY += isnan(deltaY) ? 0 : deltaY;
    // positionX -= isnan(x_tracking_offset - last_x_tracking_offset) ? 0 : (x_tracking_offset - last_x_tracking_offset);
    // positionY += isnan(y_tracking_offset - last_y_tracking_offset) ? 0 : (y_tracking_offset - last_y_tracking_offset);

    // last_x_tracking_offset = x_tracking_offset;
    // last_y_tracking_offset = y_tracking_offset;

    // pros::lcd::set_text(5, "Position X: " + std::to_string(positionX));
    // pros::lcd::set_text(6, "Position Y: " + std::to_string(positionY));
    // pros::lcd::set_text(2, "Horizontal Track: " + std::to_string(horizontal_track.get_value()));
    // pros::lcd::set_text(3, "Vertical Track: " + std::to_string(vertical_track.get_value()));
    // pros::lcd::set_text(7, std::to_string(horizontal_track.get_config()));
    // std::cout << "Heading: " + std::to_string(currentHeading) << std::endl;
    // pros::lcd::set_text(4, "Heading: " + std::to_string(currentHeading));

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

    lastHeading = currentHeading;                   // stores previous movement values before updating them
    lastTransverseValue = currentTransverseValue;
    lastRadialValue = currentRadialValue;

    imuRotation = imu.get_rotation();
    currentHeading = imu.get_heading();             // updates current values for how far each wheel turned and overall robot rotation
    currentTransverseValue = (*transverseWheel).get_meters_travelled();
    currentRadialValue = (*radialWheel).get_meters_travelled();
    avgHeading = (currentHeading + lastHeading)/2;

    currentTransverseValue -= (sin(TRANSVERSE_THETA) * imuRotation);    // corrects current values to account for the fact that
    currentRadialValue -= (sin(RADIAL_THETA) * imuRotation);            // rotating the robot turns the tracking wheels too
    // at this point, imuRotation stores rotation since last cycle, currentHeading stores the overall current heading, and the wheel values store how far
    // each wheel has moved linearly during that time (how much they moved after we accounted for robot rotation)

    double deltaX = (cos(avgHeading) * currentTransverseValue) + (sin(avgHeading) * currentRadialValue);
    double deltaY = (sin(avgHeading) * currentTransverseValue) + (cos(avgHeading) * currentRadialValue);
    positionX += isnan(deltaX) ? 0 : deltaX; // updates position values
    positionY += isnan(deltaY) ? 0 : deltaY;
}

double Odom::getX() { return positionX; }
double Odom::getY() { return positionY; }
double Odom::getHeading() { return currentHeading; }
double Odom::getTransverseValue() { return currentTransverseValue; }
double Odom::getRadialValue() { return currentRadialValue; }
