// Fill out your copyright notice in the Description page of Project Settings.


#include "QuadLevelMatrixComponentVisualizer.h"
#include "Mechanics/Components/QuadLevelMatrixComponent.h"

void FQuadLevelMatrixComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                            FPrimitiveDrawInterface* PDI)
{
	const UQuadLevelMatrixComponent* LevelMatrixComponent = Cast<UQuadLevelMatrixComponent>(Component);
	if (!LevelMatrixComponent)
	{
		return;
	}

	const AActor* OwnerActor = Component->GetOwner();
	const FVector OwnerLocation = OwnerActor->GetActorLocation();
	const float MetricsDistance = LevelMatrixComponent->GetMetricDistance() * 100.f;

	TArray<AActor*> ActorsToCheck;

	if(LevelMatrixComponent->GetUseAutomaticSelection())
	{
		for (TObjectIterator<UQuadLevelMatrixComponent> MatrixComponentIterator; MatrixComponentIterator; ++MatrixComponentIterator)
		{
			if (*MatrixComponentIterator == LevelMatrixComponent || LevelMatrixComponent->GetWorld() != OwnerActor->GetWorld())
			{
				continue;
			}

			AActor* CurrentOwnerActor = MatrixComponentIterator->GetOwner();
			if(OwnerActor->GetDistanceTo(CurrentOwnerActor) <= MetricsDistance * 1.5f)
			{
				ActorsToCheck.Add(CurrentOwnerActor);
			}
		}
	}
	else
	{
		ActorsToCheck = LevelMatrixComponent->GetLinkedActors();
	}

	for (const AActor* LinkedActor : ActorsToCheck)
	{
		if(LinkedActor)
		{
			const float DistanceToActor = LinkedActor->GetDistanceTo(OwnerActor);
			const FVector LinkedActorLocation = LinkedActor->GetActorLocation();

			FLinearColor Color = FLinearColor::Green;

			if (DistanceToActor > MetricsDistance)
			{
				Color = FLinearColor::Red;

				const FVector VectorTowardsLinkedActor = (LinkedActorLocation - OwnerLocation).GetSafeNormal();
				const FVector MetricsStopLocation = VectorTowardsLinkedActor * MetricsDistance;
				PDI->DrawPoint(OwnerLocation + MetricsStopLocation, FLinearColor::Yellow, 10.0f, SDPG_Foreground);
			}

			PDI->DrawLine(OwnerLocation, LinkedActorLocation, Color, SDPG_Foreground, 2.0f);
		}
	}
}
