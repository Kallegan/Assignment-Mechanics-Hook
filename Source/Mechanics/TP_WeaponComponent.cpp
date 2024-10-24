// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "MechanicsCharacter.h"
#include "MechanicsPlayerState.h"
#include "MechanicsProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"

#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 60.f);
}


void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, [this]()
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			if (PlayerController)
			{
				

				const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
				const FVector SpawnLocation = GetSocketLocation("Muzzle");
				TraceWithBounce(GetSocketLocation("Muzzle"), SpawnRotation);
				
			}
			
		}, 0.01, true); // true to loop the timer

}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Get the PlayerController and PlayerState
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController != nullptr && PlayerController->PlayerState != nullptr)
	{
		// Check if the PlayerState implements the IPlayerStateInterface
		if (PlayerController->PlayerState->GetClass()->ImplementsInterface(UPlayerStateInterface::StaticClass()))
		{
			// Call GetLootedOrbs through the interface
			int LootedOrbs = IPlayerStateInterface::Execute_GetLootedOrbs(PlayerController->PlayerState);

			if (LootedOrbs > 0)
			{
				// Try and fire a projectile
				if (ProjectileClass != nullptr)
				{
					UWorld* const World = GetWorld();
					if (World != nullptr)
					{
						const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
						const FVector SpawnLocation = GetSocketLocation("Muzzle");

						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						World->SpawnActor<AMechanicsProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
						IPlayerStateInterface::Execute_RemoveLootedOrb(PlayerController->PlayerState, 1);
					}
				}

				// Play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
				}

				if (FireAnimation != nullptr)
				{
					UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}
				}
			}
			else
			{
				if (FailedFireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FailedFireSound, Character->GetActorLocation());
				}
			}
		}
	}
}


void UTP_WeaponComponent::AttachWeapon(AMechanicsCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (Character == nullptr || Character->GetHasRifle())
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}



void UTP_WeaponComponent::TraceWithBounce(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	PerformBounceTrace(SpawnLocation, SpawnRotation, TotalRange, MaxBounces);
}

void UTP_WeaponComponent::PerformBounceTrace(const FVector& StartLocation, const FRotator& StartRotation, float RemainingRange, int BounceCount)
{
	if (BounceCount <= 0 )
	{
		return; 
	}

	FVector StartPoint = StartLocation; 
	FVector ForwardVector = StartRotation.Vector(); 
	FVector EndPoint = StartPoint + (ForwardVector * RemainingRange); 

	// Step 2: Setup trace parameters
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;

	TraceParams.AddIgnoredActor(Character); 

	// Step 3: Perform the trace
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECC_Pawn,
		TraceParams
	);

	OnWeaponBeamHit.Broadcast(StartPoint, EndPoint, BounceCount);

	
	if (bHit)
	{
		
		float DistanceTraveled = (HitResult.Location - StartPoint).Size();
		float RemainingBounceRange = RemainingRange - DistanceTraveled;

		// Step 6: Get the hit location and the hit normal
		FVector HitLocation = HitResult.Location + (HitResult.Normal * 1.0f); 

		FVector HitNormal = HitResult.Normal.GetSafeNormal(); 

		// Step 7: Calculate the reflection vector for the bounce
		FVector IncomingVector = (EndPoint - StartPoint).GetSafeNormal();  
		FVector ReflectionVector = FMath::GetReflectionVector(IncomingVector, HitNormal); 

		// Step 8: Ensure the remaining bounce range is valid
		if (RemainingBounceRange > 0.0f)
		{
			// Perform the next bounce trace using the hit location and reflection vector
			PerformBounceTrace(HitLocation, ReflectionVector.Rotation(), RemainingBounceRange, BounceCount - 1);
		}
	}
}
