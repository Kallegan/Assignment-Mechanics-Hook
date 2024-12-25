// Copyright Epic Games, Inc. All Rights Reserved.


#include "MechanicsPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AMechanicsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	// Register the console command to open a room
	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Room"),
		TEXT("Debug room 0-3"),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &AMechanicsPlayerController::HandleOpenRoomCommand),
		ECVF_Cheat
	);
}

void AMechanicsPlayerController::HandleOpenRoomCommand(const TArray<FString>& Args)
{
	if (Args.Num() == 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Please specify a room name (e.g., 'Room 0')."));
		}
		return;
	}

	FString RoomName = Args[0];
	SpawnRoom(RoomName); // Directly call SpawnRoom
}

void AMechanicsPlayerController::SpawnRoom(const FString& RoomName)
{
	if (RoomName.Equals(TEXT("0"), ESearchCase::IgnoreCase))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Room 0"));
		DebugRoom(0);
	}
	else if (RoomName.Equals(TEXT("1"), ESearchCase::IgnoreCase))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Room 1"));
		DebugRoom(1);
	}
	else if (RoomName.Equals(TEXT("2"), ESearchCase::IgnoreCase))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Room 2"));
		DebugRoom(2);
	}
	else if (RoomName.Equals(TEXT("3"), ESearchCase::IgnoreCase))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Room 3"));
		DebugRoom(3);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid room name! Please specify 'Room 0', 'Room 1', 'Room 2', or 'Room 3'."));
		}
	}
}