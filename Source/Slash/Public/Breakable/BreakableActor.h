// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "BreakableActor.generated.h"


class UGeometryCollectionComponent;
class UCapsuleComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

	
	void SpawningTreasures();
	void SpawningPotion();

protected:

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void BeginPlay() override;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> Capsule;
private:

	UPROPERTY(EditAnywhere, Category = "Breakables Propreties")
	TArray<TSubclassOf<class ATreasures>> TreasureClasses;

	UPROPERTY(EditAnywhere, Category = "Breakables Propreties")
	TArray<TSubclassOf<class AHealthPotion>> PotionClasse;

	bool bBroken = false;

	UFUNCTION()
	void SpawnWhenBreak(const FChaosBreakEvent& BreakEvent);



};
