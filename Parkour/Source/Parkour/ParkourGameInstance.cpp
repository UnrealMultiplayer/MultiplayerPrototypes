// Fill out your copyright notice in the Description page of Project Settings.

#include "ParkourGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "UI/ServerMenu.h"

UParkourGameInstance::UParkourGameInstance() {
	// This must be in constructor or will throw
	ConstructorHelpers::FClassFinder<UServerMenu> ServerMenu(TEXT("/Game/ThirdPersonCPP/UI/WBP_ServerMenu"));
	if (ServerMenu.Class == nullptr) return;
	ServerMenuClass = ServerMenu.Class;
}

void UParkourGameInstance::LoadMainMenu()
{
	if (ServerMenuClass == nullptr) return;
	Menu = CreateWidget<UServerMenu>(this, ServerMenuClass);

	if (Menu == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't create widget."))
		return;
	}
	Menu->AddToViewport();

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly data;
	data.SetLockMouseToViewport(false);
	data.SetWidgetToFocus(Menu->TakeWidget());
	PlayerController->SetInputMode(data);

	PlayerController->bShowMouseCursor = true;

	Menu->AddServer();
	Menu->AddServer();
	Menu->AddServer();
}

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
	UE_LOG(LogTemp, Warning, TEXT("Server hosting success n %s."), *FString(success ? "True" : "False"));
	TSharedRef<FOnlineSessionSearch> search(new FOnlineSessionSearch());
	//sessionMngr->OnFindSessionsCompleteDelegates.AddUObject(this, &UParkourGameInstance::SessionCreated);
	sessionMngr->FindSessions(0, search);
}

void UParkourGameInstance::SessionCreated(FName name, bool success)
{
	UE_LOG(LogTemp, Warning, TEXT("Server hosting begun %s with success %s."), *name.ToString(), *FString(success ? "True" : "False"));
}
