// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "VectorTypes.h"

/**
 * TPointSetAdapter provides a very generic interface to an indexable list of points.
 * The list may be sparse, ie some indices may be invalid.
 */
template<typename RealType>
struct TPointSetAdapter
{
	/** Maximum point index */
	TFunction<int32()> MaxPointID;
	/** Number of points. If PointCount == MaxPointID, then there are no gaps in the index list */
	TFunction<int32()> PointCount;
	/** Returns true if this index valid */
	TFunction<bool(int32)> IsPoint;
	/** Get point at this index */
	TFunction<FVector3<RealType>(int32)> GetPoint;
	/** Returns a timestamp. If point set changes, timestamp should also change. */
	TFunction<int32()> Timestamp;

	/** Returns true if this point set has per-point normals */
	TFunction<bool()> HasNormals;
	/** Get the normal at a point index */
	TFunction<FVector3f(int32)> GetPointNormal;
};

typedef TPointSetAdapter<double> FPointSetAdapterd;
typedef TPointSetAdapter<float> FPointSetAdapterf;

