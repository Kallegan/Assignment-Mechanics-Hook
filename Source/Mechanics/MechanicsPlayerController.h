// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MechanicsPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class MECHANICS_API AMechanicsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface


public:
	// Function to spawn a room based on the room name
	UFUNCTION(BlueprintCallable)
	void SpawnRoom(const FString& RoomName);

	UFUNCTION(BlueprintImplementableEvent)
	void DebugRoom(int32 RoomIndex);

	// Function to handle the console command input
	void HandleOpenRoomCommand(const TArray<FString>& Args);
};
