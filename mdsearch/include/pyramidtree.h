#ifndef MDSEARCH_PYRAMIDTREE_H
#define MDSEARCH_PYRAMIDTREE_H

// Only define if you want a hack which causes the Pyramid Tree hasher
// to ignore dimensions when a point is at the min or max boundaries
// for that dimension
#define BOUNDARY_VALUE_HACK 1

#include "hashstruct.h"

namespace mdsearch
{

	template<int D>
	class PyramidTree : public HashStructure<D>
	{

	public:
		PyramidTree(const Boundary<D>& boundary) : boundary(boundary)
		{
			// Compute the interval between buckets 
			bucketInterval = static_cast<Real>( MAX_BUCKET_NUMBER / (D * 2) );
			bucketInterval = floor(bucketInterval);
			// Initial median point is centre of data space
			// NOTE: This is currently unused, but could be updated to create
			// an adaptive variant of the structure (e.. Extended Pyramid-Technique)
			for (unsigned int d = 0; (d < D); d++)
				medianPoint[d] = boundary.minVal(d) + ((boundary.maxVal(d) - boundary.minVal(d)) / 2.0f);
		}

	protected:
		static const Real MAX_BUCKET_NUMBER = 300000;

		/* Normalise value into 0-1 range based on min-max interval. */
		inline Real normaliseCoord(Real coord, Real min, Real max)
		{
			return (coord - min) / (max - min);
		}
		
		/* Compute Pyramid height of a point, for a specific pair of
		 * pyramid (that are both for the same dimension). */
		inline Real pyramidHeight(Real coord, Real min, Real max)
		{
			return std::abs(0.5f - normaliseCoord(coord, min, max));
		}

		/* Compute pyramid value of the given point, using the original Pyramid-technique. */
		virtual HashType hashPoint(const Point<D>& p)
		{
			int index = 0;
			int dMax = 0;
			Real dMaxHeight = pyramidHeight(p[0], boundary.minVal(0), boundary.maxVal(0));
			for (int d = 1; (d < D); d++)
			{
				Real currentHeight = pyramidHeight(p[d], boundary.minVal(d), boundary.maxVal(d));
				#ifdef BOUNDARY_VALUE_HACK
					if (compare(currentHeight, 0.5f) == 0)
						continue;
				#endif

				if (dMaxHeight < currentHeight)
				{
					dMax = d;
					dMaxHeight = currentHeight;
				}
			}

			if (normaliseCoord(p[dMax], boundary.minVal(dMax), boundary.maxVal(dMax)) < 0.5f)
				index = dMax; // pyramid lower than central point
			else 
				index = dMax + D; // pyramid higher than central point

			return (index + dMaxHeight) * bucketInterval;			
		}



		// Entire region of space the Pyramid tree covers
		// (points outside this region are ignored)
		Boundary<D> boundary;
		// Interval between buckets
		Real bucketInterval;
		// Median values of the data set.
		Point<D> medianPoint;

	};

}

#endif