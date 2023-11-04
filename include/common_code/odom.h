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

    double toMeters(double value, double wheelRadius);
    void initTracker(double initial_x, double initial_y, double initial_heading);
    double headingCorrection(double currentRotation);
    void updatePosition();
    double getX();
    double getY();

    static pros::Task initTask(Odom odometer) {
        pros::Task odom_task(updatePosition);
        return odom_task;
    }
    Odom();

    private:
    double transverseWheelRad;
    double radialWheelRad;
    double lastTransverseValue;
    double lastRadialValue;
    // double last_x_tracking_offset;
    // double last_y_tracking_offset;
    double positionX;
    double positionY;
    const double TICKS_PER_ROTATION = 19600.0;
    const double FEET_TO_METERS = 0.304;
    const double ADJUSTMENT_MULTIPLIER = 1.015;
    const double RADIAL_TRACKING_WHEEL_OFFSET = 0;
    const double TRANSVERSE_TRACKING_WHEEL_OFFSET = 0.1;

    double initHeading;
    double currentHeading;
    double scale_factor_heading;

    pros::ADIEncoder vertical_track;
    pros::ADIEncoder horizontal_track;
    pros::IMU imu;
};