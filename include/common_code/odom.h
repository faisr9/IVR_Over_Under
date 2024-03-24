#pragma once
#include "main.h"
#include "common_code/generic_rotation_vex_rot.h"

class Odom {
    public:
    Odom(pros::IMU &theImu, Generic_Rotation* transverseWheel, Generic_Rotation* radialWheel);
    ~Odom();

    void initTracker(double initial_x, double initial_y, double initial_heading);
    double headingCorrection(double currentRotation);
    void updatePosition();
    double getX();
    double getY();
    double getHeading();
    double getTransverseValue();
    double getRadialValue();

    private:
    // wheel radii are stored in the GenericRotation sensor
    double initHeading;
    double x_tracking_offset;
    double y_tracking_offset;
    double last_x_tracking_offset;
    double last_y_tracking_offset;
    double positionX = 0.0;
    double positionY = 0.0;
    double last_heading = 0.0;
    const double TICKS_PER_ROTATION = 2000.0; // 19600.
    const double FEET_TO_METERS = 0.304;

    const double RADIAL_WHEEL_TRANS_OFFSET = 0.013; // meters. Slightly off but this is just a test robot now. Positive because it is on the "right" side of the robot
    const double TRANSVERSE_WHEEL_RAD_OFFSET = -0.168; // meters. Negative because it is on the "back" of the robot
    // Be careful when defining the offset from the axis of rotation. The axis of rotation might not be what you think it is. 
    // To figure out the axis of rotation, do it per axis, and find the line through the robot (parallel to the axis) 
    // that does not move perpendicular to itself when the robot rotates 180 degrees
    // tl;dr the axis of rotation is not always halfway between the wheels or such. Be careful

    Generic_Rotation* transverseWheel = nullptr;
    Generic_Rotation* radialWheel = nullptr;
    pros::IMU& imu;
};