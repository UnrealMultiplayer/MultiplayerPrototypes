// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ParkourGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PARKOUR_API UParkourGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UParkourGameInstance();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void LoadMainMenu();

	virtual void Init() override;
	
	void HostServer();

private:
	void SessionCreated(FName name, bool success);

	UClass *ServerMenuClass = nullptr;
};
