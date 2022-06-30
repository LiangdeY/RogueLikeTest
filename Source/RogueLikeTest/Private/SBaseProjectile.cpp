// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASBaseProjectile::ASBaseProjectile()
	: SphereComp{ CreateDefaultSubobject<USphereComponent>("SphereComp") }
	, MovementComp{ CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp") }
	, ParticleSysComp{ CreateDefaultSubobject<UParticleSystemComponent>("ParticleSysComp") }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Projectile"); // use the collision profile set in the Engine
	ParticleSysComp->SetupAttachment(SphereComp);

	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

}

// Called when the game starts or when spawned
void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

