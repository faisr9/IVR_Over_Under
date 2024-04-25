/*
 * Description: implementations for asterisk drive system
 * Path: src/common_code/asterisk-drive.cpp
 * Header: include/common_code/asterisk-drive.h
 * Last Modified: 04/16/24 by Zach Martin
 */
/**************************************
 * CAUTION: THIS FILE HAS NOT BEEN TESTED
 * USE AT YOUR OWN RISK
 * MAY CONTAIN BUGS
 * MAY MODIFY YOUR SSN
 * MAY DELTE YOUR FAMILY
 * MAY RESET THE SIMULATION
 **************************************/
#include "common_code/asterisk-drive.h"
#include "asterisk-drive.h"

asterisk_drive::asterisk_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Motor_Group &straight_right, Motor_Group &straight_left, Imu &imu) : straight_left_(straight_left), straight_right_(straight_right), x_drive(master, front_left, front_right, back_left, back_right, imu)
{
    auto gearing = front_left_->get_gearing(); // assume all motors have the same gearing

    // set max speed based on gear
    if (gearing == 0)      // 36:1
        maxspeed = 100.0;  // max rpm
    else if (gearing == 1) // 18:1
        maxspeed = 200.0;  // max rpm
    else if (gearing == 2) // 6:1
        maxspeed = 600.0;  // max rpm
    else
        maxspeed = 200.0; // default max rpm

}

// asterisk_drive::asterisk_drive(Controller &master, Motor &front_left, Motor &front_right, Motor &back_left, Motor &back_right, Motor &right_middle_1, Motor &right_middle_2, Motor &left_middle_1, Motor &left_middle_2, Imu &imu)
//     : straight_left_(Motor_Group({left_middle_1,left_middle_2})), straight_right_(Motor_Group({right_middle_1,right_middle_2})), x_drive(master, front_left, front_right, back_left, back_right, imu)
// {
//     auto gearing = front_left_->get_gearing(); // assume all motors have the same gearing

//     // set max speed based on gear
//     if (gearing == 0)      // 36:1
//         maxspeed = 100.0;  // max rpm
//     else if (gearing == 1) // 18:1
//         maxspeed = 200.0;  // max rpm
//     else if (gearing == 2) // 6:1
//         maxspeed = 600.0;  // max rpm
//     else
//         maxspeed = 200.0; // default max rpm
// }

asterisk_drive::~asterisk_drive()
{
    stop();
    // straight_left_=nullptr;
    // straight_right_=nullptr;
}

void asterisk_drive::stop()
{
    front_left_->brake();
    front_right_->brake();
    back_left_->brake();
    back_right_->brake();
    straight_left_.brake();
    straight_right_.brake();
}

void asterisk_drive::robot_centric_move(pair<double, double> movement_vector, double turn)
{
    auto speed = 0.0;
    auto dir = movement_vector.second; // direction in radians

    auto move_1 = 0.0; // first diagonal component of movement
    auto move_2 = 0.0; // second diagonal component of movement
    auto scaling = 0.0; // scale factor for movement
    if (movement_vector.first > 0.2) // consider joystick deadzone
    {
        speed = maxspeed * movement_vector.first; // normalized speed of movement times max speed
        dir -= M_PI / 4;                                 // adjust direction by 45˚ to get the diagonal components of movement
        move_1 = -1 * cos(dir);                          // opposite of cosine of direction
        move_2 = sin(dir);                               // sine of direction
        scaling = speed / max(abs(move_1), abs(move_2)); // speed divided by max of x and y
    }
    
    auto move_1_scaled = move_1 * scaling; // move speed
    auto move_2_scaled = move_2 * scaling; // move speed
    auto turn_scaled = maxspeed * turn; // turn speed
    auto priority = 0.5; // priority of movement over turning

    // if the sum of the speeds is greater than the max speed, scale them down
    if (max(abs(move_1_scaled),abs(move_2_scaled)) + abs(turn_scaled) > maxspeed) 
    {
        move_1_scaled = move_1_scaled / (max(abs(move_1_scaled),abs(move_2_scaled)) + abs(turn_scaled)) * maxspeed;
        move_2_scaled = move_2_scaled / (max(abs(move_1_scaled), abs(move_2_scaled)) + abs(turn_scaled)) * maxspeed;
        turn_scaled = turn_scaled / (max(abs(move_1_scaled), abs(move_2_scaled)) + abs(turn_scaled)) * maxspeed;
    }
    auto fl_move = move_1_scaled - turn_scaled; // fl and br use the first diagonal component
    auto fr_move = move_2_scaled - turn_scaled; // front motors subtract turn
    auto bl_move = move_2_scaled + turn_scaled; // bl and fr use the second diagonal component
    auto br_move = move_1_scaled + turn_scaled; // back motors add turn

    // move the four primary motors
    front_left_->move_velocity(fl_move);
    front_right_->move_velocity(fr_move);
    back_left_->move_velocity(bl_move);
    back_right_->move_velocity(br_move);
    /************************************************************************
     * everything above is the same as x_drive::robot_centric_move
     * below is the new code for the asterisk drive
     * move the four secondary motors (straight wheels)
     ************************************************************************/

    auto theta = M_PI / 4; // wheels are at 45˚ angle

    // take the average velocity in the forward direction
    // of the front and back wheels on each side
    auto sl_move = (fl_move-bl_move)/2; // subtracting back from front eliminates horizontal component
    auto sr_move = (fr_move-br_move)/2; // giving only the vertical component

    // scale the straight wheel speeds so they don't overpower the primary wheels
    sl_move*=1/cos(M_PI); // cos(45˚)=1/√2 
    sr_move*=1/cos(M_PI); // cos(45˚)=1/√2

    // The 5/3 is a scaling factor from the gear ratio of the physical robot
    auto gear_ratio=5.0/3.0; // 5:3 gear ratio
    sl_move*=gear_ratio; // scale left side by gear ratio
    sr_move*=gear_ratio; // scale right side by gear ratio

    // move forward/straight wheels
    straight_left_.move_velocity(sl_move); // move straight left Motor_Group
    straight_right_.move_velocity(sr_move); // move straight right Motor_Group
}

