#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UPlayerInterface : public UInterface
{
    GENERATED_BODY()
};


class MECHANICS_API IPlayerInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
    float GetActorSpeed() const;
};