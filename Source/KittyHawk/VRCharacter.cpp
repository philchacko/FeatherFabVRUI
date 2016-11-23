// Fill out your copyright notice in the Description page of Project Settings.

#include "KittyHawk.h"

/* VR Includes */
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"
#include "stdio.h"
#include "VRCharacter.h"
#include "SelectActor.h"
#include "MyActor.h"
#include "Engine.h"


// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraOrigin"));
	VROriginComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	/* Assign to the VR origin component so any reset calls to the HMD can reset to 0,0,0 relative to this component */
	CameraComp->SetupAttachment(VROriginComp);

	LeftHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHandComponent->Hand = EControllerHand::Left;
	LeftHandComponent->SetupAttachment(VROriginComp);

	RightHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHandComponent->Hand = EControllerHand::Right;
	RightHandComponent->SetupAttachment(VROriginComp);

	bPositionalHeadTracking = false;
	TArray<AMyActor> Objects;
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupVROptions();

	//AllocConsole();
	//freopen("conin$", "r", stdin);
	//freopen("conout$", "w", stdout);
	//freopen("conout$", "w", stderr);

	if (!LastActiveBrush)
	{
		for (TActorIterator<ASelectActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			UE_LOG(LogTemp, Warning, TEXT("Found select actor.\n"));

			ASelectActor *aSelector = *ActorItr;
			if (aSelector->PrimitiveType == "sphere") {
				LastActiveBrush = Cast<ISelectableInterface>(aSelector);
				UE_LOG(LogTemp, Warning, TEXT("Attempted cast.\n"));
			}
		}
	}
}

void AVRCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//FreeConsole();
}

