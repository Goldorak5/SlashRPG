// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon.h"
#include "Characters/CharacterType.h"
#include "OneHandedSword.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API AOneHandedSword : public AWeapon
{
	GENERATED_BODY()
public:

	const FName SocketName = FName("RightHandSocket");
	const ECharacterState CharacterState = ECharacterState::ECS_EquipOneHandWeapon;

public:
	FORCEINLINE FName GetSocketName() { return SocketName; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
