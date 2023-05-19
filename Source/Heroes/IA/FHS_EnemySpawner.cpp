#include "FHS_EnemySpawner.h"

#include "AbilitySystemComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/Hero/FHS_BaseHero.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_EnemySpawner::Spawn()
{
	const TSubclassOf<AFHS_BaseHero> EnemyClassLoaded = EnemyClass.LoadSynchronous();
	if (EnemyClassLoaded == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	EnemySpawned = GetWorld()->SpawnActor<AFHS_BaseHero>(EnemyClassLoaded, GetOwner()->GetActorLocation(),
	                                                     GetOwner()->GetActorRotation(), SpawnParameters);

	UAbilitySystemComponent* ASC = EnemySpawned->GetAbilitySystemComponent();
	ASC->RegisterGameplayTagEvent(TAG_Status_Dead.GetTag()).AddUObject(this, &UFHS_EnemySpawner::EnemySpawnedDead);
	
} // Spawn

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	Spawn();
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_EnemySpawner::EnemySpawnedDead(FGameplayTag DeadTag, int32 NumCounts)
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UFHS_EnemySpawner::Spawn, TimeToRespawnSinceDeath, false);
	EnemySpawned = nullptr;
	
} // EnemySpawnedDead

// ---------------------------------------------------------------------------------------------------------------------

