// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "MutableInterface.h"
#include "MyActor.generated.h"

UCLASS()
class KITTYHAWK_API AMyActor : public AActor, public IMutableInterface
{
	GENERATED_BODY()
	
	//UPROPERTY(EditAnywhere, Category=Mesh)
	//class UStaticMeshComponent* spawnMesh;

public:	
	// Sets default values for this actor's properties
	AMyActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Upon highlight/dehighlight.
	virtual void OnHighlight(AController * user) override;
	virtual void DeHighlight(AController * user) override;

	virtual void UpdateMoveLocRot(FVector PivotLoc, FVector PosDelta, FRotator frRotDelta, FQuat fqRotDelta, AController * user) override;
	virtual void SetScale(FVector newScale, AController * user) override;
	virtual FVector GetScale(AController * user) override;
	//virtual void SetRotationWithOffset(FRotator Rotation, FRotator Offset) override;
	virtual FRotator GetRotation() override;

	UShapeComponent* TBox;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SM_TBox;

	UPROPERTY(EditAnywhere)
		FString PrimitiveType;

	//AVRCharacter* MyController;

	void DeletePart();
	//void GetController(AActor* Controller);

	bool bPartIsWithinRange = false;

	float fActorScale = 1.0f;

	UFUNCTION()
		void TriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UMaterialInstanceDynamic* DynamicHighlight;
};
