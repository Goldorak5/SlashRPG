// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharactere.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Weapons/OneHanded/OneHandedSword.h"
#include "Items/Weapons/TwoHanded/TwoHandedSword.h"
#include "Animation/AnimMontage.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Components/AttributeComponents.h"
#include "Items/Soul.h"
#include "Items/HealthPotion.h"
#include "Items/Treasures.h"

// Sets default values
ASlashCharactere::ASlashCharactere()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 350.f, 0.f);

	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 200.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyesbrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyesbrows->SetupAttachment(GetMesh());
	Eyesbrows->AttachmentName = FString("head");

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASlashCharactere::BeginPlay()
{
	Super::BeginPlay();

	AddMappingContext();

	Tags.Add(FName("EngageableTarget"));

	InitializeOverlay();
}

void ASlashCharactere::InitializeOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && Attributes)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay)
			{
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}
	}
}

void ASlashCharactere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attributes)
	{
		Attributes->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());

	}
}

void ASlashCharactere::AddMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}

void ASlashCharactere::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancePlayerComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancePlayerComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharactere::Move);
		
		//Looking
		EnhancePlayerComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharactere::Look);

		//jump
		EnhancePlayerComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharactere::Jump);

		//Equip
		EnhancePlayerComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharactere::Equip);
	
		//Attack
		EnhancePlayerComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharactere::Attack);
		
		//Dodge
		EnhancePlayerComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharactere::Dodge);
		}
}

