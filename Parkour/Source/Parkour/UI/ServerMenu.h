// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerMenu.generated.h"

class UParkourGameInstance;
/**
 * 
 */
UCLASS()
class PARKOUR_API UServerMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UServerMenu(const FObjectInitializer& ObjectInitializer);

	void AddServer(const FString& ServerName);

	void SetGameInstance(UParkourGameInstance *GameInstance);

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

protected:
	UPROPERTY(BlueprintReadOnly)
	UParkourGameInstance *GameInstance;
	
	UClass* ServerItemClass;
};
