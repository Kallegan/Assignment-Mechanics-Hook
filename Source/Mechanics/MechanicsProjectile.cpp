// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechanicsProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMechanicsProjectile::AMechanicsProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMechanicsProjectile::OnHit);
	PrimaryActorTick.bCanEverTick = true;

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMechanicsProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}



void AMechanicsProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Tick is running!")); // Debugging log

	if(bReturningProjectile)
	{
		// Find the player (assumes single-player or first player in a multiplayer game)
		AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0); 
		if (!PlayerPawn) return;  // Check if the player exists

		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		FVector ProjectileLocation = GetActorLocation();

		// Calculate the distance between the projectile and the player
		float Distance = FVector::Dist(PlayerLocation, ProjectileLocation);

		// Define minimum and maximum distances for slowing down
		const float MinDistance = 300.0f;  // Distance at which projectile slows down to minimum speed
		const float MaxDistance = 1000.0f; // Beyond this, the projectile moves at max speed
		const float MinSpeed = 600.0f;     // Minimum speed (when close to the player)
		const float MaxSpeed = 3000.0f;    // Maximum speed (when far from the player)

		// If the projectile is within the range to slow down
		if (Distance <= MaxDistance)
		{
			// Linearly interpolate the speed based on distance
			float SpeedFactor = FMath::Clamp((Distance - MinDistance) / (MaxDistance - MinDistance), 0.0f, 1.0f);
			float NewSpeed = FMath::Lerp(MinSpeed, MaxSpeed, SpeedFactor);

			// Set the new speed to the ProjectileMovement component
			ProjectileMovement->MaxSpeed = NewSpeed;
			ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * NewSpeed;
		}
		else
		{
			// Maintain max speed when beyond MaxDistance
			ProjectileMovement->MaxSpeed = MaxSpeed;
		}
	}
	
}