#ifndef POINT_3D_H
#define POINT_3D_H
#define SQR(X) ((X)*(X))

#include <math.h>
#include "../util/NTuple.h"
namespace geom
{
	class Vector3D;
    /** Class that handles data and operations related to points in a 3D space */
    class Point3D
    {
        public:
            union{
                struct
                {
                    float x,y,z;
                };
                float v[3];
            };

            /** Computes the distance between this point and another
              @param p The point to calculate the distance to
              \return Distance from this point to p */
            inline float distance (const Point3D &p) const{return sqrtf(distanceSquared(p));}

            /** Computes the square of the distance to the point p. This is
              here only for performance reasons (distance requires an sqrt op,
              which can be too much of an overhead for cpu-intensive ops
              @param p Point to calculate the square of the distance
              \return Square of the distance to p */
            inline float distanceSquared(const Point3D &p) const {return ( SQR(x-p.x) + SQR(y-p.y) + SQR(z-p.z) );}
            /** Default constructor. Creates a Point3D with coordinates (0,0,0)*/
            Point3D();
            /** Constructor with specified coordinates */
            Point3D(float px, float py, float pz);
            /** Constructor with specified coordinates given as a 3 floats array
              @param pv Memory address of an array with 3 floats */
            Point3D(const float * pv);
            /** Calculates a new point adding a vector to this one
              @param v Vector to add to this point
              \return A new point created displacing this one by the vector v */
            Point3D operator+(const Vector3D &v) const;

			/** Gets the vector from this point to the point p*/
            Vector3D operator-(const Point3D& p) const;


            /** Checks if this point is equal to the given one
              @param p Point to compare to
              \note There is a small threshold to check for equality, not
              bigger than 1e-06, so slightly different points can be considered
              equal. Note that as Point3D uses floating point values internally,
              this value cannot be trusted for sufficiently big values. For example, 
              these two pieces of code return different values:
                \code 
                float a = 1000.0;
                float b = 1000.0+1e-05;
                return (a == b) // this returns true
                \endcode
                \code
                float a = 100.0;
                float b = 100.0+1e-05;
                return (a == b) // this returns false
                \endcode 
             */
			bool operator==(const Point3D & p) const
			{ return distanceSquared(p) < SQR(1e-07);}
            
            /** The reverse operation of '=='. Check operator== doc for details */
            bool operator!=(const Point3D & p) const {return !((*this)==p);}

			/** Assigns a point value to this one */
			Point3D & operator=(const Point3D &p){x=p.x;y=p.y;z=p.z;return *this;}
			/** Assigns a point value to this one, from a Triplet<float> */
			Point3D & operator=(const util::Triplet<float> &p){x=p[0];y=p[1];z=p[2]; return *this;}
    };
}

#endif
