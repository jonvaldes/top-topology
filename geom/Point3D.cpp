#include "Point3D.h"
#include <math.h>

using namespace geom;

Point3D::Point3D(float px, float py, float pz) : x(px), y(py), z(pz)
{   
}

Point3D::Point3D()
{
    x=y=z=0.0;
}

Point3D::Point3D(const float * pv)
{
    v[0] = pv[0];
    v[1] = pv[1];
    v[2] = pv[2];
}

Point3D Point3D::operator+(const Point3D &p) const
{
	Point3D result;
	for(int i=0;i<3;++i)
		result.v[i] = v[i] + p.v[i];
	return result;
}

Point3D Point3D::operator/(float scalar) const
{
	Point3D result;
	for(int i=0;i<3;++i)
		result.v[i] = v[i] / scalar;
	return result;
}

Point3D Point3D::operator+(const Vector3D &v) const
{
    return Point3D(x+v.dx, y+v.dy, z+v.dz);
}

Point3D Point3D::operator-(const Vector3D &v) const
{
    return Point3D(x-v.dx, y-v.dy, z-v.dz);
}

