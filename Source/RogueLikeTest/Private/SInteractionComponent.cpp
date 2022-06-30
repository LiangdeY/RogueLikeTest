// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams QueryParams{};
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // Add a type to collision query

	AActor* MyOwner{ GetOwner() };

	FVector EyeLocation{};
	FRotator EyeRotation{};
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End{ EyeLocation + EyeRotation.Vector() * 1000.f }; //

	//FHitResult Hit{};

	/*  bool bBlockingHit{ GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, QueryParams) }; 
	* Usually too small ( like a pixel ) to detect if the player can interact with an item.
	* 
	* Sweeping in physics term, meaning it takes a sphere, and move it, and detect collisions on its radius.
	*/

	TArray<FHitResult> Hits{};
	FCollisionShape ColShape{};		// Set the shape for sweeping
	float Radius{ 30.f };
	ColShape.SetSphere(Radius); // the radius
	/* 
	* FQuat is a more complex Rotation which contains more information, which is normally used for e.g. Character rotation
	* to prevent a character from getting stuck( after a rotation)
	* FQuat is not exposed to the unreal, so you can't use it in BP
	* FQuat::Identity simply means no rotation
	*/
	bool bBlockingHit{ GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, QueryParams, ColShape) };

	// Color for debug line and sphere
	FColor LineColor{ bBlockingHit ? FColor::Blue : FColor::Red };

	for (FHitResult Hit : Hits)
	{

		if (AActor * HitActor{ Hit.GetActor() })
		{
			/* Return true if interface is implemented
			*  For the interface, you should notice that there are U prefix one and I prefix one
			*  For some reason I don't know yet, to use with the implements, we need the U-prefix one 
			*/
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn{ Cast<APawn>(MyOwner) }; // the Cast in UE is actually safe, returns null pointer if cast fail.
				/* To use the interface function, you need to type  'Execute_' following whatever you function is named.
				* The first param is the object that is going to use the interface, which cannot be null
				* The second one is actually the parameter of our function
				*/ 
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
				// To prevent interacting multiple object at the same time
				break;
			}
		}
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0, 2.f);
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

