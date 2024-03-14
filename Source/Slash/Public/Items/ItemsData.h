// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterType.h"
#include "Engine/UserDefinedStruct.h"
#include "ItemsData.generated.h"

UCLASS()
class SLASH_API UItemsData : public UUserDefinedStruct
{
	GENERATED_BODY()
	
	public:

	EItemRarety itemRarety = EItemRarety::EIR_Normal;

	EItemType itemType = EItemType::EIT_Potion;

	



};
