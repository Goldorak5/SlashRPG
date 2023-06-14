// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API AHealthPotion : public AItem
{
	GENERATED_BODY()
public:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:

	UPROPERTY(EditAnywhere,Category = "Value")
	float PotionValue;

public:
	FORCEINLINE float GetPotionValue() { return PotionValue; }
};
