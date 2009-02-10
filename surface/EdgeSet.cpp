#include "EdgeSet.h"

using namespace voxel;

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

void EdgeSet::removeComponent(PointID p)
{
	std::vector<PointID> toRemove;
	std::vector<PointID> toRemoveNext;
	toRemove.push_back(p);

	while(toRemove.size() > 0)
	{
		int numToRemove = toRemove.size();
		for(int i=0; i<numToRemove;++i)
		{
			// We take note of the points connected to this one
			std::vector<PointID> connected = getConnectedTo(toRemove[i]);
			toRemoveNext.insert(toRemoveNext.begin(), connected.begin(), connected.end());
			// And remove this point
			std::pair<AdjacencyList::iterator, AdjacencyList::iterator> range; 
			range = m_edges.equal_range(toRemove[i]);
			m_edges.erase(range.first,range.second);
		}
		toRemove = toRemoveNext;
		toRemoveNext.clear();
	}
}

EdgeSet EdgeSet::getComponent(PointID p) const
{
	EdgeSet result;
	std::vector<PointID> toAdd, toAddNext;

	toAdd.push_back(p);

	while(toAdd.size() > 0)
	{
		int numToAdd= toAdd.size();
		for(int i=0; i<numToAdd;++i)
		{
			// We take note of the points connected to this one
			std::vector<PointID> connected = getConnectedTo(toAdd[i]);
			toAddNext.insert(toAddNext.begin(), connected.begin(), connected.end());
			// And add this point
			std::pair<AdjacencyList::const_iterator, AdjacencyList::const_iterator> range; 
			range = m_edges.equal_range(toAdd[i]);
			result.m_edges.insert(range.first,range.second);
		}
		toAdd = toAddNext;
		toAddNext.clear();
	}
	return result;
}
