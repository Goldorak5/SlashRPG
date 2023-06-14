// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponents.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponents>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		DirectionnalHitReact(Hitter->GetActorLocation());
	}
	else Die();

	PlayHitSound(ImpactPoint);
	SpawnParticules(ImpactPoint);
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}

void ABaseCharacter::Die_Implementation()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
	SetWeaponCollision(ECollisionEnabled::NoCollision);
}
bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnParticules(const FVector& ImpactPoint)
{
	if (HitParticules && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticules, ImpactPoint);
	}
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceivedDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

int32 ABaseCharacter::PlayRandomMontage(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return - 1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

int32 ABaseCharacter::PlayMontageInOrder(UAnimMontage* Montage, const TArray<FName>& SectionNames, int32& SectionIndex)
{
	if (SectionNames.Num() <= 0) return - 1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	if (SectionIndex < MaxSectionIndex)
	{
		SectionIndex++;
	}
	else SectionIndex = 0;
	PlayMontageSection(Montage, SectionNames[SectionIndex]);
	return SectionIndex;
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();

	if (Animinstance)
	{
		Animinstance->Montage_Stop(.25f, AttackMontage);
	}
}

void ABaseCharacter::PlayDodgeMontage()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Dodge"));
	}
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();

	if (Animinstance)
	{
		Animinstance->Montage_Play(DodgeMontage);
	}
    
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{

	if (CombatTarget == nullptr)return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;

}

FVector ABaseCharacter::GetRotationWrapTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeEnd()
{
}

void ABaseCharacter::PlayAttackMontage()
{
	//default at random
	PlayRandomMontage(AttackMontage, AttackMontageSection);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontage(DeathMontage, DeathMontageSection);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type EnableCollision)
{
	if (EquipWeapon && EquipWeapon->GetWeaponBox())
	{
		EquipWeapon->GetWeaponBox()->SetCollisionEnabled(EnableCollision);
		EquipWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && ReactMontage)
	{
		AnimInstance->Montage_Play(ReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, ReactMontage);
	}
}

void ABaseCharacter::DirectionnalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();

	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	/*Foward* ToHit = | Forward || ToHit | *cos(Theta)
	* Forward = 1 ToHit = 1, So Forward * ToHit = cos(theta)*/
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	//Take the inverse cosine(arc-cosine) of cos(Theta) to get theta(get the angle in radians)
	double Theta = FMath::Acos(CosTheta);
	//convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//if crossproduct is down then Theta negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	//Debug Shape to see 
	/*	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.f, FColor::Green, 3.f);*/

// 	if (GEngine)
// 	{
// 		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Tehta = %f"), Theta));
// 	}
	// 	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Yellow, 3.f);
	// 	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Red, 3.f);

	FName Section("FromBack");
	if (Theta >= -45 && Theta < 45)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= 45 && Theta < 135)
	{
		Section = FName("FromRight");
	}
	else if (Theta >= -135 && Theta < -45)
	{
		Section = FName("FromLeft");
	}

	PlayHitReactMontage(Section);
}