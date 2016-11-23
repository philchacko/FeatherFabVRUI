// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "MyActor.h"
#include "SelectableInterface.h"
#include "VRCharacter.generated.h"

UCLASS()
class KITTYHAWK_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	/* Component to specify origin for the HMD */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* VROriginComp;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	bool bPositionalHeadTracking;

	/* Motion Controllers */

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UMotionControllerComponent* LeftHandComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UMotionControllerComponent* RightHandComponent;

public:
	// Sets default values for this character's properties
	AVRCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetupVROptions();

	/* Resets HMD Origin position and orientation */
	void ResetHMDOrigin();

	/* Toggle between Seated and Standing VR Tracking */
	void ToggleTrackingSpace();

	// Spawn object function and array to store
	TArray<AMyActor*> Objects;
	void SpawnObject();

	// Right controller triggered.
	void RTriggerPressed();
	
	//AMyActor* SpawnSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Objects")
		TSubclassOf<AMyActor> SpawnSphereAsset; // Gets connected inside Editor for VR Character
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Objects")
		TSubclassOf<AMyActor> SpawnCubeAsset; // Gets connected inside Editor for VR Character
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Objects")
		TSubclassOf<AMyActor> SpawnConeAsset; // Gets connected inside Editor for VR Character
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Objects")
		TSubclassOf<AMyActor> SpawnCylAsset; // Gets connected inside Editor for VR Character
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Objects")
		TSubclassOf<AMyActor> SpawnColbertAsset; // Gets connected inside Editor for VR Character
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Objects")
		TSubclassOf<AMyActor> SpawnCollarAsset; // Gets connected inside Editor for VR Character


	// TODO: Add equivalent variables for cube

	// Add destroy/resize functionality
	bool bIsDestroyingPart = false;
	bool bIsResizingObj = false;
	//FVector fvResizeStartLoc;
	bool bIsMovingObj = false;
	FVector fvMoveLastLoc;
	FRotator frMoveLastRot;
	FQuat fqMoveLastRot;

	// Selects spawn brush.
	ISelectableInterface* SelectFocus;
	ISelectableInterface* LastActiveBrush;

	// For handling mutable actor actions.
	IMutableInterface* MutableFocus;
	IMutableInterface* MoveFocus;
	
	FString spawnBrush = "sphere";

protected:

	FVector fvStartResizeScale;
	FVector fvStartRightHandLoc;
	FVector fvStartLeftHandLoc;

	void BeginDestroyPart();
	void EndDestroyPart();
	void BeginResizeObj();
	void EndResizeObj();
	void BeginMoveObj();
	void EndMoveObj();
	void PrintToCSV();
};
