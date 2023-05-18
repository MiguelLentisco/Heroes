#include "FHS_RewindComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Heroes/Weapons/FHS_BaseWeapon.h"

// ---------------------------------------------------------------------------------------------------------------------

UFHS_RewindComponent::UFHS_RewindComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
} // UFHS_RewindComponent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HeroASC != nullptr)
	{
		HeroASC->RegisterGameplayTagEvent(TAG_Status_Dead, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HeroASC == nullptr)
	{
		DestroyComponent();
		return;
	}
	
	CurrentTime += DeltaTime;
	while (!RewindData.IsEmpty() && CurrentTime - RewindData.Peek()->Timestamp > RewindTime)
	{
		RewindData.Pop();
	}
	
	RecordData();
	
} // TickComponent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::StartRecording()
{
	PrimaryComponentTick.SetTickFunctionEnable(true);

	const auto* Hero = Cast<AFHS_BaseHero>(GetOwner());
	if (Hero == nullptr || Hero->GetAbilitySystemComponent() == nullptr)
	{
		DestroyComponent();
		return;
	}

	HeroASC = Hero->GetAbilitySystemComponent();
	CurrentTime = 0.f;

	HeroASC->RegisterGameplayTagEvent(TAG_Status_Dead, EGameplayTagEventType::AnyCountChange).AddUObject(
		this, &UFHS_RewindComponent::OnHeroDead);
	
	RecordData();
	
} // StartRecording

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::Rewind()
{
	FFHS_RewindData* PastData = RewindData.Peek();
	if (PastData == nullptr)
	{
		return;
	}

	GetOwner()->SetActorLocation(PastData->Position, false, nullptr, ETeleportType::TeleportPhysics);
	GetOwner()->GetInstigatorController()->SetControlRotation(PastData->ControlRotation);
	
	if (auto* Hero = Cast<ACharacter>(GetOwner()))
	{
		Hero->GetCharacterMovement()->Velocity = PastData->Velocity;
	}
	
	for (const TPair<FGameplayAttribute, int32>& AttributeCurve : PastData->AttributesCurves)
	{
		const float FinalValue = FMath::Max(AttributeCurve.Value, HeroASC->GetNumericAttributeBase(AttributeCurve.Key));
		HeroASC->SetNumericAttributeBase(AttributeCurve.Key, FinalValue);
	}
	
} // Rewind

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::OnHeroDead(const FGameplayTag DeadTag, int32 NumTag)
{
	const bool bDead = NumTag > 0;
	
	SetComponentTickEnabled(bDead);
	if (bDead)
	{
		RewindData.Empty();
	}
	else
	{
		RecordData();
	}
	
} // OnHeroDead

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::RecordData()
{
	FFHS_RewindData Data;

	Data.Timestamp = CurrentTime;
	Data.Position = GetOwner()->GetActorLocation();
	Data.ControlRotation = GetOwner()->GetInstigatorController()->GetControlRotation();
	Data.Velocity = GetOwner()->GetVelocity();
	
	const FGameplayAttribute HealthAttribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	const FGameplayAttribute AmmoAttribute = UFHS_Attributes_Weapon::GetCurrentAmmoAttribute();
	Data.AttributesCurves.FindOrAdd(HealthAttribute, HeroASC->GetNumericAttributeBase(HealthAttribute));
	Data.AttributesCurves.FindOrAdd(AmmoAttribute, HeroASC->GetNumericAttributeBase(AmmoAttribute));

	RewindData.Enqueue(Data);
	
} // RecordData

// ---------------------------------------------------------------------------------------------------------------------



