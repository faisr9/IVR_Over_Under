#pragma once
#include <string>
#include <cmath>

/**
 * Create an instance that contains an angle. This angle's unit can be taken to degree, radian, or rotation.
 * Its range is also all real numebrs meaning that an angle of 361 degree is not equivalent with an angle of 1 degree.
 * However, it's possible to converted to range [0, 2PI) or [-PI, PI).
 */
class Angle2d
{
private:
    double _sin_value;
    double _cos_value;
    double _radians;

public:
    ~Angle2d();

    /**
     * Create an Angle2d instance from radians. If want to create an instance from degrees, then use the createFromDegrees() method
     *
     * @param radians the radian of the angle
     */
    Angle2d(double radians);

    /**
     * Default constructor that generate an instance with radian of 0.0
     */
    Angle2d();

    /**
     * Create an Angle2d instance whose angle is the angle from the x-axis to the vector <x,y>
     *
     * @param x the x coordinate
     * @param y the y coordinate
     *
     *
     *
     */
    Angle2d(double x, double y);

    /**
     * Create an Angle2d instance from degrees
     *
     * @param degrees the degree of the angle
     * @return the instance created
     */
    static Angle2d createFromDegrees(double degrees);

    /**
     * Create an Angle2d instance from degrees
     *
     * @param rotations the number of full rotation of the angle (1 rotation = 360 degrees)
     * @return the instance created
     */
    static Angle2d createFromRotations(double rotations);

    /**
     * returns a new instance of Angle2d with this angle plus another angle
     * @param other the other angle in the addition operation
     */
    Angle2d plus(Angle2d other);

    /**
     * returns a new instance of Angle2d with 0 minus this angle
     * e.g. 60 degrees's inverse is -60 degrees
     *
     *
     */
    Angle2d unaryMinus();

    /** returns a new instance of Angle2d with the opposite angle (+180 degrees)*
     * e.g. 30 degree's inverse is 210 degree
     *
     */
    Angle2d oppositeAngle();

    /**
     * returns a new instance of Angle2d with this angle minus another angle
     */
    Angle2d minus(Angle2d other);

    /**
     * returns a new instance of Angle2d with this angle times a scalar
     */
    Angle2d times(double scalar);

    /**
     * returns a new Angle2d instance which is this angle converting into a corresponding angle in range [0, 2PI) radians.
     */
    Angle2d getAngleIn1Rotation();

    /**
     * returns a new Angle2d instance which is this angle converting into a corresponding angle in range [-PI, PI) radians.
     */
    Angle2d getAngleIn1RotationSymmetricAbout0();

    /**
     * returns this angle in radians but in the range of [0, 2PI).
     */
    double getRadians0to2PI();

    /**
     * return this angle in radians but in the range of [-PI, PI).
     */
    double getRadiansNegPItoPI();

    /**
     * returns this angle in degrees in the range [0, 360).
     */
    double getDegrees0to360();

    /**
     * returns this angle in degrees in the range [-180, 180). 
    */
    double getDegreesN180to180();

    /**
     * returns this angle in radians
     */
    double getRadians();

    /**
     * returns this angle in degrees
     */
    double getDegrees();

    /**
     * returns this angle in the number of rotations
     */
    double getRotations();

    /** returns the cosine of this angle
     *
     */
    double getCos();

    /** returns the sine of this angle
     *
     */
    double getSin();

    /**
     * returns a string containing info of this instance
     */
    std::string toString();

    /**
     * checks if two angles are equal (or very close)
     */
    bool equalsTo(Angle2d other);
};
