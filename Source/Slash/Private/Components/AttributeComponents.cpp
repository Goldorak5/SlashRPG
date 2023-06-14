//// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponents.h"


UAttributeComponents::UAttributeComponents()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponents::BeginPlay()
{
	Super::BeginPlay();
}

float UAttributeComponents::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponents::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

void UAttributeComponents::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

void UAttributeComponents::ReceivedDamage(float Damage)
{
	//Clamp allow the variable to stay in a limit
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

bool UAttributeComponents::IsAlive()
{
	return Health > 0.f;
}

void UAttributeComponents::AddHealth(float HealthValue)
{
	Health = FMath::Clamp(Health + HealthValue, 0.f, MaxHealth);
}

void UAttributeComponents::AddGold(int32 GoldPickup)
{
	Gold += GoldPickup;
}

void UAttributeComponents::AddSouls(int32 SoulPickUp)
{
	Souls += SoulPickUp;
}

void UAttributeComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponents::RegenStamina(float DeltaTime)
{
	//StaminaRegeneration per second
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}