void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDestroyingPart) {
		int32 objCount = Objects.Num();
		for (int32 i = objCount - 1; i >= 0; i--) {
			if (Objects[i]->bPartIsWithinRange) {
				Objects[i]->DeletePart();
				Objects.RemoveAtSwap(i);
			}
		}
	}

	//float fDistance;
	//int32 iClosest = -1;
	int32 objCount = Objects.Num();
	
	/*
	if (bIsResizingObj || bIsMovingObj) {
		for (int32 j = objCount - 1; j >= 0; j--) {
			if (Objects[j]->bPartIsWithinRange) {
				if (iClosest < 0) {
					iClosest = j;
					fDistance = (RightHandComponent->GetComponentLocation() - Objects[j]->GetActorLocation()).Size();
				}
				else {
					if ((RightHandComponent->GetComponentLocation() - Objects[j]->GetActorLocation()).Size() < fDistance) {
						iClosest = j;
						fDistance = (RightHandComponent->GetComponentLocation() - Objects[j]->GetActorLocation()).Size();
					}
				}
			}
		}
	}*/

	if (bIsResizingObj) {
		FVector fvNewScale = FVector(0.1f, 0.1f, 0.1f);
		//FVector fvActorLoc;
		FVector fvRightHandLoc;
		FVector fvLeftHandLoc;
		//float fActorStartScale;
		//float fMaxScale;
		//if (iClosest >= 0) {
		if (MutableFocus) {
			//fvActorLoc = Objects[iClosest]->GetActorLocation();
			fvRightHandLoc = RightHandComponent->GetComponentLocation();
			fvLeftHandLoc = LeftHandComponent->GetComponentLocation();
			float newMagnitude = (fvRightHandLoc - fvLeftHandLoc).Size() / (fvStartRightHandLoc - fvStartLeftHandLoc).Size();
			FVector newScale = fvStartResizeScale * newMagnitude;
			MutableFocus->SetScale(newScale, Controller);

			//float newMagnitude = (fvRightHandLoc - fvActorLoc).Size() * 2; // Convert radius to diameter
			//fActorStartScale = Objects[iClosest]->fActorScale;
			//fvNewScale = fActorStartScale * (RightHandComponent->GetComponentLocation() - fvActorLoc) / (fvResizeStartLoc - fvActorLoc);
			//fMaxScale = fvNewScale.GetAbsMax();
			/*fvNewScale.X = newMagnitude*.01f;
			fvNewScale.Y = newMagnitude*.01f;
			fvNewScale.Z = newMagnitude*.01f;*/
			//Objects[iClosest]->SetActorScale3D(fvNewScale);
		}
	}
	else {
		int32 objCount = Objects.Num();
		for (int32 i = objCount - 1; i >= 0; i--) {
			if (Objects[i]->bPartIsWithinRange) {
				Objects[i]->fActorScale = Objects[i]->GetActorScale().X;
			}
		}
	}

	if (MoveFocus) {
		//int32 objCount = Objects.Num();
		FVector fvPivotLoc = RightHandComponent->GetComponentLocation();
		//FRotator MoveObjOffset = MoveFocus->GetRotation();
		//MoveObjOffset = frMoveLastRot - MoveObjOffset;

		//MoveFocus->SetRotationWithOffset(RightHandComponent->GetComponentRotation(), MoveObjOffset);

		//frMoveLastRot.Quaternion().Inverse() * RightHandComponent->GetComponentQuat();
		//RightHandComponent->GetComponentRotation().Quaternion().Inverse();

		FVector fvMoveDelta = fvPivotLoc - fvMoveLastLoc;
		FRotator frRotDelta = RightHandComponent->GetComponentRotation() - frMoveLastRot;
		frRotDelta.Normalize();
		FQuat fqRotDelta =  RightHandComponent->GetComponentQuat() * fqMoveLastRot.Inverse();


		//FVector fvForward = RightHandComponent->GetForwardVector();

		MoveFocus->UpdateMoveLocRot(fvPivotLoc, fvMoveDelta, frRotDelta, fqRotDelta, Controller); 
		/*
		if (iClosest >= 0) {	
			FVector fvNewLoc = Objects[iClosest]->GetActorLocation() + fvMoveDelta;
			FRotator frNewRot = Objects[iClosest]->GetActorRotation() + frMoveRotDelta;
			frNewRot.Normalize();
			Objects[iClosest]->SetActorLocation(fvNewLoc);
			Objects[iClosest]->SetActorRotation(frNewRot);
		}*/
		fvMoveLastLoc = RightHandComponent->GetComponentLocation();
		frMoveLastRot = RightHandComponent->GetComponentRotation(); //RightHandComponent->GetComponentRotation();
		fqMoveLastRot = RightHandComponent->GetComponentQuat();
	}
	
	if (Controller && Controller->IsLocalPlayerController()) // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	{
		FVector RightHandLoc;
		FRotator RightHandRot;

		RightHandLoc = RightHandComponent->GetComponentLocation();
		RightHandRot = RightHandComponent->GetComponentRotation();
		const FVector StartTrace = RightHandLoc; // trace start is the camera location
		const FVector Direction = RightHandRot.Vector();
		const FVector EndTrace = StartTrace + Direction * 600; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

															   // Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(FName(TEXT("HMDTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult Hit(ForceInit);
		//GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECC_Visibility, TraceParams); // simple trace function
		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, TraceParams);
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false);

		ISelectableInterface* selectable = Cast<ISelectableInterface>(Hit.GetActor()); // we cast the hit actor to the Selectable interface
		IMutableInterface* mutableObj = Cast<IMutableInterface>(Hit.GetActor()); // Cast hit actor to Mutable interface
		AMyActor* hitActor = Cast<AMyActor>(Hit.GetActor());
		if (mutableObj) {
			// do stuff for mutable
			if (MutableFocus && (MutableFocus != mutableObj))
				MutableFocus->DeHighlight(Controller);
			MutableFocus = mutableObj;
			mutableObj->OnHighlight(Controller);
			//printf("Try mutable highlight.\n");
		}
		else if (selectable) // we are looking to a usable object
		{
			if (SelectFocus && (SelectFocus != selectable))
				SelectFocus->Deselect(Controller);
			SelectFocus = selectable; // as the actor under crosshairs is a usable actor, we store it for the hud.
			selectable->OnUsed(Controller);
			//printf("try using.");
		}
		else
		{
			if (SelectFocus)
				SelectFocus->Deselect(Controller);
			if (MutableFocus)
				MutableFocus->DeHighlight(Controller);
			SelectFocus = NULL; // nothing, so we set the UseFocus pointer to NULL, so it wont give problems
			MutableFocus = NULL;

			if (hitActor) { // we are looking to a canvas object
				//do stuff
				// Calculate actorloc-starttrace, add rotDelta for newloc, translate with righthand
			}
		}
	}

	/*if (MyController != NULL) // Nullcheck motion controller
	{
	if (MyController->bIsDestroyingPart && bPartIsWithinRange)
	{
	DeletePart();
	}
	}*/
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("ToggleTrackingSpace", IE_Pressed, this, &AVRCharacter::ToggleTrackingSpace);
	InputComponent->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);
	InputComponent->BindAction("RTriggerPressed", IE_Pressed, this, &AVRCharacter::RTriggerPressed);
	InputComponent->BindAction("DeleteObject", IE_Pressed, this, &AVRCharacter::BeginDestroyPart);
	InputComponent->BindAction("DeleteObject", IE_Released, this, &AVRCharacter::EndDestroyPart);
	InputComponent->BindAction("PrintToCSV", IE_Pressed, this, &AVRCharacter::PrintToCSV);
	InputComponent->BindAction("ResizeObject", IE_Pressed, this, &AVRCharacter::BeginResizeObj);
	InputComponent->BindAction("ResizeObject", IE_Released, this, &AVRCharacter::EndResizeObj);
	InputComponent->BindAction("MoveObject", IE_Pressed, this, &AVRCharacter::BeginMoveObj);
	InputComponent->BindAction("MoveObject", IE_Released, this, &AVRCharacter::EndMoveObj);
}

