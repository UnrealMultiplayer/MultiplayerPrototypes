// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerItem.generated.h"

/**
 * 
 */
UCLASS()
class PARKOUR_API UServerItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetName(const FString& Name);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	
};
