// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ParkourGameMode.h"
#include "ParkourCharacter.h"
#include "UObject/ConstructorHelpers.h"

AParkourGameMode::AParkourGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
