// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

#include "UI/ServerMenu.h"

const auto SESSION_NAME = "MyGame";

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
	Menu->OnHostServer.AddUObject(this, &UParkourGameInstance::HostServer);
	Menu->OnRefreshServerList.AddUObject(this, &UParkourGameInstance::FindServers);

	Menu->AddToViewport();

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly data;
	data.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	data.SetWidgetToFocus(Menu->TakeWidget());
	PlayerController->SetInputMode(data);

	PlayerController->bShowMouseCursor = true;
}

void UParkourGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Game instance started."));
	auto sessionMngr = GetSession();
	if (!sessionMngr.IsValid()) return;

	sessionMngr->OnCreateSessionCompleteDelegates.AddUObject(this, &UParkourGameInstance::SessionCreated);
	sessionMngr->OnFindSessionsCompleteDelegates.AddUObject(this, &UParkourGameInstance::FindServersFinished);
	sessionMngr->OnDestroySessionCompleteDelegates.AddUObject(this, &UParkourGameInstance::SessionDestroyed);
}

void UParkourGameInstance::HostServer()
{
	auto sessionMngr = GetSession();
	if (!sessionMngr.IsValid()) return;


	auto Session = sessionMngr->GetNamedSession(SESSION_NAME);
	if (Session != nullptr) {
		sessionMngr->DestroySession(SESSION_NAME);
	}
	else 
	{
		CreateSession(SESSION_NAME);
	}
}

void UParkourGameInstance::SessionDestroyed(FName Name, bool Success)
{
	if (Success) {
		CreateSession(Name);
	}
}

void UParkourGameInstance::CreateSession(FName Name)
{
	auto sessionMngr = GetSession();
	if (!sessionMngr.IsValid()) return;

	FOnlineSessionSettings settings;
	settings.NumPublicConnections = 2;
	settings.NumPrivateConnections = 0;
	settings.bShouldAdvertise = true;
	settings.bAllowJoinInProgress = true;
	settings.bIsLANMatch = false;
	settings.bIsDedicated = false;
	settings.bUsesStats = false;
	settings.bAllowInvites = false;
	settings.bUsesPresence = true;
	settings.bAllowJoinViaPresence = true;
	settings.bAllowJoinViaPresenceFriendsOnly = false;
	settings.bAntiCheatProtected = false;
	settings.BuildUniqueId = 342; // not used

	bool success = sessionMngr->CreateSession(0, SESSION_NAME, settings);
}

void UParkourGameInstance::FindServers()
{
	UE_LOG(LogTemp, Warning, TEXT("null menu"));
	if (!ensure(Menu != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Finding servers"));
	auto sessionMngr = GetSession();
	if (!ensure(sessionMngr.IsValid())) return;

	ServerSearch = MakeShareable(new FOnlineSessionSearch());
	ServerSearch->MaxSearchResults = 10;
	ServerSearch->TimeoutInSeconds = 10;
	ServerSearch->bIsLanQuery = false;
	ServerSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	if (!ensure(ServerSearch.IsValid())) return;
	sessionMngr->FindSessions(0, ServerSearch.ToSharedRef());
}

void UParkourGameInstance::FindServersFinished(bool Success)
{
	if (!ensure(ServerSearch.IsValid())) return;
	if (!ensure(Menu != nullptr)) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Finished finding servers %s"), *FString(Success ? "True" : "False")));
	TArray<FString> ServerNames;
	for (auto&& Server : ServerSearch->SearchResults) {
		UE_LOG(LogTemp, Warning, TEXT("Found server"));
		ServerNames.Add(Server.Session.OwningUserName);
	}
	Menu->AddServers(ServerNames);
}

void UParkourGameInstance::SessionCreated(FName name, bool success)
{
	if (!ensure(GEngine != nullptr)) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server hosting begun %s with success %s."), *name.ToString(), *FString(success ? "True" : "False")));

	auto sessionMngr = GetSession();
	if (!ensure(sessionMngr.IsValid())) return;
	//sessionMngr->StartSession(name);
}

IOnlineSessionPtr UParkourGameInstance::GetSession()
{
	auto subsystem = IOnlineSubsystem::Get();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Found subsystem %s."), *subsystem->GetSubsystemName().ToString()));

	if (!ensure(subsystem != nullptr)) return nullptr;
	return subsystem->GetSessionInterface();
}
