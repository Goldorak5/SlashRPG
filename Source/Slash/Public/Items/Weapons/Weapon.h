// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"


class UBoxComponent;
/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	    void Equip(USceneComponent* InParent,FName SocketName, AActor* NewOwner, APawn* NewInstigator);
		void DisableEmbersEffect();
		void DisableSphereCollision();
		void AttachingWeapon(USceneComponent* InParent, const FName& SocketName);
		AWeapon();

		TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = true;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponBox;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USceneComponent> BoxStartTracing;	
		
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USceneComponent> BoxEndTracing;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;


	//Getter and setters
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
