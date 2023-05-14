#include "FHS_BaseWeapon.h"

#include "Heroes/Data/FHS_AbilityMeshData.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/Hero/FHS_BaseHero.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_BaseWeapon::AFHS_BaseWeapon() : MuzzleOffset(FVector(100.f, .0f, 10.f))
{
	bReplicates = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	ASC = CreateDefaultSubobject<UFHS_AbilitySystemComponent>(TEXT("GAS"));
	ASC->SetIsReplicated(true);
	
} // AFHS_BaseWeapon

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetWeaponData(UFHS_AbilityMeshData* NewData)
{
	if (NewData == nullptr || NewData == WeaponData)
	{
		return;
	}
	
	WeaponData = NewData;
	ASC->SetupMeshData(WeaponData);
	Mesh->SetSkeletalMesh(WeaponData->Mesh.LoadSynchronous());
	SetupInput();
	
} // SetWeaponData

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::AttachToHero(AFHS_BaseHero* Hero)
{
	if (Hero == nullptr)
	{
		return;
	}
	
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Hero->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	Hero->SetHasRifle(true);

	
	/*// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent:: Fire);
		}
	}*/
	
} // AttachToHero

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetupInput()
{
	if (WeaponData == nullptr || ASC == nullptr || HeroOwner == nullptr)
	{
		return;
	}

	WeaponData->SetupInput(ASC, HeroOwner);
	
} // SetupGAS

// ---------------------------------------------------------------------------------------------------------------------
