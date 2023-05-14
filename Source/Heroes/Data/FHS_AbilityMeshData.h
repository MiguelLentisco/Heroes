#pragma once

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "FHS_AbilityMeshData.generated.h"

class UFHS_AbilitySet;
class UFHS_AbilitySystemComponent;

UCLASS()
class UFHS_AbilityMeshData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Categories = "Name"))
	FGameplayTag Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes)
	TSoftObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<FAttributeDefaults> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TSoftObjectPtr<UFHS_AbilitySet> AbilitySet;
	
	void SetupInput(UFHS_AbilitySystemComponent* ASC, APawn* Pawn);
	void ClearInput(APawn* Pawn);
	
}; // UFHS_AbilityMeshData
