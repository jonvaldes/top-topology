#ifndef __EDGE_SET_H__
#define __EDGE_SET_H__
#include <map>
#include <vector>
#include <sys/types.h>
namespace voxel
{
	typedef uint64_t PointID;
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

			bool areConnected(PointID p1, PointID p2) const;
			std::vector<PointID> getConnectedTo(PointID p) const;

			void removeComponent(PointID p);
			EdgeSet getComponent(PointID p) const;

		private:
			AdjacencyList m_edges;
	};
}

#endif

