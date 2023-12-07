#pragma once
#include <cmath>
#include <vector>
#include <string>
#include "pros/adi.hpp"
#include "main.h"
#include "pros/imu.h"

class Odom {
    public:
    Odom(pros::IMU theImu);
    ~Odom();

    double toMeters(double value, double wheelRadius);
    void initTracker(double initial_x, double initial_y, double initial_heading);
    double headingCorrection(double currentRotation);
    void updatePosition();
    double getX();
    double getY();
    double getHeading();

    //static pros::Task initTask(Odom odometer);

    private:
    double transverseWheelRad;
    double radialWheelRad;
    double currentTransverseValue;
    double currentRadialValue;
    double lastTransverseValue;
    double lastRadialValue;
    double x_tracking_offset;
    double y_tracking_offset;
    double last_x_tracking_offset;
    double last_y_tracking_offset;
    double positionX;
    double positionY;
    const double TICKS_PER_ROTATION = 19600.0;
    const double FEET_TO_METERS = 0.304;
    const double ADJUSTMENT_MULTIPLIER = 1.015;
    const double RADIAL_WHEEL_X_OFFSET = 0.0746125;
    const double RADIAL_WHEEL_Y_OFFSET = 0.11;
    const double TRANSVERSE_WHEEL_Y_OFFSET = 0.104775;
    const double TRANSVERSE_WHEEL_X_OFFSET = 0.04;
    const double RADIAL_THETA = atan(RADIAL_WHEEL_X_OFFSET / RADIAL_WHEEL_Y_OFFSET);
    const double TRANSVERSE_THETA = atan(TRANSVERSE_WHEEL_Y_OFFSET / TRANSVERSE_WHEEL_X_OFFSET);
    const double RADIAL_WHEEL_OFFSET_HYP = hypot(RADIAL_WHEEL_X_OFFSET, RADIAL_WHEEL_Y_OFFSET);
    const double TRANSVERSE_WHEEL_OFFSET_HYP = hypot(TRANSVERSE_WHEEL_X_OFFSET, TRANSVERSE_WHEEL_Y_OFFSET);

    double initHeading;
    double currentHeading;
    double lastHeading;
    double avgHeading;
    double imuRotation;
    double scale_factor_heading;

    pros::ADIEncoder vertical_track;
    pros::ADIEncoder horizontal_track;
    pros::IMU imu;
};