// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickUpInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


AItem::AItem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = ItemMesh;//Rootcomponent given by AActor default U5

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers Effects"));
	EmbersEffect->SetupAttachment(GetRootComponent());

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformSin()
{
	return Amplitude * FMath::Sin(Runningtime * TimeConstant);
}
float AItem::TransformCos()
{
	return Amplitude * FMath::Cos(Runningtime * TimeConstant);
}

void AItem::SpawnPickUpSystem()
{
	if (PickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickUpEffect, GetActorLocation());
	}
}

void AItem::SoundPickUpSystem()
{
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
	}
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		if (IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor))
		{
			PickupInterface->SetOverlappingItem(this);
		}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor))
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::OveringItem(float Time)
{
	if (ItemState == EItemState::EIS_Hoverring)
	{

		AddActorWorldRotation(FRotator(0.f, RotationSpeed * Time, 0.f));
		AddActorWorldOffset(FVector(0.f, 0.f, TransformSin()));
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Runningtime += DeltaTime;

	OveringItem(DeltaTime);
}

