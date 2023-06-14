// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMesh;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Input)
	UInputMappingContext* BirdmappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

private:
		UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

		UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* Capsule;

		UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* BirdMesh;

		UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* ViewCamera;
};
