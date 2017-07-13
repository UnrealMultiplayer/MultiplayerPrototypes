// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerMenu.generated.h"

/**
 * 
 */
UCLASS()
class PARKOUR_API UServerMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Data")
	void AddServer();
};
