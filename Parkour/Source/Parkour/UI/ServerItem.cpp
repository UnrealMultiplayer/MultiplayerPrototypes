// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerItem.h"
#include "Components/TextBlock.h"

void UServerItem::SetName(const FString& Name)
{
	ServerName->Text = FText::FromString(Name);
}