void AVRCharacter::SetupVROptions()
{
	IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
	if (HMD && HMD->IsStereoEnabled())
	{
		/* Disable/Enable positional movement to pin camera translation */
		HMD->EnablePositionalTracking(bPositionalHeadTracking);

		/* Remove any translation when disabling positional head tracking */
		if (!bPositionalHeadTracking)
		{
			CameraComp->SetRelativeLocation(FVector(0, 0, 0));
		}
	}
}


void AVRCharacter::ResetHMDOrigin()
{
	IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
	if (HMD && HMD->IsStereoEnabled())
	{
		HMD->ResetOrientationAndPosition();
	}
}


void AVRCharacter::ToggleTrackingSpace()
{
	// TODO: Fix module includes for SteamVR

	//@todo Make this safe once we can add something to the DeviceType enum.  For now, make the terrible assumption this is a SteamVR device.
// 	FSteamVRHMD* SteamVRHMD = (FSteamVRHMD*)(GEngine->HMDDevice.Get());
// 	if (SteamVRHMD && SteamVRHMD->IsStereoEnabled())
// 	{
// 		ESteamVRTrackingSpace TrackingSpace = SteamVRHMD->GetTrackingSpace();
// 		SteamVRHMD->SetTrackingSpace(TrackingSpace == ESteamVRTrackingSpace::Seated ? ESteamVRTrackingSpace::Standing : ESteamVRTrackingSpace::Seated);
// 	}
}

void AVRCharacter::RTriggerPressed() {

	if (SelectFocus) {
		if (LastActiveBrush) {
			LastActiveBrush->SetInactive(Controller);
		}
		/*else {
			//printf("No active brush.\n");
			printf("Try brush init.\n");
			for (TActorIterator<ASelectActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
				printf("Found select actor.\n");

				ASelectActor *aSelector = *ActorItr;
				if (aSelector->PrimitiveType == "sphere") {
					LastActiveBrush = Cast<ISelectableInterface>(aSelector);
					printf("Attempted cast.\n");				
				}
			}
			LastActiveBrush->SetInactive(Controller);
		}*/
		spawnBrush = SelectFocus->GetPrimitiveType(Controller);
		SelectFocus->SetActive(Controller);
		LastActiveBrush = SelectFocus;
		UE_LOG(LogTemp, Warning, TEXT("Select focus.\n"));
	}
	else {
		SpawnObject();
	}
}

