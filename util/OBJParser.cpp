#include "OBJParser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
using namespace modelfile;
using namespace std;

OBJParser::OBJParser(const char * filename)
{
	ifstream ifs ( filename , ifstream::in );
	while(ifs)
	{
		string token;
		ifs>>token;
		if(token == "v")
		{
			geom::Point3D p;
			ifs>>p.v[0];
			ifs>>p.v[1];
			ifs>>p.v[2];
			m_surface.addPoint(p);
		}
		else if(token == "f")
		{
			surface::Face f;
			ifs>>f[0];
			ifs>>f[1];
			ifs>>f[2];
			ifs>>f[3];
			for(int i=0;i<4;++i)
				--f[i]; // obj numbers the faces starting from 1
			m_surface.addFace(f);
		}
		else
		{
		//		printf("Unknown token: \"%s\"",token.c_str());
		}
	}
}

