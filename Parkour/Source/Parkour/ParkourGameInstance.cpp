// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "UI/ServerMenu.h"

UParkourGameInstance::UParkourGameInstance() {
	// This must be in constructor or will throw
	ConstructorHelpers::FClassFinder<UServerMenu> ServerMenu(TEXT("/Game/ThirdPersonCPP/UI/WBP_ServerMenu"));
	if (!ensure(ServerMenu.Class != nullptr)) return;
	ServerMenuClass = ServerMenu.Class;
}

void UParkourGameInstance::LoadMainMenu()
{
	if (!ensure(ServerMenuClass != nullptr)) return;
	Menu = CreateWidget<UServerMenu>(this, ServerMenuClass);

	if (!ensure(Menu != nullptr)) return;
	Menu->SetGameInstance(this);

	Menu->AddToViewport();

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly data;
	data.SetLockMouseToViewport(false);
	data.SetWidgetToFocus(Menu->TakeWidget());
	PlayerController->SetInputMode(data);

	PlayerController->bShowMouseCursor = true;

	UE_LOG(LogTemp, Warning, TEXT("Get here."))
	FindServers();
}

void UParkourGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Game instance started."));
	auto sessionMngr = GetSession();
	if (!sessionMngr.IsValid()) return;

	sessionMngr->OnCreateSessionCompleteDelegates.AddUObject(this, &UParkourGameInstance::SessionCreated);
	sessionMngr->OnFindSessionsCompleteDelegates.AddUObject(this, &UParkourGameInstance::FindServersFinished);
}

void UParkourGameInstance::HostServer()
{
	auto sessionMngr = GetSession();
	if (!sessionMngr.IsValid()) return;

	FOnlineSessionSettings settings;
	bool success = sessionMngr->CreateSession(0, "MyGame", settings);
	UE_LOG(LogTemp, Warning, TEXT("Server hosting success n %s."), *FString(success ? "True" : "False"));
}

void UParkourGameInstance::FindServers()
{
	UE_LOG(LogTemp, Warning, TEXT("null menu"));
	if (!ensure(Menu != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Finding servers"));
	auto sessionMngr = GetSession();
	if (!ensure(sessionMngr.IsValid())) return;

	ServerSearch = MakeShareable(new FOnlineSessionSearch());
	if (!ensure(ServerSearch.IsValid())) return;
	sessionMngr->FindSessions(0, ServerSearch.ToSharedRef());
}

void UParkourGameInstance::FindServersFinished(bool Success)
{
	if (!ensure(ServerSearch.IsValid())) return;
	if (!ensure(Menu != nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("Finished finding servers"));
	for (auto&& server : ServerSearch->SearchResults) {
		UE_LOG(LogTemp, Warning, TEXT("Found server"));
		Menu->AddServer(server.GetSessionIdStr());
	}

	auto World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->GetTimerManager().SetTimer(ServerCheckTimer, this, &UParkourGameInstance::FindServers, 2, false);
}

void UParkourGameInstance::SessionCreated(FName name, bool success)
{
	UE_LOG(LogTemp, Warning, TEXT("Server hosting begun %s with success %s."), *name.ToString(), *FString(success ? "True" : "False"));
}

IOnlineSessionPtr UParkourGameInstance::GetSession()
{
	auto subsystem = IOnlineSubsystem::Get();
	if (!ensure(subsystem != nullptr)) return nullptr;
	return subsystem->GetSessionInterface();
}
