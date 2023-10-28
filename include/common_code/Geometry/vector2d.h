#pragma once
#include "angle2d.h"
#include <string>

class Vector2d {
    private:
        double _x, _y;

    public:

        ~Vector2d();

        Vector2d();

        /**
         * Create an 2d vector with rectangular/Cartesian coordinates.
         * 
         * @param x the x coordinate of the vector.
         * @param y the y coordinate of the vector.
        */
        Vector2d(double x, double y);


        /**
         * Create an 2d vector with polar coordinates.
         * 
         * @param r the magnitude of the vector.
         * @param theta the angle/direction of the vector.
        */
        Vector2d(double r, Angle2d theta);


        /**
         * @return The x coordinate of the vector.
        */
        double getX();

        /**
         * @return The y coordinate of the vector.
        */
        double getY();


        /**
         * @return The magnitude (r) of the vector.
        */
        double getMagnitude();


        /** 
         * @return The angle(theta) of the vector as a Angle2d instance.
        */
        Angle2d getDirection();


        /**
         * @return A new instance of Vector2d which is this current vector rotated by the angle represented by "other".
         * 
         * For example, the vector <1,0> rotated by 90 degrees will be <0,1>;
         * 
         * the vector <1,0> rotated by 45 degrees will be <(sqrt2)/2, (sqrt2)/2>
         * 
        */
        Vector2d rotateBy(Angle2d other);

        /**
         * @returns This vector plus another vector.
         * For example, <2,1> plus <1,0> will be <3,1>.
         * 
        */
        Vector2d plus(Vector2d other);


        /**
         * @returns This vector minus another vector.
         * For example, <2,1> minus <1,0> will be <1,1>.
        */
        Vector2d minus(Vector2d other);


        /**
         * @returns The inverse of this vector.
         * For example, the inverse of <1,1> is <-1,-1>
        */
        Vector2d inverse();

        /**
         * @return The dot product of this and the other vector.
         * 
        */
        double dot(Vector2d other);


        /**
         * @param scalar the scalar that you want to times the vector by.
         * @returns The scalar product of scalar times the vector.
        */
        Vector2d times(double scalar);

        /**
         * @returns A string format of the vector
        */
        std::string toString();

        /**
         * @returns Whether the two vectors are equal.
        */
        bool equalsTo(Vector2d other);
};