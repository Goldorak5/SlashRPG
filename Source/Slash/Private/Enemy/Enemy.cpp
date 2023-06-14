// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponents.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "HUD/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Soul.h"

AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle(45.f);
	PawnSensing->SightRadius = 2000.f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HideHealthBar();
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(Patroltarget);

	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSee);

	EquipMainWeapon();
	Tags.Add(FName("Enemy"));
}

void AEnemy::EquipMainWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquipWeapon = DefaultWeapon;
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead())return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else 
	{
		CheckPatrolTarget();
	}	
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistancetoTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistancetoTarget <= Radius;
}
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AttackRange);
	EnemyController->MoveTo(MoveRequest);
}
AActor* AEnemy::ChosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != Patroltarget)

			ValidTargets.AddUnique(Target);
	}
	const int32 NumPatrolTargets = ValidTargets.Num();

	if (NumPatrolTargets > 0)
	{
		const int32 RandomTarget = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[RandomTarget];
	}
	return nullptr;
}
//fire frequently
void AEnemy::PawnSee(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn; 
		ClearPatrolTimer();
		StartChasing();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(Patroltarget, PatrolRadius))
	{
		Patroltarget = ChosePatrolTarget();
		const float RandWait = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatroleTimerFinished, RandWait);
	}
}

void AEnemy::PatroleTimerFinished()
{
	MoveToTarget(Patroltarget);
}
void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}
void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}
void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(Patroltarget);
}
void AEnemy::StartChasing()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}
void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}
bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}
bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}
bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}
bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}
bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}
bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}
bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}
void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackWaitMin, AttackWaitMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}
void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged()) StartChasing();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
		ShowHealthBar();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	StopAttackMontage();

	if (IsInsideAttackRadius() && !IsDead())
	{
		CombatTarget = Hitter;
		StartAttackTimer();
	}
}

void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();
	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SpawnASoul();
}

void AEnemy::SpawnASoul()
{
	UWorld* World = GetWorld();
	if (World)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(Attributes->GetSouls());
			SpawnedSoul->SetOwner(this);
		}
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	//if receive damage start chasing
	CombatTarget = EventInstigator->GetPawn();
	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		StartChasing();
	}
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquipWeapon)
	{
		EquipWeapon->Destroy();
	}
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = 
		IsInsideAttackRadius() && 
		!IsAttacking() && 
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}


