
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteractData.generated.h"


UCLASS(BlueprintType)
class MECHANICS_API UInteractData : public UDataAsset
{
	GENERATED_BODY()

public:
	UInteractData()
		: InteractTraceRadius(100.f)
		, InteractTraceRange(5000.f) 
		, InteractUpdateTime(0.1f)
		, bShowDebugTrace(false)
	{
	}

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractTraceRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractTraceRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractUpdateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bShowDebugTrace;


};
