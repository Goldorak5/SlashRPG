// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasures.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ATreasures : public AItem
{
	GENERATED_BODY()
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(EditAnywhere, Category = "Treasure Propreties")
	int32 Gold;

public:
	FORCEINLINE int32 GetGoldValue() const { return Gold; }
};
