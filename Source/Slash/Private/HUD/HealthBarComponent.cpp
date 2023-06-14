// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	//only do cast  once
	if (HealthBarWidgetComponent == nullptr)
	{
		HealthBarWidgetComponent = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidgetComponent && HealthBarWidgetComponent->HealthBar)
	{
		HealthBarWidgetComponent->HealthBar->SetPercent(Percent);
	}
}
