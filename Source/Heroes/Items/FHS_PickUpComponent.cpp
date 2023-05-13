// Copyright Epic Games, Inc. All Rights Reserved.

#include "FHS_PickUpComponent.h"

#include "GameFramework/Character.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_PickUpComponent::UFHS_PickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SphereRadius = 32.f;
	
} // UFHS_PickUpComponent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddDynamic(this, &UFHS_PickUpComponent::OnSphereBeginOverlap);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RespawnTimer);
	}
	
	Super::EndPlay(EndPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult)
{
	auto* Character = Cast<ACharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}
	
	OnPickUp.Broadcast(Character);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!bHideAndRespawn)
	{
		return;
	}

	GetOwner()->SetActorHiddenInGame(true);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &UFHS_PickUpComponent::RespawnPickup, RespawnTime);
	
} // OnSphereBeginOverlap

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::RespawnPickup()
{
	GetOwner()->SetActorHiddenInGame(false);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
} // RespawnPickup

// ---------------------------------------------------------------------------------------------------------------------

