// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharactere.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharactere>(TryGetPawnOwner());

	if (SlashCharacter)
	{
		SlashCharacterMove = SlashCharacter->GetCharacterMovement();
	}

}

void USlashAnimInstance::NativeUpdateAnimation(float deltatime)
{
	Super::NativeUpdateAnimation(deltatime);

	if (SlashCharacterMove)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMove->Velocity);
		IsFalling = SlashCharacterMove->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState();
		ActionState = SlashCharacter->GetActionState();
		DeathPose = SlashCharacter->GetDeathPose();
		}
}


