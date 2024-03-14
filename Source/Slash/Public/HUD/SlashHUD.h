// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

class USlashOverlay;
class UInventoryWidget;

UCLASS()
class SLASH_API ASlashHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay()override;
	virtual void PreInitializeComponents()override;
private:

	UPROPERTY(EditDefaultsOnly, Category = Slash)
	TSubclassOf<USlashOverlay> SlashOverlayClass;

		UPROPERTY(EditDefaultsOnly, Category = Slash)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<USlashOverlay> SlashOverlay;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
public:
	FORCEINLINE USlashOverlay* GetSlashOverlay() const { return SlashOverlay; }
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
};
