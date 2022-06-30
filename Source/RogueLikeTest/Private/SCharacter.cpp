// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "SInteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter() 
	: InteractComp{ CreateDefaultSubobject<USInteractionComponent>("InteractComp") }
	, SpringArmComp{ CreateDefaultSubobject<USpringArmComponent>("SpringArmComp") }
	, CameraComp{ CreateDefaultSubobject<UCameraComponent>("CameraComp") }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);

	SpringArmComp->bUsePawnControlRotation = true; // So that the Pawn( mainly the camera) rotate along with the controller
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}

/* When MoveForward, we want to move to the camera direction, not the pawn forward direction
*/
void ASCharacter::MoveForward(float Value)
{
	FRotator ControllerRot{ GetControlRotation() };
	// Don't really care about Pitch and Roll
	ControllerRot.Pitch = 0.f;
	ControllerRot.Roll = 0.f;

	AddMovementInput(ControllerRot.Vector(), Value);
	//AddMovementInput(GetActorForwardVector(), Value);
}

void ASCharacter::MoveRight(float Value)
{

	FRotator ControllerRot{ GetControlRotation() };
	// Don't really care about Pitch and Roll
	ControllerRot.Pitch = 0.f;
	ControllerRot.Roll = 0.f;

	/* When we move right, we want the pawn to move to, the right of the Camera, not the Pawn
	*  Without this line of code, the Pawn will just turning in circle, because its own Yaw is keep increasing by itself.
	*/
	FVector RightVector{ FRotationMatrix(ControllerRot).GetScaledAxis(EAxis::Y) };


	AddMovementInput(RightVector	, Value);
}

void ASCharacter::PrimaryAttack()
{
	FVector HandLocation{ GetMesh()->GetSocketLocation("hand_rSocket") };

	FTransform SpawnTM{ FTransform(GetShootingDirection(), HandLocation) };

	FActorSpawnParameters SpawnParms{};
	SpawnParms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParms.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParms);
}
// Q
void ASCharacter::SecondaryAttack()
{

}

void ASCharacter::PrimaryInteract()
{
	if(ensure(InteractComp)) InteractComp->PrimaryInteract();
}

FRotator ASCharacter::GetShootingDirection()
{
	if (ensure(GetController()))
	{
		// Random value to define the range of shooting
		float ShootingRange{ 1000.f };

		FVector HandLocation{ GetMesh()->GetSocketLocation("hand_rSocket") };

		// Out parameters
		FVector Olocation{};
		FRotator Orotation{};
		// This function use the camera for player and use the character's eyes for AI 
		GetController()->GetPlayerViewPoint(Olocation, Orotation);

		FVector End{ Olocation + Orotation.Vector() * ShootingRange };
		FHitResult Hit{};
		FCollisionQueryParams CollisionParams{};
		CollisionParams.AddIgnoredActor(this);
		

		DrawDebugLine(GetWorld(), Olocation, End, FColor::Blue, false, 2.f, 2, 2.0f);
		GetWorld()->LineTraceSingleByProfile(Hit, Olocation, End, TEXT("Projectile"), CollisionParams);
		if (Hit.GetActor())
			return (Hit.Location - HandLocation).Rotation();
		else
			return (End - HandLocation).Rotation();
	}
	return { 0.f, 0.f, 0.f };

}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); 
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput); 

	// IE_Pressed: Trigger when pressed
	// Basic Attack
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	// E : Blackhole
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

}

