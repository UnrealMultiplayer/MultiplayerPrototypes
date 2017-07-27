// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerItem.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UServerItem::SetName(const FString& Name)
{
	ServerName->Text = FText::FromString(Name);
	JoinButton->OnClicked.AddDynamic(this, &UServerItem::TriggerJoinButton);
}



