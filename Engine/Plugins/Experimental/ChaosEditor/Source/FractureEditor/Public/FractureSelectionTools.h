// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

class UGeometryCollectionComponent;

class FFractureSelectionTools
{
public:
	/**
	 * Sets the selected bones on a geometry collection component
	 *
	 * @param GeometryCollectionComponent	The component with bones being selected
	 * @param BoneIndices					The indices to select
	 * @param bClearCurrentSelection		Whether or not to clear the current selection set or just append the new indices to it				
	 */
	static void ToggleSelectedBones(UGeometryCollectionComponent* GeometryCollectionComponent, TArray<int32>& BoneIndices, bool bClearCurrentSelection);
	static void ClearSelectedBones(UGeometryCollectionComponent* GeometryCollectionComponent);

	static void SelectNeighbors(UGeometryCollectionComponent* GeometryCollectionComponent);
};