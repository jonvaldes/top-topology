#ifndef __EDGE_SET_H__
#define __EDGE_SET_H__
#include <map>
#include <vector>
#include "PointID.h"

namespace surface 
{
	typedef std::multimap<PointID,PointID> AdjacencyList;
	
	class EdgeSet
	{
		public:
			EdgeSet();
			~EdgeSet();

			int totalEdgesCount() const;
			AdjacencyList::const_iterator begin();
			AdjacencyList::const_iterator end();

			void addEdge(PointID p1, PointID p2);
			void clear();

			void mergePoint(PointID p, PointID into);

			bool areConnected(PointID p1, PointID p2) const;
			std::vector<PointID> getConnectedTo(PointID p) const;
			std::vector<PointID> getConnectedTo(PointID p, PointID excludedP1, PointID excludedP2) const;

		private:
			AdjacencyList m_edges;
	};
}

#endif

