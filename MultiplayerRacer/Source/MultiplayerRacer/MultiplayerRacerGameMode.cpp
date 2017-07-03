// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MultiplayerRacerGameMode.h"
#include "MultiplayerRacerPawn.h"
#include "MultiplayerRacerHud.h"

AMultiplayerRacerGameMode::AMultiplayerRacerGameMode()
{
	DefaultPawnClass = AMultiplayerRacerPawn::StaticClass();
	HUDClass = AMultiplayerRacerHud::StaticClass();
}
