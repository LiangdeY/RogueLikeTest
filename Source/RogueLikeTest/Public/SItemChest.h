// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ROGUELIKETEST_API ASItemChest : public AActor ,  public ISGameplayInterface
{
	GENERATED_BODY()

	// '_Implementation' is needed for the 'BlueprintNativeEvent' keyword
	void Interact_Implementation(APawn* InvestigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh{};
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh{};


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
