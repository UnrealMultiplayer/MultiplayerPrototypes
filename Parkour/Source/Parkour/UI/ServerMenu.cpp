// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerMenu.h"
#include "Components/ScrollBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "../ParkourGameInstance.h"

#include "ServerItem.h"

UServerMenu::UServerMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UServerItem> ServerItem(TEXT("/Game/ThirdPersonCPP/UI/WBP_ServerItem"));
	if (!ensure(ServerItem.Class != nullptr)) return;
	ServerItemClass = ServerItem.Class;
}

void UServerMenu::SetGameInstance(UParkourGameInstance *GameInstance)
{
	this->GameInstance = GameInstance;
}

void UServerMenu::AddServer(const FString& ServerName)
{
	auto Item = CreateWidget<UServerItem>(this->GameInstance, ServerItemClass);
	Item->SetName(ServerName);
	ServerList->AddChild(Item);
}
