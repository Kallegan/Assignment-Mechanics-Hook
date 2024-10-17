#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuadLevelMatrixComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MECHANICS_API UQuadLevelMatrixComponent : public UActorComponent
{
	GENERATED_BODY()

public:
float GetMetricDistance() const { return MetricDistance; }
const TArray<AActor*>& GetLinkedActors() const { return LinkedActors; }
bool GetUseAutomaticSelection() const { return bUseAutomaticActorSelection;}
protected:
	UPROPERTY(EditAnywhere, meta=(ForceUnits ="m"))
	float MetricDistance = 10.0f;

	UPROPERTY(EditAnywhere, meta=(EditCondition="!bUseAutomaticActorSelection", EditConditionHides))
	TArray<AActor*> LinkedActors;

	UPROPERTY(EditAnywhere)
	bool bUseAutomaticActorSelection = true;
};
