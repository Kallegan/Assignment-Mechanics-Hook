#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestBlueprintableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UTestBlueprintableInterface : public UInterface
{
    GENERATED_BODY()
};


class MECHANICS_API ITestBlueprintableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
    float GetActorSpeed() const;
};