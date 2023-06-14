// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

class SLASH_API IPickUpInterface
{
	GENERATED_BODY()

public:

	virtual void SetOverlappingItem(class AItem* Item);
	virtual void AddSouls(class ASoul* Soul);
	virtual void AddGold(class ATreasures* Treasures);
	virtual void AddHealth(class AHealthPotion* Potion);
};
