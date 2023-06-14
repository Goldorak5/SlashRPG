// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapons/Weapon.h"
#include "Characters/SlashCharactere.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"



AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(FName("Weapon Box") );
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);

	BoxStartTracing = CreateDefaultSubobject<USceneComponent>(FName("Start Tracing"));
	BoxStartTracing->SetupAttachment(GetRootComponent());

	BoxEndTracing = CreateDefaultSubobject<USceneComponent>(FName("End Tracing"));
	BoxEndTracing->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equip;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachingWeapon(InParent,SocketName);
	DisableSphereCollision();
	DisableEmbersEffect();
}

void AWeapon::DisableEmbersEffect()
{
	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();
	}
}
void AWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachingWeapon(USceneComponent* InParent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, SocketName);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (ActorIsSameType(OtherActor))return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor()) 
	{
		if (ActorIsSameType(BoxHit.GetActor()))return;

		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass() // basic damage without creating a class of damage
		);
		ExecuteGetHit(BoxHit);
		CreateField(BoxHit.ImpactPoint);
	}
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxStartTracing->GetComponentLocation();
	const FVector End = BoxEndTracing->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxStartTracing->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,//default 5 seconds
		BoxHit,
		true);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}



