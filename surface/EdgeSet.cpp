#include "EdgeSet.h"

using namespace surface;

EdgeSet::EdgeSet()
{
}

EdgeSet::~EdgeSet()
{
	m_edges.clear();
}

int EdgeSet::totalEdgesCount() const
{
	return m_edges.size() / 2; // The edges are stored 2 times, one for each direction
}

AdjacencyList::const_iterator EdgeSet::begin()
{
	return m_edges.begin();
}

AdjacencyList::const_iterator EdgeSet::end()
{
	return m_edges.end();
}

void EdgeSet::addEdge(PointID p1, PointID p2)
{
	if(!areConnected(p1,p2))
	{
		m_edges.insert(std::pair<PointID, PointID>(p1,p2));
		m_edges.insert(std::pair<PointID, PointID>(p2,p1));
	}
}

void EdgeSet::clear()
{
	m_edges.clear();
}

bool EdgeSet::areConnected(PointID p1, PointID p2) const
{
	std::pair<AdjacencyList::const_iterator, AdjacencyList::const_iterator> firstRange; 
	firstRange = m_edges.equal_range(p1); // we get the range of elements with first == p1
	AdjacencyList::const_iterator it;

	for(it = firstRange.first; it!=firstRange.second;++it)
		if(it->second == p2)
			return true;

	return false;
}

std::vector<PointID> EdgeSet::getConnectedTo(PointID p) const
{
	std::pair<AdjacencyList::const_iterator, AdjacencyList::const_iterator> range; 
	range = m_edges.equal_range(p); // we get the range of elements with first == p1
	std::vector<PointID> result;
	AdjacencyList::const_iterator it;
		for(it = range.first; it!=range.second;++it)
			result.push_back(it->second);

	return result;
}

void EdgeSet::mergePoint(PointID p, PointID into)
{
	std::pair<AdjacencyList::iterator, AdjacencyList::iterator> pointsRange; 
	pointsRange = m_edges.equal_range(p); // we get the range of elements with first == p 

	// Now we remove them all, and change the points connected to this one be connected to 'into'
	AdjacencyList::const_iterator it;
	for(it = pointsRange.first; it!=pointsRange.second; ++it)
	{
		PointID other = it->second;
		std::pair<AdjacencyList::iterator, AdjacencyList::iterator> otherRange; 
		otherRange = m_edges.equal_range(other);
		addEdge(into, other);
		AdjacencyList::iterator it2;
		for(it2 = otherRange.first; it2!= otherRange.second; ++it2)
			if(it2->second == p)
				it2->second = into;
	}
	m_edges.erase(pointsRange.first, pointsRange.second);
}

