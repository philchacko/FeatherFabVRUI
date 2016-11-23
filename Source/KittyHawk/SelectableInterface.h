// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SelectableInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class USelectableInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ISelectableInterface
{
	GENERATED_IINTERFACE_BODY()
	// This function will be called when the user uses the object
	virtual void OnUsed(AController * user);
	// Called on deselect
	virtual void Deselect(AController * user);
	// Return primitive type
	virtual FString GetPrimitiveType(AController * user);
	virtual void SetActive(AController * user);
	virtual void SetInactive(AController * user);
};
