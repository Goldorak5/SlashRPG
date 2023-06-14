// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"



UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void BeginPlay() override;
	void LineTraceToFloor();
private:

	UPROPERTY(EditAnywhere, Category = "Soul Proprety")
	int32 Souls;

	double DesiredZ;

	UPROPERTY(EditAnywhere, Category = Location)
	double SoulHeight = 50.f;

	UPROPERTY(EditAnywhere, Category = Location)
	float DriftRate = -15.f;

public:
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumOfSouls) { Souls = NumOfSouls; }
};

  