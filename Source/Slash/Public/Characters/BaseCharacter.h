// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterType.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponents;
class UAnimMontage;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
    ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	/* Combat */
	virtual void Attack();

	UFUNCTION(BlueprintNativeEvent)
	void Die();

	virtual bool CanAttack();
	bool IsAlive();
	virtual void HandleDamage(float DamageAmount);
	void DisableCapsule();
	void DirectionnalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnParticules(const FVector& ImpactPoint);
	void DisableMeshCollision();

    UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type EnableCollision);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	/* Combat Variables */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> EquipWeapon;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance;

	/* Montage Function */
    void PlayHitReactMontage(const FName& SectionName);
	virtual void PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	int32 PlayMontageInOrder(UAnimMontage* Montage, const TArray<FName>& SectionNames,int32& SectionIndex);
	void StopAttackMontage();
	virtual void PlayDodgeMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWrapTarget();

	/* Montages Variables */
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> AttackMontageSection;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponents> Attributes;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

private:

	/*Montage Section*/
	void PlayMontageSection(UAnimMontage* AnimMontage, const FName& SectionName);
	int32 PlayRandomMontage(UAnimMontage* Montage,const TArray<FName>& SectionNames);

	/* Montage Variables */
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> ReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> DeathMontageSection;

	/* Component */
	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = VFX)
	TObjectPtr<UParticleSystem> HitParticules;

public:

	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

};
