// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasures.h"
#include "Items/HealthPotion.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "Components/CapsuleComponent.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}


void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();

	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::SpawnWhenBreak);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken)return;
	if (TreasureClasses.Num() > 0)
	{
    SpawningTreasures();
	}
	else SpawningPotion();
	
}

void ABreakableActor::SpawnWhenBreak(const FChaosBreakEvent& BreakEvent)
{
	if (bBroken == false)
	{
		SpawningTreasures();
	}
}

void ABreakableActor::SpawningTreasures()
{
	bBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		const int32 SelectTreasure = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasures>(TreasureClasses[SelectTreasure], Location, GetActorRotation());
		/*GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Treasure"));*/
	}
}

void ABreakableActor::SpawningPotion()
{
	bBroken = true;
	UWorld* World = GetWorld();
	if (World && PotionClasse.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		const int32 SelectPotion = FMath::RandRange(0, PotionClasse.Num() - 1);
		World->SpawnActor<AHealthPotion>(PotionClasse[SelectPotion], Location, GetActorRotation());
		/*GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Potion"));*/
	}
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

