#include "pros/llemu.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include "comp15_includeList.h"

// 15.5 length by 12.5 inches (width)
// 41.5 something. 3.6in field width - 15.5in robo length = 3.21 field width meters
// 3.21/41.5 = whatever it is to meters conversion
// old 
// 10.3
// 3.25/1.96 = 1.65, *2 = 3.31
// 3.15
// 3.17

double transverseWheelRad = 1.96 * 0.0254 / 2; // 3.25 * 0.0254 / 2;
double radialWheelRad = 1.96 * 0.0254 / 2;
double lastTransverseValue = 0;
double lastRadialValue = 0;
double last_x_tracking_offset = 0;
double last_y_tracking_offset = 0;
double positionX = 0;
double positionY = 0;
const double TICKS_PER_ROTATION = 19600.0; // 5120.0
const double FEET_TO_METERS = 0.304;
const double ADJUSTMENT_MULTIPLIER = 1.015;

double initHeading = 90;
double currentHeading = initHeading;
double scale_factor_heading = 1.0;

double toMeters(double value, double wheelRadius) {
    return ((value / TICKS_PER_ROTATION) * 2 * M_PI * wheelRadius); //*2 * FEET_TO_METERS * ADJUSTMENT_MULTIPLIER;
}

void initTracker(double initial_X, double initial_Y, double initial_heading) {
    // TEAL ROBOT:
    // double currentTransverseValue = toMeters(horizontal_track.get_value(), transverseWheelRad); // 1.0
    // double currentRadialValue = toMeters(vertical_track.get_value()*4.0, radialWheelRad); // *4.0

    // PINK ROBOT:
    double currentTransverseValue = toMeters(horizontal_track.get_value()*4.0, transverseWheelRad); //*4.0
    double currentRadialValue = toMeters(vertical_track.get_value(), radialWheelRad); // *1.0
    
    positionX = initial_X;
    positionY = initial_Y;
    initHeading = initial_heading;
    currentHeading = initHeading;
    imu.set_heading(initHeading);
    last_x_tracking_offset = RADIAL_TRACKING_WHEEL_OFFSET * cos(initHeading * M_PI / 180.0);
    last_y_tracking_offset = RADIAL_TRACKING_WHEEL_OFFSET * sin(initHeading * M_PI / 180.0);
    // pros::lcd::set_text(2, "Position X: " + std::to_string(positionX));
    // pros::lcd::set_text(3, "Position Y: " + std::to_string(positionY));
}

double headingCorrection (double currentRotation) {
    double correctedHeading = fmod((currentRotation*scale_factor_heading), 360.0) + initHeading;

    if (correctedHeading > 360) {
        correctedHeading = fmod(correctedHeading, 360);
    }

    if (correctedHeading < 0) {
        correctedHeading = 360 + correctedHeading;
    }

    return correctedHeading;
}

void updatePosition() {
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

        double x_tracking_offset = TRANSVERSE_TRACKING_WHEEL_OFFSET * sine;
        double y_tracking_offset = TRANSVERSE_TRACKING_WHEEL_OFFSET * cosine;

        // when pure rotating (x_tracking_offset - last_x_tracking_offset) should = deltaX

        positionX += isnanf(deltaX) ? 0 : deltaX;
        positionY += isnanf(deltaY) ? 0 : deltaY;
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