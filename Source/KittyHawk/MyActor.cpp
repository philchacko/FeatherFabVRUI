// Fill out your copyright notice in the Description page of Project Settings.

#include "KittyHawk.h"
#include "MyActor.h"
#include "stdio.h"
#include "ConstructorHelpers.h"
#include "engine.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//spawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaterialSphere"));
	//RootComponent = spawnMesh;

	TBox = CreateDefaultSubobject<USphereComponent>(TEXT("Box"));
	TBox->bGenerateOverlapEvents = true;
	TBox->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::TriggerEnter);
	TBox->OnComponentEndOverlap.AddDynamic(this, &AMyActor::TriggerExit);
	RootComponent = TBox;

	FString MeshRef;

	//printf("Spawn Primitive Type: %s\n", TCHAR_TO_UTF8(*PrimitiveType));
	if (PrimitiveType == "cube") {
		MeshRef = "StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'";
	}
	else if (PrimitiveType == "cone") {
		MeshRef = "StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'";
	}
	else if (PrimitiveType == "cylinder") {
		MeshRef = "StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'";
	}
	else if (PrimitiveType == "colbert") {
		MeshRef = "StaticMesh'/Game/Colbert_HighRes_Smile.Colbert_HighRes_Smile'";
	}
	else {
		MeshRef = "StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'";
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMShape(*MeshRef);

	SM_TBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	if (SMShape.Succeeded()) {
		SM_TBox->SetStaticMesh(SMShape.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MatMaster(TEXT("Material'/Game/MutableMat.MutableMat'"));
	if (MatMaster.Succeeded()) {
		SM_TBox->SetMaterial(0, MatMaster.Object);
		/*DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Yellow);
		printf("Set as yellow.\n");
		SM_TBox->SetMaterial(0, DynamicHighlight);*/
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Did not find material instance.\n"));
	}

	SM_TBox->AttachTo(RootComponent);
}

/*void AMyActor::GetController(AActor* Controller)
{
	MyController = Cast<AVRCharacter>(Controller);
}*/

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyActor::DeletePart()
{
	UE_LOG(LogTemp, Warning, TEXT("You deleted a part.\n"));
	Destroy();
}

// Called every frame
void AMyActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/*if (MyController != NULL) // Nullcheck motion controller
	{
		if (MyController->bIsDestroyingPart && bPartIsWithinRange)
		{
			DeletePart();
		}
	}*/
}

void AMyActor::OnHighlight(AController * user)
{
	// Do stuff.

	//if (bIsActive) return;

	DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicHighlight) {
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Yellow);
		//printf("Color select.\n");
		SM_TBox->SetMaterial(0, DynamicHighlight);
	}

	//SM_TBox->SetMaterial(0, MHighlight);
}

void AMyActor::DeHighlight(AController * user)
{

	DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicHighlight) {
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Gray);
		SM_TBox->SetMaterial(0, DynamicHighlight);
	}
}

void AMyActor::UpdateMoveLocRot(FVector PivotLoc, FVector PosDelta, FRotator frRotDelta, FQuat fqRotDelta, AController * user) {
	
	FVector fvRelativeStartLoc = GetActorLocation() - PivotLoc;
	float distance = fvRelativeStartLoc.Size();
	FVector fvRelativeNewLoc = distance * (fvRelativeStartLoc.Rotation() + frRotDelta).Vector();

	FVector fvNewLoc = GetActorLocation() + PosDelta + fvRelativeNewLoc - fvRelativeStartLoc;
	//FRotator frNewRot = GetActorRotation() + RotDelta;
	//frNewRot.Normalize();
	FQuat fqNewRot = GetActorQuat() * fqRotDelta;

	SetActorLocation(fvNewLoc);
	AddActorWorldRotation(fqRotDelta);
	//AddActorWorldRotation(RotDelta);
	//SetActorRotation(frNewRot);
}

void AMyActor::SetScale(FVector newScale, AController * user) {

	//FVector actorScale = GetActorScale() * ScaleMult;
	SetActorScale3D(newScale);
	fActorScale = newScale.X;
	//printf("Set scale: %0.2f\n", GetActorScale().X);
}

FVector AMyActor::GetScale(AController * user) {
	return GetActorScale();
}
/*
void AMyActor::SetRotationWithOffset(FRotator Rotation, FRotator Offset) {
	FRotator SetRotation = Rotation - Offset;
	SetActorRotation(SetRotation);
}*/

FRotator AMyActor::GetRotation() {
	return GetActorRotation();

}

void AMyActor::TriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bPartIsWithinRange = true;
	//printf("Trigger enter.\n");
//	GetController(OtherActor);
}

void AMyActor::TriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bPartIsWithinRange = false;
	//printf("Trigger exit.\n");

	//TODO: Ensure booleans are being updated properly, as well as motion controller locations relative to HMD.
}
