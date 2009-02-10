#ifndef __OBJ_PARSER_H__
#define __OBJ_PARSER_H__

#include "NTuple.h"
#include <stdint.h>
#include <vector>
#include "../surface/Surface.h"

namespace modelfile
{
	typedef std::vector< geom::Point3D > PointCoordsVector;
	typedef std::vector< surface::Face > FacePointIDsVector;

	class OBJParser
	{
		public:
			OBJParser(const char * filename);
			const surface::Surface &getSurface() const {return m_surface;}
		private:
			surface::Surface m_surface;
	};
}
#endif

