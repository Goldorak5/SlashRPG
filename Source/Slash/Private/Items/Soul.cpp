// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickUpInterface.h"
#include "Kismet/KismetSystemLibrary.h"



void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const double ActorLocationZ = GetActorLocation().Z;
	if (DesiredZ < ActorLocationZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
	AddActorWorldOffset(DeltaLocation);
	}



}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor))
	{
		PickupInterface->AddSouls(this);
	SpawnPickUpSystem();
	SoundPickUpSystem();
	Destroy();
	}

}

void ASoul::BeginPlay()
{
	Super::BeginPlay();

	LineTraceToFloor();

}

void ASoul::LineTraceToFloor()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectHit;
	ObjectHit.Add(EObjectTypeQuery::ObjectTypeQuery1);

	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(GetOwner());
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectHit,
		false,
		ActorToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true);
	DesiredZ = HitResult.ImpactPoint.Z + SoulHeight;
}
