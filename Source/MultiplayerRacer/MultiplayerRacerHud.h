// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiplayerRacerHud.generated.h"

UCLASS(config = Game)
class AMultiplayerRacerHud : public AHUD
{
	GENERATED_BODY()

public:
	AMultiplayerRacerHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
