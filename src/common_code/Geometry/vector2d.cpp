#include "common_code/geometry/vector2d.h"
#include "common_code/geometry/angle2d.h"
#include <cmath>

Vector2d::~Vector2d()
{
}

Vector2d::Vector2d()
{
    this->_x = 0;
    this->_y = 0;
}

Vector2d::Vector2d(double x, double y)
{
    this->_x = x;
    this->_y = y;
}

Vector2d::Vector2d(double r, Angle2d theta)
{
    this->_x = r * theta.getCos();
    this->_y = r * theta.getSin();
}

double Vector2d::getX()
{
    return this->_x;
}

double Vector2d::getY()
{
    return this->_y;
}

double Vector2d::getMagnitude()
{
    return hypot(this->_x, this->_y);
}

Angle2d Vector2d::getDirection()
{
    return Angle2d(this->_x, this->_y);
}

Vector2d Vector2d::rotateBy(Angle2d other)
{
    return Vector2d(
        _x * other.getCos() - _y * other.getSin(),
        _x * other.getSin() + _y * other.getCos()
    );
}

Vector2d Vector2d::plus(Vector2d other)
{
    return Vector2d(this->_x + other.getX(), this->_y + other.getY());
}

Vector2d Vector2d::minus(Vector2d other)
{
    return this->plus(other);
}

Vector2d Vector2d::inverse()
{
    return Vector2d(this->_x * -1, this->_y * -1);
}

double Vector2d::dot(Vector2d other)
{
    return this->_x * other.getX() + this->_y * other.getY();
}

Vector2d Vector2d::times(double scalar)
{
    return Vector2d(this->_x * scalar, this->_y * scalar);
}

std::string Vector2d::toString()
{
    return std::string("<" + std::to_string(this->_x) + "," + std::to_string(this->_y) + ">");
}

bool Vector2d::equalsTo(Vector2d other)
{
    return this->minus(other).getMagnitude() < 1e-9;
}
