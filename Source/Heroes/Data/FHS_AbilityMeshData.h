#pragma once

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "FHS_AbilityMeshData.generated.h"

class UFHS_AbilitySet;
class UFHS_AbilitySystemComponent;

UCLASS()
class HEROES_API UFHS_AbilityMeshData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Name"))
	FGameplayTag Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<FAttributeDefaults> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TSoftObjectPtr<UFHS_AbilitySet> AbilitySet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TArray<TSoftClassPtr<UGameplayEffect>> InitialEffects;
	
	void SetupInput(UFHS_AbilitySystemComponent* ASC, APawn* Pawn);
	void ClearInput(UFHS_AbilitySystemComponent* ASC, APawn* Pawn);
	
}; // UFHS_AbilityMeshData
