// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressbar)
	{
		HealthProgressbar->SetPercent(Percent);
	}
}

void USlashOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressbar)
	{
		StaminaProgressbar->SetPercent(Percent);
	}
}

void USlashOverlay::SetGold(int32 gold)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), gold)));
	}
}

void USlashOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
