// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Delegate.h"

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

	virtual bool Initialize() override;

	void AddServer(const FString& ServerName);

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;
	UPROPERTY(meta = (BindWidget))
	class UButton* HostServerButton;

	DECLARE_EVENT(UServerMenu, FOnHostServer);
	FOnHostServer OnHostServer;

protected:
	UFUNCTION()
	void TriggerHostServer();
	
	UClass* ServerItemClass;
};
