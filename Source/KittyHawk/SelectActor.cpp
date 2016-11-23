// Fill out your copyright notice in the Description page of Project Settings.

#include "KittyHawk.h"
#include "SelectActor.h"
#include "stdio.h"
#include "ConstructorHelpers.h"
#include "engine.h"

// Sets default values
ASelectActor::ASelectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TBox = CreateDefaultSubobject<USphereComponent>(TEXT("Box"));
	TBox->bGenerateOverlapEvents = true;
	RootComponent = TBox;

	FString MeshRef;

	//printf("Select Primitive Type: %s\n", TCHAR_TO_UTF8(*PrimitiveType));
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
	else if (PrimitiveType == "collar") {
		MeshRef = "StaticMesh'/Game/marussia.marussia'";
	}
	else {
		MeshRef = "StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'";
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SMSphere(*MeshRef);


	SM_TBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	//SM_TBox->SetMobility(EComponentMobility::Movable);
	if (SMSphere.Succeeded()) {
		SM_TBox->SetStaticMesh(SMSphere.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> SelectMatMaster(TEXT("Material'/Game/SelectMatMaster.SelectMatMaster'"));
	if (SelectMatMaster.Succeeded()) {
		SM_TBox->SetMaterial(0, SelectMatMaster.Object);
		/*DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Yellow);
		printf("Set as yellow.\n");
		SM_TBox->SetMaterial(0, DynamicHighlight);*/
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Did not find material instance.\n"));
	}
	SM_TBox->AttachTo(RootComponent);

	//static ConstructorHelpers::FObjectFinder<UMaterial> Material_Gold(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));
	//if (Material_Gold.Succeeded())
	//	DynamicHighlight = UMaterialInstanceDynamic::Create(Material_Gold.Object, this);

	UE_LOG(LogTemp, Warning, TEXT("My Actor created."));
}

// Called when the game starts or when spawned
void ASelectActor::BeginPlay()
{
	Super::BeginPlay();

	DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicHighlight) {
		if (PrimitiveType == "sphere") {
			DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Blue);
			bIsActive = true;
		}
		else
			DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Yellow);
		SM_TBox->SetMaterial(0, DynamicHighlight);
	}

}

// Called every frame
void ASelectActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASelectActor::OnUsed(AController * user)
{
	if (bIsActive) return;

	//UMaterialInstanceDynamic* Dynmat = UMaterialInstanceDynamic::Create(SM_TBox->GetMaterial(0), this);
	//Dynmat->SetVectorParameterValue("BaseColor", FLinearColor::Blue);
	//SM_TBox->SetMaterial(0, Dynmat);

	DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicHighlight) {
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Green);
		//printf("Color select.\n");
		SM_TBox->SetMaterial(0, DynamicHighlight);
	}
	//printf("New color!\n");

	//SM_TBox->SetMaterial(0, MHighlight);
}

void ASelectActor::Deselect(AController * user)
{
	if (bIsActive) return;

	//UMaterialInstanceDynamic* Dynmat = UMaterialInstanceDynamic::Create(SM_TBox->GetMaterial(0), this);
	//Dynmat->SetVectorParameterValue("BaseColor", FLinearColor::Blue);
	//SM_TBox->SetMaterial(0, Dynmat);

	DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicHighlight) {
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Yellow);
		//printf("Color deselect.\n");
		SM_TBox->SetMaterial(0, DynamicHighlight); 
	}
	//printf(("Hit primitive: %s\n"), TCHAR_TO_UTF8(*PrimitiveType));

	//SM_TBox->SetMaterial(0, MHighlight);
}

FString ASelectActor::GetPrimitiveType(AController * user)
{
	return PrimitiveType;
}

void ASelectActor::SetActive(AController * user)
{
	if (bIsActive) return;
	// Need to somehow initialize sphere as "LastActive"
	bIsActive = true;

	DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicHighlight) {
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Blue);
		//printf("Color deselect.\n");
		SM_TBox->SetMaterial(0, DynamicHighlight);
	}
}

void ASelectActor::SetInactive(AController * user)
{
	bIsActive = false;

	DynamicHighlight = SM_TBox->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicHighlight) {
		DynamicHighlight->SetVectorParameterValue("BaseColor", FLinearColor::Yellow);
		//printf("Color deselect.\n");
		SM_TBox->SetMaterial(0, DynamicHighlight);
	}
}

