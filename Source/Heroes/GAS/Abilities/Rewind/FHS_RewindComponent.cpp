#include "FHS_RewindComponent.h"

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

void UFHS_RewindComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HeroASC == nullptr || WeaponASC == nullptr)
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

	const auto* WeaponI = Cast<IAbilitySystemInterface>(Hero->GetCurrentWeapon());
	if (WeaponI == nullptr || WeaponI->GetAbilitySystemComponent() == nullptr)
	{
		DestroyComponent();
		return;
	}

	HeroASC = Hero->GetAbilitySystemComponent();
	WeaponASC = WeaponI->GetAbilitySystemComponent();
	CurrentTime = 0.f;
	
	RecordData();
	
} // StartRecording

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::Rewind()
{
	FFHS_RewindData* PastData = RewindData.Peek();

	GetOwner()->SetActorLocation(PastData->Position, false, nullptr, ETeleportType::TeleportPhysics);
	GetOwner()->GetInstigatorController()->SetControlRotation(PastData->ControlRotation);
	
	for (const TPair<FGameplayAttribute, int32>& AttributeCurve : PastData->HeroAttributeCurves)
	{
		HeroASC->SetNumericAttributeBase(AttributeCurve.Key, AttributeCurve.Value);
	}
	for (const TPair<FGameplayAttribute, int32>& AttributeCurve : PastData->WeaponAttributeCurves)
	{
		WeaponASC->SetNumericAttributeBase(AttributeCurve.Key, AttributeCurve.Value);
	}
	
} // Rewind

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_RewindComponent::RecordData()
{
	FFHS_RewindData Data;

	Data.Timestamp = CurrentTime;
	Data.Position = GetOwner()->GetActorLocation();
	Data.ControlRotation = GetOwner()->GetInstigatorController()->GetControlRotation();
	
	const FGameplayAttribute HealthAttribute = UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute();
	Data.HeroAttributeCurves.FindOrAdd(HealthAttribute, HeroASC->GetNumericAttributeBase(HealthAttribute));
	const FGameplayAttribute AmmoAttribute = UFHS_Attributes_Weapon::GetCurrentAmmoAttribute();
	Data.WeaponAttributeCurves.FindOrAdd(AmmoAttribute, WeaponASC->GetNumericAttributeBase(AmmoAttribute));

	RewindData.Enqueue(Data);
	
} // RecordData

// ---------------------------------------------------------------------------------------------------------------------



