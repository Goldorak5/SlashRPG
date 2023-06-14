// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "CharacterType.h"
#include "BaseCharacter.h"
#include "Interfaces/PickUpInterface.h"
#include "SlashCharactere.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class ATwoHandedSword;
class AOneHandedSword;
class USlashOverlay;
class ASoul;
class ATreasures;
class AHealthPotion;

UCLASS()
class SLASH_API ASlashCharactere : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	ASlashCharactere();

	/*<AActor>*/
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	/*<AActor>*/

	/* <I_HitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* <I_HitInterface> */

	/* <IPickUpInterface>*/
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasures* Treasures) override;
	virtual void AddHealth(AHealthPotion* Potion) override;
	/* <IPickUpInterface>*/

	UPROPERTY(BlueprintReadOnly)
	bool bCanJump = true;

	UFUNCTION(BlueprintCallable)
	void SetCanJump(bool bCan) { bCanJump = bCan; }
	
protected:
	/*ACharacter*/
	virtual void BeginPlay() override;	
	virtual void Jump() override;
	/*ACharacter*/

	/* Enhanced input action Variables*/
	void AddMappingContext();

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr <UInputMappingContext> SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr <UInputAction> MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr <UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr <UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr <UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr <UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr <UInputAction > DodgeAction;

	/* Call back input */
	virtual void Attack() override;
	void Dodge();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip();

	void EquippingTwoHandSword(ATwoHandedSword*& OverlappingTwoHandSword);

	void EquippingOneHandSword(AOneHandedSword*& OverlappingOneHandSword);

	/* Montage Function */
	void PlayEquipMontage(const FName& SectionName);
	void PlayAttackMontageTwoHand(int32& Select);
	void PlayEquipMontageTwoHand(const FName& SectionName);

	/* <ABaseCharacter> */
	virtual void PlayAttackMontage() override;
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	virtual void Die_Implementation() override;
	/* <ABaseCharacter> */

	UFUNCTION(BlueprintCallable)
	void AttacheWeaponToTheBack();

	UFUNCTION(BlueprintCallable)
	void AttacheWeaponToTheHand();

	UFUNCTION(BlueprintCallable)
	void EquipEnd();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	bool CanArm();
	bool CanDisarm();

private:

	void InitializeOverlay();
	void SetHUDHealth();
	bool IsUnoccupied();
	bool HasEnoughStamina();

	/* Character Components*/
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr < USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr < UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr < UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr < UGroomComponent> Eyesbrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlapingItem;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<ATwoHandedSword> TwoHandSword;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AOneHandedSword> OneHandSword;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TObjectPtr<USlashOverlay> SlashOverlay;
	/* Animation montages */

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UAnimMontage> AttackTwoHandMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UAnimMontage> EquipTwoHandMontage;

	bool IsEquipOneHand = false;
	bool IsEquipedTwoHand = false;

	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	//Variable to select attack
	int32 AttackMontageSelection = 0;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	//FORCEINLINE void SetOverlapingItem(AItem* Item) { OverlapingItem = Item; }

	FORCEINLINE EActionState GetActionState() const {return ActionState; }

	FORCEINLINE bool GetAlive() { return IsAlive(); }
};
