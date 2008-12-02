#ifndef CONNECTED_TUPLE_SET_H
#define CONNECTED_TUPLE_SET_H

#include <set>
#include <vector>
#include "NTuple.h"

using namespace std;

namespace util{
	/** Utility class used to check if a set of tuples form a connected graph (used for mesh normals calculation)*/
	template < typename T, int size> class ConnectedTupleSet
	{
		protected:
			/** Unique objects in the set */
			set<T> m_individuals;
			/** Tuples in the set */
			vector< NTuple<T,size> > m_tuples;

		public:
			/** Clears all data in the object */
			void clearAll()
			{
				m_tuples.erase(m_tuples.begin(),m_tuples.end());
				m_individuals.erase(m_individuals.begin(),m_individuals.end());
			}
			/** Checks if the tuple t has any elements in common with this TupleSet */
			bool checkConnection(NTuple<T,size> t)
			{
				for(int i=0;i<size;i++)
				{
					if(m_individuals.find(t[i])!=m_individuals.end())
						return true;
				}
				return false;
			}
			/** Adds another tuple to the set */
			void insertTuple(NTuple<T,size> t)
			{
				for(int i=0;i<size;i++)
					m_individuals.insert(t[i]);
				m_tuples.push_back(t);
			}

			/** Returns another ConnectedTupleSet that is the sum of two sets */
			ConnectedTupleSet<T,size> operator+(const ConnectedTupleSet<T,size>& other ) const
			{
				ConnectedTupleSet<T,size> result;
				result+=*this;
				result+=other;
				return result;
			}

			/** Adds the elements of another tupleSet to this one */
			const ConnectedTupleSet<T,size>& operator+=(const ConnectedTupleSet<T,size>& other)
			{
				m_individuals.insert(other.m_individuals.begin(), other.m_individuals.end());
				m_tuples.insert(m_tuples.end(), other.m_tuples.begin(), other.m_tuples.end());
				return *this;
			}

			/** Gets a read-only reference to the tuples in this set */
			const vector<NTuple<T,size> > & getTuples(){return m_tuples;}
	};

}
#endif

