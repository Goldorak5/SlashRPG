// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Hud/InventoryWidget.h"

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASlashHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && SlashOverlayClass && InventoryWidgetClass)
		{
			SlashOverlay = CreateWidget<USlashOverlay>(PlayerController, SlashOverlayClass);
			SlashOverlay->AddToViewport();
// 			InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);
// 			InventoryWidget->AddToViewport();
		}
	}
}
