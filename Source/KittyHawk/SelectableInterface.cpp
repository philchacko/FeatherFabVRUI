// Fill out your copyright notice in the Description page of Project Settings.

#include "KittyHawk.h"
#include "SelectableInterface.h"
#include "stdio.h"
#include "engine.h"

USelectableInterface::USelectableInterface(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

//This is required for compiling,  and its the base version, you can put something here and it will be the default behaviour
void ISelectableInterface::OnUsed(AController * user)
{
	UE_LOG(LogTemp, Warning, TEXT("ISelectable used.\n"));
}

void ISelectableInterface::Deselect(AController * user)
{
	UE_LOG(LogTemp, Warning, TEXT("ISelectabled::Deselected used.\n"));
}

FString ISelectableInterface::GetPrimitiveType(AController * user) {
	UE_LOG(LogTemp, Warning, TEXT("ISelectable::GetPrimitiveType called.\n"));
	return "none";
}

void ISelectableInterface::SetActive(AController * user)
{
	UE_LOG(LogTemp, Warning, TEXT("ISelectabled::SetActive used.\n"));
}

void ISelectableInterface::SetInactive(AController * user)
{
	UE_LOG(LogTemp, Warning, TEXT("ISelectabled::SetActive used.\n"));
}

