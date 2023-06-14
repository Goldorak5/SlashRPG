// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasures.h"
#include "Interfaces/PickUpInterface.h"
#include "Kismet/GameplayStatics.h"

void ATreasures::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor))
	{
		
		PickupInterface->AddGold(this);
		SoundPickUpSystem();
		Destroy();
	}
   
// 	if (GEngine)
// 	{
// 		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, FString::Printf(TEXT("%d"), GetGold()));
// 	}
}
