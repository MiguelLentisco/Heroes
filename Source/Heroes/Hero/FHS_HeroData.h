#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "FHS_HeroData.generated.h"

class UFHS_Attributes_CharacterCore;
class UFHS_AbilitySet;

UCLASS()
class HEROES_API UFHS_HeroData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Name.Hero"))
	FGameplayTag HeroName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes)
	TSoftObjectPtr<USkeletalMesh> Hero1PMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes)
	TSoftObjectPtr<USkeletalMesh> Hero3PMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TSoftObjectPtr<UFHS_AbilitySet> AbilitySet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<FAttributeDefaults> Attributes;
	
}; // UFHS_HeroData
