// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponents.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponents : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponents();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegenStamina(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:

	//Current health
	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	float MaxHealth;

	//Current Stamina
	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	float DodgeCost = 30.f;

	UPROPERTY(EditAnywhere, Category = "Actors Attributes")
	float StaminaRegenRate = 5.f;

	//Getter and setter
public:
	float GetHealthPercent();
	void ReceivedDamage(float Damage);
	float GetStaminaPercent();
	void UseStamina(float StaminaCost);
	bool IsAlive();
	void AddHealth(float HealthValue);
	void AddGold(int32 GoldPickup);
	void AddSouls(int32 SoulPickUp);
	FORCEINLINE int32 GetSouls() { return Souls; }
	FORCEINLINE int32 GetGold() { return Gold; }
	FORCEINLINE float GetDodgeCost() { return DodgeCost; }
	FORCEINLINE float GetStamina() { return Stamina; }



};