void AVRCharacter::SpawnObject()
{
	// TODO: Fix module includes for SteamVR

	//printf("Try spawn.\n");

	FVector RHandLoc = RightHandComponent->GetComponentLocation();
	FRotator RHandRot = RightHandComponent->GetComponentRotation();
	/*RHandLoc.X = 0.0f;
	RHandLoc.Y = 0.0f;
	RHandLoc.Z = 10.0f;*/

	//FTransform RHandTform = RightHandComponent->GetComponentTransform();
	FActorSpawnParameters ourSpawnParams;
	ourSpawnParams.Owner = this;
	ourSpawnParams.Instigator = Instigator;

	//AMyActor* SpawnedSphere = GetWorld()->SpawnActor<AMyActor>(ourSpawningObject, RHandLoc, FRotator::ZeroRotator, ourSpawnParams);
	//Objects.Add(SpawnedSphere);
	if (spawnBrush == "cube")
		Objects.Emplace(GetWorld()->SpawnActor<AMyActor>(SpawnCubeAsset, RHandLoc, RHandRot, ourSpawnParams));
	else if (spawnBrush == "cone")
		Objects.Emplace(GetWorld()->SpawnActor<AMyActor>(SpawnConeAsset, RHandLoc, RHandRot, ourSpawnParams));
	else if (spawnBrush == "cylinder")
		Objects.Emplace(GetWorld()->SpawnActor<AMyActor>(SpawnCylAsset, RHandLoc, RHandRot, ourSpawnParams));
	else if (spawnBrush == "colbert")
		Objects.Emplace(GetWorld()->SpawnActor<AMyActor>(SpawnColbertAsset, RHandLoc, RHandRot, ourSpawnParams));
	else if (spawnBrush == "collar")
		Objects.Emplace(GetWorld()->SpawnActor<AMyActor>(SpawnCollarAsset, RHandLoc, RHandRot, ourSpawnParams));
	else
		Objects.Emplace(GetWorld()->SpawnActor<AMyActor>(SpawnSphereAsset, RHandLoc, RHandRot, ourSpawnParams));
	int32 objCount = Objects.Num();
	if (objCount > 0) {
		FVector fNewScale;
		fNewScale.X = 0.1f;
		fNewScale.Y = 0.1f;
		fNewScale.Z = 0.1f;
		Objects[objCount - 1]->SetActorScale3D(fNewScale);
		Objects[objCount - 1]->fActorScale = 0.1f;
	}
}

void AVRCharacter::BeginDestroyPart()
{
	//printf("Begin destroy part.\n");
	bIsDestroyingPart = true;
}
void AVRCharacter::EndDestroyPart()
{
	//printf("End destroy part.\n");
	bIsDestroyingPart = false;
}

void AVRCharacter::BeginResizeObj()
{
	//printf("Begin resize.\n");

	if (!MutableFocus)
		return;

	bIsResizingObj = true;
	//fvResizeStartLoc = RightHandComponent->GetComponentLocation();

	fvStartResizeScale = MutableFocus->GetScale(Controller);
	fvStartRightHandLoc = RightHandComponent->GetComponentLocation();
	fvStartLeftHandLoc = LeftHandComponent->GetComponentLocation();
}
void AVRCharacter::EndResizeObj()
{
	//printf("End resize.\n");
	bIsResizingObj = false;
}

