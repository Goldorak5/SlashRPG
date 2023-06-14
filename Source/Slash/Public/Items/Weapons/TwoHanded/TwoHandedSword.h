// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon.h"
#include "Characters/CharacterType.h"
#include "TwoHandedSword.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ATwoHandedSword : public AWeapon
{
	GENERATED_BODY()
public:

	FName SocketName = FName("RightHandTwoHanded");
	ECharacterState CharacterState = ECharacterState::ECS_EquipTwoHAndWeapon;

public:
	FORCEINLINE FName GetSocketName() { return SocketName; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

};
