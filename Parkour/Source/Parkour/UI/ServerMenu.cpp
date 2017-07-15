// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerMenu.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "ServerItem.h"

UServerMenu::UServerMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UServerItem> ServerItem(TEXT("/Game/ThirdPersonCPP/UI/WBP_ServerItem"));
	if (!ensure(ServerItem.Class != nullptr)) return;
	ServerItemClass = ServerItem.Class;
}

bool UServerMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) {
			return false;
	}
	if (!ensure(HostServerButton != nullptr)) return false;
	HostServerButton->OnClicked.AddDynamic(this, &UServerMenu::TriggerHostServer);
	return true;
}

void UServerMenu::TriggerHostServer()
{
	OnHostServer.Broadcast();
}

void UServerMenu::AddServer(const FString& ServerName)
{
	if (!ensure(GetWorld() != nullptr)) return;
	auto Item = CreateWidget<UServerItem>(GetWorld(), ServerItemClass);
	Item->SetName(ServerName);
	ServerList->AddChild(Item);
}
