// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

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
	void UnloadMainMenu();
	
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HostServer();

	void FindServers();

private:
	void SessionCreated(FName name, bool success);
	void FindServersFinished(bool Success);
	void SessionDestroyed(FName Name, bool Success);
	void CreateSession(FName Name);
	void JoinServer(uint32 ServerNumber);
	void FinishJoin(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	IOnlineSessionPtr GetSession();

	UPROPERTY()
	UClass *ServerMenuClass;

	UPROPERTY()
	class UServerMenu *Menu;

	struct FTimerHandle ServerCheckTimer;
	TSharedPtr<FOnlineSessionSearch> ServerSearch = nullptr;
};
