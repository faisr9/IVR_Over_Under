#include "common_code/Geometry/angle2d.h"
#include <cmath>
#include <string>

#define PI 3.14159265358979

Angle2d::~Angle2d()
{
}

Angle2d::Angle2d(double radians)
{
    this->_radians = radians;
    this->_cos_value = cos(radians);
    this->_sin_value = sin(radians);

}

Angle2d::Angle2d()
{
    this->_radians = 0;
    this->_cos_value = 1;
    this->_sin_value = 0;
}

Angle2d Angle2d::createFromDegrees(double degrees)
{
    return Angle2d(degrees/180.0*PI);
}

Angle2d Angle2d::createFromRotations(double rotations)
{
    return Angle2d(rotations*2*PI);
}

Angle2d::Angle2d(double x, double y)
{
    double magnitude = sqrt(x*x+y*y);

    //to prevent chaos near 0
    if(magnitude > 1e-6){
        this->_sin_value = y / magnitude;
        this->_cos_value = x / magnitude;
    }else{
        this->_sin_value = 0;
        this->_cos_value = 1;
    }
    this->_radians = atan2(_sin_value, _cos_value);
}

Angle2d Angle2d::plus(Angle2d other)
{
    return Angle2d(this->_radians + other.getRadians());
}

Angle2d Angle2d::inverse()
{
    return Angle2d(this->_radians*-1);
}

Angle2d Angle2d::minus(Angle2d other)
{
    return this->plus(other.inverse());
}

Angle2d Angle2d::times(double scalar)
{
    return Angle2d(scalar * this->_radians);
}

double Angle2d::getRadians()
{
    return this->_radians;
}

double Angle2d::getDegrees()
{
    return this->_radians / PI * 180;
}

double Angle2d::getRotations()
{
    return this->_radians / 2 / PI;
}

double Angle2d::getCos()
{
    return this->_cos_value;
}

double Angle2d::getSin()
{
    return this->_sin_value;
}

std::string Angle2d::toString()
{
    return std::string("Angle2d with " + std::to_string(_radians) + " radians");
}

bool Angle2d::equalsTo(Angle2d other)
{
    return hypot(other._cos_value - this->_cos_value, other._sin_value - this->_sin_value) < 1e-9;
}
