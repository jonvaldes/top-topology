#include <math.h>
#include "Vector3D.h"
#include "Point3D.h"
#include <stdlib.h>

using namespace geom;

float Vector3D::module() const
{
    return sqrtf(SQR(dx)+SQR(dy)+SQR(dz));
}

Vector3D Vector3D::getNormalized() const
{
    return (*this)/module(); 
}

Vector3D Vector3D::getWithModule(float newModule) const
{
    float currentModule = module();
    if(currentModule == 0)
        return Vector3D(0,0,0);

    return (*this)*(newModule/currentModule);
}


Vector3D Vector3D::operator/(float v) const
{
    return (*this)*(1/v);
}

Vector3D Vector3D::operator*(float v) const
{
    return Vector3D(dx*v,dy*v,dz*v);
}

Vector3D Vector3D::operator+(const Vector3D& v) const
{
	Vector3D res(*this);
	res+=v;
	return res;
}

Vector3D & Vector3D::operator+=(const Vector3D &v)
{
	dx+=v.dx, dy+=v.dy, dz+=v.dz;
	return *this;
}


Vector3D::Vector3D() : dx(0), dy(0), dz(0)
{
}

Vector3D::Vector3D(float x, float y, float z) : dx(x), dy(y), dz(z)
{

}

Vector3D::Vector3D(const float * v)
{
    dx = v[0];
    dy = v[1];
    dz = v[2];
}


Vector3D Vector3D::cosRandom(const Vector3D& normal) 
{
	double e1 = rand()/(float)RAND_MAX;
	double e2 = rand()/(float)RAND_MAX;

	// Angle from normal
	double theta = acos(sqrt(e1));

	// Angle about normal
	double phi   = 2 * M_PI * e2;

	// Make a coordinate system
	Vector3D U = normal;
	Vector3D V = Vector3D(1,0,0);

	if (abs(normal.dotProduct(V)) > .9)
		V = Vector3D(0,1,0);
	

	Vector3D W = U.vectorProduct(V);
	V = W.vectorProduct(U);

	// Convert to rectangular form
	return U * cos(theta) + (V * cos(phi) + W * sin(phi)) * sin(theta);
}


Vector3D Vector3D::hemiRandom(const Vector3D& normal) 
{
	Vector3D v = cosRandom(Vector3D(0,0,1));

	if (v.dotProduct(normal) < 0) 
		return -v;
	else 
		return v;
}

bool geom::operator==(const Vector3D &v1, const Vector3D &v2)
{
	if(v1.dx != v2.dx || v1.dy != v2.dy || v1.dz != v2.dz)
		return false;
	return true;
}

bool geom::operator!=(const Vector3D &v1, const Vector3D &v2)
{
	return !(v1 == v2);
}

