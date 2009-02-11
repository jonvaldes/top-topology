#ifndef NTUPLE_H
#define NTUPLE_H

namespace util{
	/** Template for a tuple of all-equal objects*/
	template<typename T, int size>
		class NTuple
		{
			protected:
				/**Elements in the tuple */
				T v[size];
			public:
				/** Read only reference to an element in the tuple */
				const T& operator[](int pos) const {return v[pos];}
				/** Read-write reference to an element in the tuple */
				T& operator[](int pos) {return v[pos];}
		};

	/** Tuple of 2 elements */
	template <typename T> class Pair : public NTuple<T,2>{};

	/** Tuple of 3 elements */
	template <typename T> class Triplet : public NTuple<T,3>{
		public:
			/** Default constructor */
			Triplet(){}
			/** Copy constructor from a 3 objects NTuple */
			Triplet(const NTuple<T,3>& t)
			{
				for(int i=0;i<3;i++)
					(*this)[i] = t[i]; 
			}
	};
}

#endif