void AVRCharacter::BeginMoveObj()
{
	//printf("Begin move.\n");
	bIsMovingObj = true;
	MoveFocus = MutableFocus;
	fvMoveLastLoc = RightHandComponent->GetComponentLocation(); //account for raycast
	frMoveLastRot = RightHandComponent->GetComponentRotation(); //RightHandComponent->GetComponentRotation();
	fqMoveLastRot = RightHandComponent->GetComponentQuat();

	/*
	printf("RtCompRot: Roll: %0.2f; Pitch: %0.2f, Yaw: %0.2f\n", frMoveLastRot.Roll, frMoveLastRot.Pitch, frMoveLastRot.Yaw);
	FVector frMoveLastRotVect = frMoveLastRot.Vector();
	printf("RtCompRotVec: X: %0.2f; Y: %0.2f, Z: %0.2f\n", frMoveLastRotVect.X, frMoveLastRotVect.Y, frMoveLastRotVect.Z);
	frMoveLastRot = frMoveLastRotVect.Rotation();
	printf("RtCompRotfromVec: Roll: %0.2f; Pitch: %0.2f, Yaw: %0.2f\n", frMoveLastRot.Roll, frMoveLastRot.Pitch, frMoveLastRot.Yaw);*/
}
void AVRCharacter::EndMoveObj()
{
	//printf("End resize.\n");
	bIsMovingObj = false;
	MoveFocus = NULL;
}

void AVRCharacter::PrintToCSV()
{
	//printf("Print to CSV.\n");
	//UE_LOG(LogTemp, Warning, TEXT("Print to CSV.\n"));

	FILE* pFile;
	pFile = fopen("ExportParams.csv", "w");

	int32 objCount = Objects.Num();
	for (int32 i = 0; i < objCount; i++) {
		FString objType = Objects[i]->PrimitiveType;
		//FVector objLoc = Objects[i]->SM_TBox->GetComponentLocation();
		FVector objLoc = Objects[i]->GetActorLocation();
		FRotator objRot = Objects[i]->GetActorRotation();
		FQuat objRotQuat = Objects[i]->GetActorQuat();
		float objSize1 = 0.0f;
		float objSize2 = 0.0f;
		float objSize3 = 0.0f;
		if (objType == "sphere")
			objSize1 = Objects[i]->fActorScale * 50.0f;
		else if (objType == "cube")
			objSize1 = Objects[i]->fActorScale * 100.0f;
		else if (objType == "cone") {
			objSize1 = Objects[i]->fActorScale * 100.0f;
			objSize2 = Objects[i]->fActorScale * 50.0f;
		}
		else if (objType == "colbert")
			objSize1 = Objects[i]->fActorScale * 0.2f;
		else if (objType == "collar")
			objSize1 = Objects[i]->fActorScale * 1.0f;
		else { // cylinder
			objSize1 = Objects[i]->fActorScale * 100.0f;
			objSize2 = Objects[i]->fActorScale * 50.0f;
			objSize3 = Objects[i]->fActorScale * 50.0f;
		}
		
		fprintf(pFile, "%i,", (i+1));
		fprintf(pFile, "%s,", TCHAR_TO_UTF8(*objType));
		//fprintf(pFile, "sphere,"); // Update to dynamic
		fprintf(pFile, "%0.3f,", objLoc.Y);
		fprintf(pFile, "%0.3f,", objLoc.X);
		fprintf(pFile, "%0.3f,", objLoc.Z);
		fprintf(pFile, "%0.3f,", objRot.Pitch); // y rot
		fprintf(pFile, "%0.3f,", objRot.Roll); // x rot
		fprintf(pFile, "%0.3f,", -objRot.Yaw); // z rot
		fprintf(pFile, "%0.3f,", objSize1); // radius, height (sidelength)
		fprintf(pFile, "%0.3f,", objSize2); // diameter 1 (base) - not required for cube
		fprintf(pFile, "%0.3f,", objSize3);// diameter 2 (point) - not required for cube
		fprintf(pFile, "\n");
	}

	fclose(pFile);
}

