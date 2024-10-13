#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MechanicsPlayerState.generated.h"

UINTERFACE(BlueprintType)
class UPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlayerStateInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Loot")
	int GetLootedOrbs() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Loot")
	void RemoveLootedOrb(int Count);
};

UCLASS()
class MECHANICS_API AMechanicsPlayerState : public APlayerState, public IPlayerStateInterface
{
	GENERATED_BODY()

public:
	AMechanicsPlayerState()
		: LootedOrbs(0)
	{
	}

	virtual int GetLootedOrbs_Implementation() const override
	{
		return LootedOrbs;
	}

	virtual void RemoveLootedOrb_Implementation(int Count) override
	{
		if (LootedOrbs >= Count)
		{
			LootedOrbs -= Count;
		}
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loot")
	int LootedOrbs;
};
