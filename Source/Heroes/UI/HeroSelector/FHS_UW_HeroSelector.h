#pragma once

#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "FHS_UW_HeroSelector.generated.h"

class UWidgetSwitcher;
class UFHS_HeroData;
class UGridPanel;

USTRUCT(BlueprintType)
struct HEROES_API FFHS_HeroInfoAssets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFHS_HeroData> HeroData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> HeroInfoDisplayWidget;
	
}; // FFHS_HeroInfo

UCLASS()
class  UFHS_ButtonDelegateHandler : public UObject
{
	GENERATED_BODY()

public:
	FGameplayTag HeroName;

	UFUNCTION()
	void ButtonClicked();
	UFUNCTION()
	void ButtonHovered();
	UFUNCTION()
	void ButtonUnhovered();
	
}; // UFHS_UW_HeroSelector

DECLARE_DELEGATE_OneParam(FOnHeroSelected, UFHS_HeroData*);

UCLASS()
class HEROES_API UFHS_UW_HeroSelector : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnHeroSelected OnHeroSelected;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void OnButtonClicked(const FGameplayTag& HeroName);
	void OnButtonHovered(const FGameplayTag& HeroName);
	void OnButtonUnhovered(const FGameplayTag& HeroName);

protected:
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UGridPanel> HeroPanel;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> HeroInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Displays = "Name.Hero"))
	TMap<FGameplayTag, FFHS_HeroInfoAssets> HeroesInfoAssets;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFHS_HeroData> SelectedHero;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UUserWidget>> HeroesInfoByTag;

	UPROPERTY()
	TArray<TObjectPtr<UFHS_ButtonDelegateHandler>> ButtonDelegates;

	void Create();
	
}; // UFHS_UW_HeroSelector
