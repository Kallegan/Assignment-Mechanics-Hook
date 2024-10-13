// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MechanicsPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MECHANICS_API AMechanicsPlayerState : public APlayerState
{
	GENERATED_BODY()
	

	AMechanicsPlayerState()
		: LootedOrbs(0)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite)
	int LootedOrbs;
};
