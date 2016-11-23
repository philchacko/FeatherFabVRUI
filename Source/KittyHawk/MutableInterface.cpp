// Fill out your copyright notice in the Description page of Project Settings.

#include "KittyHawk.h"
#include "MutableInterface.h"
#include "stdio.h"

UMutableInterface::UMutableInterface(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

//This is required for compiling,  and its the base version, you can put something here and it will be the default behaviour
void IMutableInterface::OnHighlight(AController * user)
{
	//printf("Mutable highlight used.\n");
}

void IMutableInterface::DeHighlight(AController * user)
{
	//printf("IMutable::DeHighlight used.\n");
}

void IMutableInterface::UpdateMoveLocRot(FVector PivotLoc, FVector PosDelta, FRotator frRotDelta, FQuat fqRotDelta, AController * user)
{
	//printf("IMutable::UpdateMoveLocRot used.\n");
}

void IMutableInterface::SetScale(FVector newScale, AController * user) {
	//printf("IMutable::UpdateScale used.\n");
}

FVector IMutableInterface::GetScale(AController * user) {
	//printf("ISelectable::GetScale called.\n");
	FVector defaultNull;
	return defaultNull;
}

/*
void IMutableInterface::SetRotationWithOffset(FRotator Rotation, FRotator Offset) {
	printf("Rotation with offset used.\n");
}*/

FRotator IMutableInterface::GetRotation() {
	return FRotator::ZeroRotator;
}

FString IMutableInterface::GetPrimitiveType(AController * user) {
	//printf("ISelectable::GetPrimitiveType called.\n");
	return "none";
}

void IMutableInterface::SetActive(AController * user)
{
	//printf("ISelectabled::SetActive used.\n");
}

void IMutableInterface::SetInactive(AController * user)
{
	//printf("ISelectabled::SetActive used.\n");
}