void ASlashCharactere::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	//		first way of doing stuff
	// 		const FVector Forward = GetActorForwardVector();
	// 		AddMovementInput(Forward, MovementVector.Y);
	// 
	// 		const FVector Right = GetActorRightVector();
	// 		AddMovementInput(Right, MovementVector.X);

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotator(0.f, Rotation.Yaw, 0.f);

	if (ActionState != EActionState::EAS_Unoccupied)return;//not moving while attacking
	{
		const FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASlashCharactere::Look(const FInputActionValue& Value)
{
	const FVector2D LookaxisValue = Value.Get<FVector2D>();

	AddControllerPitchInput(LookaxisValue.Y);
	AddControllerYawInput(LookaxisValue.X);
}

void ASlashCharactere::Jump()
{
	if (bCanJump && IsUnoccupied())
	{
		Super::Jump();
		bCanJump = false;
	}
}

void ASlashCharactere::Attack()
{
	if (CanAttack() && IsEquipOneHand)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	else if (CanAttack() && IsEquipedTwoHand)
	{
		PlayAttackMontageTwoHand(AttackMontageSelection);
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharactere::Dodge()
{
	if (!IsUnoccupied() || !HasEnoughStamina())return;
	
	//Immunity When Dodging
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && SlashOverlay)
	{
	Attributes->UseStamina(Attributes->GetDodgeCost());
	SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
	ActionState = EActionState::EAS_Dodge;
	PlayDodgeMontage();
}

void ASlashCharactere::Equip()
{
	ATwoHandedSword* OverlappingTwoHandSword = Cast<ATwoHandedSword>(OverlapingItem);
	AOneHandedSword* OverlappingOneHandSword = Cast<AOneHandedSword>(OverlapingItem);
	
	if (OverlappingOneHandSword)
	{
		if (EquipWeapon)
		{
			EquipWeapon->Destroy();
		}
		EquippingOneHandSword(OverlappingOneHandSword);
	}
	else if (OverlappingTwoHandSword)
	{
		if (EquipWeapon)
		{
			EquipWeapon->Destroy();
		}
		EquippingTwoHandSword(OverlappingTwoHandSword);
	}
	else
	{
		if (CanDisarm())
		{
			if (IsEquipOneHand)
			{
				PlayEquipMontage(FName("Unequip"));
				CharacterState = ECharacterState::ECS_UnEquipped;
				ActionState = EActionState::EAS_Equiping;
			}
			else if (IsEquipedTwoHand)
			{
				PlayEquipMontageTwoHand(FName("Unequip"));
				CharacterState = ECharacterState::ECS_UnEquipped;
				ActionState = EActionState::EAS_Equiping;
			}
		}
		else if (CanArm())
		{
			if (IsEquipOneHand)
			{
				PlayEquipMontage(FName("Equip"));
				CharacterState = ECharacterState::ECS_EquipOneHandWeapon;
				ActionState = EActionState::EAS_Equiping;
			}
			else if(IsEquipedTwoHand)
			{
				PlayEquipMontageTwoHand(FName("Equip"));
				CharacterState = ECharacterState::ECS_EquipTwoHAndWeapon;
				ActionState = EActionState::EAS_Equiping;
			}
		}
	}
}

void ASlashCharactere::EquippingTwoHandSword(ATwoHandedSword*& OverlappingTwoHandSword)
{
	OverlappingTwoHandSword->Equip(GetMesh(), OverlappingTwoHandSword->GetSocketName(), this, this);
	CharacterState = OverlappingTwoHandSword->GetCharacterState();
	OverlapingItem = nullptr;
	EquipWeapon = OverlappingTwoHandSword;
	IsEquipedTwoHand = true;
	IsEquipOneHand = false;
}

void ASlashCharactere::EquippingOneHandSword(AOneHandedSword*& OverlappingOneHandSword)
{
	OverlappingOneHandSword->Equip(GetMesh(), OverlappingOneHandSword->GetSocketName(), this, this);
	CharacterState = OverlappingOneHandSword->GetCharacterState();
	OverlapingItem = nullptr;
	EquipWeapon = OverlappingOneHandSword;
	IsEquipOneHand = true;
	IsEquipedTwoHand = false;
}

void ASlashCharactere::EquipEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharactere::AttacheWeaponToTheBack()
{
	if (EquipWeapon)
	{
		EquipWeapon->AttachingWeapon(GetMesh(), FName("SpineSocket"));
	}
}
void ASlashCharactere::AttacheWeaponToTheHand()
{
	if (IsEquipOneHand && EquipWeapon)
	{
		EquipWeapon->AttachingWeapon(GetMesh(), FName("RightHandSocket"));
	}
	else if (IsEquipedTwoHand && EquipWeapon)
	{
		EquipWeapon->AttachingWeapon(GetMesh(), FName("RightHandTwoHanded"));
	}
}

bool ASlashCharactere::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharactere::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}
	
bool ASlashCharactere::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_UnEquipped;
}

void ASlashCharactere::Die_Implementation()
{
	Super::Die_Implementation();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

void ASlashCharactere::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharactere::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASlashCharactere::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();

	if (Animinstance && EquipMontage)
	{
		Animinstance->Montage_Play(EquipMontage);
		Animinstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharactere::PlayAttackMontageTwoHand(int32& SelectTwoHand)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (SelectTwoHand == 0)
	{
		SelectTwoHand = 1;
	}
	else
		SelectTwoHand = 0;

	if (AnimInstance && AttackTwoHandMontage)
	{
		AnimInstance->Montage_Play(AttackTwoHandMontage);
		//const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (SelectTwoHand)
		{
		case 0:
			SectionName = FName("Attack1");
			//AnimInstance->Montage_JumpToSection(FName("Attack1"),AttackMontage);
			break;
		case 1:
			SectionName = FName("Attack2");
			//AnimInstance->Montage_JumpToSection(FName("Attack2"),AttackMontage);
			break;
		
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackTwoHandMontage);
	}
}

void ASlashCharactere::PlayEquipMontageTwoHand(const FName& SectionName)
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();

	if (Animinstance && EquipTwoHandMontage)
	{
		Animinstance->Montage_Play(EquipTwoHandMontage);
		Animinstance->Montage_JumpToSection(SectionName, EquipTwoHandMontage);
	}
}

void ASlashCharactere::PlayAttackMontage()
{
	PlayMontageInOrder(AttackMontage, AttackMontageSection, AttackMontageSelection);
}

bool ASlashCharactere::CanArm()
{
		return 
			ActionState == EActionState::EAS_Unoccupied && 
			CharacterState == ECharacterState::ECS_UnEquipped &&
			EquipWeapon;
}
bool ASlashCharactere::CanDisarm()
{
	return 
		CharacterState != ECharacterState::ECS_UnEquipped && 
		ActionState == EActionState::EAS_Unoccupied;
}

float ASlashCharactere::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASlashCharactere::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void ASlashCharactere::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent()>0.f)
	{
	ActionState = EActionState::EAS_HitReaction;
	}
}
void ASlashCharactere::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharactere::SetOverlappingItem(AItem* Item)
{
	OverlapingItem = Item;
}

void ASlashCharactere::AddSouls(ASoul* Soul)
{
	if (Attributes && SlashOverlay)
	{
	Attributes->AddSouls(Soul->GetSouls());
	SlashOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ASlashCharactere::AddGold(ATreasures* Treasures) 
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddGold(Treasures->GetGoldValue());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}

void ASlashCharactere::AddHealth(AHealthPotion* Potion)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddHealth(Potion->GetPotionValue());
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}



