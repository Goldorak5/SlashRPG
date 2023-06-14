// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetGold(int32 gold);
	void SetSouls(int32 Souls);

private:

	UPROPERTY ( meta = (BindWidget) ) 
	TObjectPtr<UProgressBar> HealthProgressbar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaProgressbar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SoulsText;

};
