#include "FHS_GA_SuperSpeed.h"

#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"
#include "Heroes/GAS/Effects/FHS_GE_SuperSpeed.h"

// ---------------------------------------------------------------------------------------------------------------------

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Name_Ability_Ultimate_SuperSpeed, TEXT("Name.Ability.Ultimate.SuperSpeed"));

// ---------------------------------------------------------------------------------------------------------------------

UFHS_GA_SuperSpeed::UFHS_GA_SuperSpeed()
{
	AbilityTags.AddTag(TAG_Name_Ability_Ultimate_SuperSpeed.GetTag());
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	CancelAbilitiesWithTag.AddTag(TAG_Status_AbilityCancelable.GetTag());
	ActivationBlockedTags.AddTag(TAG_Status_Stunned.GetTag());
	ActivationBlockedTags.AddTag(TAG_Status_Dead.GetTag());

	// Cost
	FGameplayModifierInfo Cost;
	Cost.ModifierOp = EGameplayModOp::Additive;
	Cost.Attribute = UFHS_Attributes_CharacterCore::GetCurrentUltimatePowerAttribute();
	
	FAttributeBasedFloat MaxPowerCost;
	MaxPowerCost.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(
		UFHS_Attributes_CharacterCore::GetMaxUltimatePowerAttribute(), EGameplayEffectAttributeCaptureSource::Source,
		false);
	MaxPowerCost.Coefficient = -1.f;

	Cost.ModifierMagnitude = MaxPowerCost;
	AttributeCosts = { Cost };
	
} // UFHS_GA_SuperSpeed

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_GA_SuperSpeed::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UFHS_GE_SuperSpeed::StaticClass(), 1,
	                                                                   ASC->MakeEffectContext());
	const int32 AbilityLevel = GetAbilityLevel(Handle, ActorInfo);
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Duration, Time.GetValueAtLevel(AbilityLevel));
	SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Speed, Speed.GetValueAtLevel(AbilityLevel));
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	ASC->ExecuteGameplayCue(GCTag, ASC->MakeEffectContext());
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
} // ActivateAbility

// ---------------------------------------------------------------------------------------------------------------------
