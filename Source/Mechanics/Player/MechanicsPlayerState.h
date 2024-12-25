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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Loot")
	void AddLootedOrb(int Count);

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrbCountChanged, int, NewCount);

UCLASS()
class MECHANICS_API AMechanicsPlayerState : public APlayerState, public IPlayerStateInterface
{
	GENERATED_BODY()

public:
	AMechanicsPlayerState()
		: LootedOrbs(0)
		, LootedOrbsMax(3)
	{
	}

	virtual int GetLootedOrbs_Implementation() const override
	{
		return LootedOrbs;
	}

	virtual void AddLootedOrb_Implementation(int Count) override;
	virtual void RemoveLootedOrb_Implementation(int Count) override;
	

	UPROPERTY(BlueprintAssignable, Category = "Loot")
	FOnOrbCountChanged OnOrbCountChanged;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loot")
	int LootedOrbs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loot")
	int LootedOrbsMax;
};
