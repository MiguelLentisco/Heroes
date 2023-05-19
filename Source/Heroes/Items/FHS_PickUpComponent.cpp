#include "FHS_PickUpComponent.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_PickUpComponent::UFHS_PickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SphereRadius = 32.f;
	
} // UFHS_PickUpComponent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetIsReplicated(true);
	
} // PostInitProperties

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
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

void UFHS_PickUpComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UFHS_PickUpComponent, bPickedUp, COND_None);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::OnRep_bPickedUp()
{
	if (!bHideAndRespawn)
	{
		return;
	}
	
	if (bPickedUp)
	{
		GetOwner()->SetActorHiddenInGame(true);
		SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		GetOwner()->SetActorHiddenInGame(false);
		SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	
} // OnRep_bPickedUp

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

	bPickedUp = true;
	OnPickUp.Broadcast(Character);

	if (!bHideAndRespawn)
	{
		return;
	}

	OnRep_bPickedUp();
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &UFHS_PickUpComponent::RespawnPickup, RespawnTime);
	
} // OnSphereBeginOverlap

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_PickUpComponent::RespawnPickup()
{
	bPickedUp = false;
	OnRep_bPickedUp();
	
} // RespawnPickup

// ---------------------------------------------------------------------------------------------------------------------

