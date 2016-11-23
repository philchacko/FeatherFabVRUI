// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SelectableInterface.h"
#include "SelectActor.generated.h"

UCLASS()
class KITTYHAWK_API ASelectActor : public AActor, public ISelectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Upon select.
	virtual void OnUsed(AController * user) override;

	// Upon deselect.
	virtual void Deselect(AController * user) override;

	// Return primitive.
	virtual FString GetPrimitiveType(AController * user) override;

	// Active brush functions.
	virtual void SetActive(AController * user) override;
	virtual void SetInactive(AController * user) override;
	bool bIsActive = false;

	UShapeComponent* TBox;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SM_TBox;
	
	UPROPERTY(EditAnywhere)
		FString PrimitiveType;

	UMaterialInstanceDynamic* DynamicHighlight;
	UMaterialInterface*  MHighlight;
};
