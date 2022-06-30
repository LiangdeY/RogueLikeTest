// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class URadialForceComponent;
class UStaticMeshComponent;

UCLASS()
class ROGUELIKETEST_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp{};

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp{};

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
