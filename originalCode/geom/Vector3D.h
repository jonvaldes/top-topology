#ifndef VECTOR_3D_H
#define VECTOR_3D_H

namespace geom
{
	class Point3D;
	/** Class that represents a vector in a 3D space. This is thought of as a 
      free vector, not having a fixed position in space, just direction. If a 
      non-free vector is needed, use the Segment3D class */
    class Vector3D
    {
        public:
            union
            {
                struct
                {
                    float dx, dy,dz;
                };
                float dv[3];
            };
        
            /** Gets the vector module (length) */
            float module() const;
			
			/** Gets the square of the module for this vector (faster that the module when you don't need size, but only relation between vectors)*/
			inline float squaredModule() const {return dx*dx+dy*dy+dz*dz;}

            /** Returns a normalized vector with the same direction as this one*/
            Vector3D getNormalized() const;
            
            /** Returns a vector with the same direction as this one, but with 
              the given module */
            Vector3D getWithModule(float newModule) const;
            
            /** Returns a vector with its module divided by the given factor */
            Vector3D operator/(float v) const;
            /** Retursn a vector with its module multiplied by the given factor */
            Vector3D operator*(float v) const;

            /** Adds another vector to this one, and returns the sum vector */
            Vector3D operator+(const Vector3D &v) const;
            
            /** Adds another vector to this one*/
			Vector3D & operator+=(const Vector3D &v);
            
			/** Subs another vector from this one, and returns the resulting vector */
			inline Vector3D operator-(const Vector3D &v) const{ return Vector3D(dx-v.dx, dy-v.dy, dz-v.dz);	}

			/** Returns the reverse of this vector*/
			inline Vector3D operator-() const{return Vector3D(-dx,-dy,-dz);}

            /** Default constructor. Creates a vector with direction (0,0,0) */
            Vector3D();
            
			/** Constructor. Creates a vector with direction (x,y,z) */
            Vector3D(float x, float y, float z);
            /** Constructor. Creates a vector with direction (v[0],v[1],v[2])
              @param v Array with three floats to be used as coordinates */
            Vector3D(const float *v);


			/** Generates a random vector around the sphere */
			static Vector3D cosRandom(const Vector3D& normal);

			/** Creates a random vector around a hemisphere denoted by the normal*/
			static Vector3D hemiRandom(const Vector3D& normal);


            /** Calculates the vector product between this vector and another,
              and returns the resulting vector */
			inline Vector3D vectorProduct(const Vector3D &v) const
			{
				return Vector3D(	dv[1] * v.dv[2] - dv[2] * v.dv[1],
			    					dv[2] * v.dv[0] - dv[0] * v.dv[2],
				    				dv[0] * v.dv[1] - dv[1] * v.dv[0]	);
			}

            /** Calculates the dot product (scalar product) between this vector
              and the given one, and returns the resulting value */
			inline float dotProduct(const Vector3D &v) const
			{
				return  dx * v.dx +	dy * v.dy +	dz * v.dz;
			}
	};

	bool operator==(const Vector3D &v1, const Vector3D &v2);
	bool operator!=(const Vector3D &v1, const Vector3D &v2);
	
}

#endif
