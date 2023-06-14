// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class USoundBase;
class UNiagaraSystem;

enum class EItemState: uint8
{
	EIS_Hoverring,
	EIS_Equip
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AItem();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Sin Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sin Parameters")
	float TimeConstant = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed = 100.f;

	UFUNCTION(BlueprintPure)
	float TransformSin();

	UFUNCTION(BlueprintPure)
	float TransformCos();

	UFUNCTION()
	virtual void SpawnPickUpSystem();

	UFUNCTION()
	virtual void SoundPickUpSystem();

	template<typename T>
	T Avg(T First, T Second);

	UFUNCTION()
	virtual void OnSphereOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	TObjectPtr<UNiagaraComponent> EmbersEffect;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	TObjectPtr<UNiagaraSystem> PickUpEffect;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> PickUpSound;

	EItemState ItemState = EItemState::EIS_Hoverring;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Runningtime;

	void OveringItem(float Time);

};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
