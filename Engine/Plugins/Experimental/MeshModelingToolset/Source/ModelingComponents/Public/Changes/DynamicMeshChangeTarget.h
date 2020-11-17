// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveToolChange.h"

#include "Changes/MeshChange.h"
#include "Changes/MeshVertexChange.h"
#include "Changes/MeshReplacementChange.h"

#include "DynamicMeshChangeTarget.generated.h"


class FDynamicMesh3;

/**
 * Bare bones wrapper of FDynamicMesh3 that supports MeshReplacementChange-based updates.  Shares the mesh ptr with the MeshReplacementChange objects, so it must not be changed directly
 * TODO: also support other MeshChange objects (by making a copy of the mesh when applying these changes)
 */
UCLASS(Transient)
class MODELINGCOMPONENTS_API UDynamicMeshReplacementChangeTarget : public UObject, public IMeshReplacementCommandChangeTarget
{
	GENERATED_BODY()

protected:
	TSharedPtr<const FDynamicMesh3> Mesh;

public:
	UDynamicMeshReplacementChangeTarget() = default;
	UDynamicMeshReplacementChangeTarget(TSharedPtr<const FDynamicMesh3> Mesh) : Mesh(Mesh)
	{}
	virtual ~UDynamicMeshReplacementChangeTarget() = default;

	/**
	* Apply/Revert a general mesh change to the internal mesh   (implements IMeshReplacementCommandChangeTarget)
	*/
	virtual void ApplyChange(const FMeshReplacementChange* Change, bool bRevert) override;

	virtual void SetMesh(TSharedPtr<const FDynamicMesh3> MeshIn)
	{
		Mesh = MeshIn;
	}

	const TSharedPtr<const FDynamicMesh3>& GetMesh() const
	{
		return Mesh;
	}

	/**
	 * Update the mesh and return a corresponding FChange
	 * Does not emit the change.
	 * Does not fire OnMeshChanged.
	 */
	TUniquePtr<FMeshReplacementChange> ReplaceMesh(const TSharedPtr<const FDynamicMesh3>& UpdateMesh);

	/**
	* This delegate fires when a FCommandChange is applied to this component, so that
	* parent objects know the mesh has changed.
	*/
	FSimpleMulticastDelegate OnMeshChanged;

};

