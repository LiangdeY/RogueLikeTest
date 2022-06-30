// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
	: MeshComp{ CreateDefaultSubobject<UStaticMeshComponent>("MeshComp")}
	, ForceComp{ CreateDefaultSubobject<URadialForceComponent>("ForceComp")}

{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);
	

	ForceComp->SetupAttachment(MeshComp);
	ForceComp->Radius = 750.f;
	ForceComp->ImpulseStrength = 2500.f;
	// Optional,ignore 'Mass' of other objects ( if false, a stronger impulse is needed to push objects with great mass)
	ForceComp->bImpulseVelChange = true;
	// Optional, by default, the radial force component will generate a little force to the actor around it. Details could be found in the tick component of it 
	ForceComp->SetAutoActivate(false);
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);

	UE_LOG(LogTemp, Warning, TEXT("PostInitializeComponents"));
}
void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
	UE_LOG(LogTemp, Warning, TEXT("FireImpulse"));
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

