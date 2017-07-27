// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourGameInstance.h"
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
	Menu->OnJoinServer.AddUObject(this, &UParkourGameInstance::JoinServer);

	Menu->AddToViewport();

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly data;
	data.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	data.SetWidgetToFocus(Menu->TakeWidget());
	PlayerController->SetInputMode(data);

	PlayerController->bShowMouseCursor = true;
}

void UParkourGameInstance::UnloadMainMenu()
{
	Menu->RemoveFromViewport();

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly data; //NB: Different type here.
	PlayerController->SetInputMode(data);

	PlayerController->bShowMouseCursor = false;
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
	sessionMngr->OnJoinSessionCompleteDelegates.AddUObject(this, &UParkourGameInstance::FinishJoin);
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

void UParkourGameInstance::JoinServer(uint32 ServerNumber)
{
	if (!ensure(GEngine != nullptr)) return;
	auto sessionMngr = GetSession();
	if (!ensure(sessionMngr.IsValid())) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Join server number %d"), ServerNumber));

	auto Server = ServerSearch->SearchResults[ServerNumber];
	auto SessionName = FName("MyGame");
	sessionMngr->JoinSession(0, SessionName, Server);

}

void UParkourGameInstance::FinishJoin(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	auto sessionMngr = GetSession();
	if (!ensure(sessionMngr.IsValid())) return;
	auto PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join for reason %d"), (uint32) Result); //NB: Displaying as decimal so won't be human readable
		return;
	}

	FString ConnectionInfo;
	bool Success = sessionMngr->GetResolvedConnectString(SessionName, ConnectionInfo);
	if (!Success) return;

	UE_LOG(LogTemp, Verbose, TEXT("Travelling to client"), (uint32)Result);

	UnloadMainMenu();

	PlayerController->ClientTravel(ConnectionInfo, TRAVEL_Absolute);

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
	Menu->ClearServers();
	for (auto&& Server : ServerSearch->SearchResults) {
		UE_LOG(LogTemp, Warning, TEXT("Found server"));
		Menu->AddServer(Server.Session.OwningUserName);
	}
}

void UParkourGameInstance::SessionCreated(FName name, bool success)
{
	if (!ensure(GEngine != nullptr)) return;
	auto sessionMngr = GetSession();
	if (!ensure(sessionMngr.IsValid())) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server hosting begun %s with success %s."), *name.ToString(), *FString(success ? "True" : "False")));
	
	sessionMngr->StartSession(name);

	UnloadMainMenu();

	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

IOnlineSessionPtr UParkourGameInstance::GetSession()
{
	auto subsystem = IOnlineSubsystem::Get();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Found subsystem %s."), *subsystem->GetSubsystemName().ToString()));

	if (!ensure(subsystem != nullptr)) return nullptr;
	return subsystem->GetSessionInterface();
}
