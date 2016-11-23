// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "MutableInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UMutableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IMutableInterface
{
	GENERATED_IINTERFACE_BODY()
		// This function will be called when the user uses the object
		virtual void OnHighlight(AController * user);
	// Called on deselect
	virtual void DeHighlight(AController * user);

	// Mutability functions
	virtual void UpdateMoveLocRot(FVector PivotLoc, FVector PosDelta, FRotator frRotDelta, FQuat fqRotDelta, AController * user);
	virtual void SetScale(FVector newScale, AController * user);

	// Return values
	virtual FString GetPrimitiveType(AController * user);
	virtual FVector GetScale(AController * user);
	//virtual void SetRotationWithOffset(FRotator Rotation, FRotator Offset);
	virtual FRotator GetRotation();

	virtual void SetActive(AController * user);
	virtual void SetInactive(AController * user);
};
