// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourGameInstance.h"
#include "OnlineSubsystem.h"

void UParkourGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Game instance started."));
	auto subsystem = IOnlineSubsystem::Get();
	auto sessionMngr = subsystem->GetSessionInterface();
}
