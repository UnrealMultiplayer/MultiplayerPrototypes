// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

void UParkourGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Game instance started."));
	HostServer();
}

void UParkourGameInstance::HostServer()
{
	auto subsystem = IOnlineSubsystem::Get();
	auto sessionMngr = subsystem->GetSessionInterface();
	FOnlineSessionSettings settings;
	sessionMngr->OnCreateSessionCompleteDelegates.AddUObject(this, &UParkourGameInstance::SessionCreated);
	bool success = sessionMngr->CreateSession(0, "MyGame", settings);
	UE_LOG(LogTemp, Warning, TEXT("Server hosting success %s."), *FString(success ? "True" : "False"));
}

void UParkourGameInstance::SessionCreated(FName name, bool success)
{
	UE_LOG(LogTemp, Warning, TEXT("Server hosting begun %s with success %s."), *name.ToString(), *FString(success ? "True" : "False"));
}
