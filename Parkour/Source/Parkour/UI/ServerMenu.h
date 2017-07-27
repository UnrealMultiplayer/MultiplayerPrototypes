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

	void ClearServers();
	void AddServer(const FString& ServerName);

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;
	UPROPERTY(meta = (BindWidget))
	class UButton* HostServerButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshServerList;

	DECLARE_EVENT(UServerMenu, FOnHostServer);
	FOnHostServer OnHostServer;

	DECLARE_EVENT(UServerMenu, FOnRefreshServerList);
	FOnRefreshServerList OnRefreshServerList;

	DECLARE_EVENT_OneParam(UServerMenu, FOnJoinServer, uint32);
	FOnJoinServer OnJoinServer;

protected:
	UFUNCTION()
	void TriggerHostServer() {
		OnHostServer.Broadcast();
	}

	UFUNCTION()
	void TriggerRefreshServerList() {
		OnRefreshServerList.Broadcast();
	}

	UClass* ServerItemClass;

	uint32 ServerCount = 0;
};
