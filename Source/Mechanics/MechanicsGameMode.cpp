// Copyright Epic Games, Inc. All Rights Reserved.

#include "MechanicsGameMode.h"
#include "MechanicsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMechanicsGameMode::AMechanicsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
