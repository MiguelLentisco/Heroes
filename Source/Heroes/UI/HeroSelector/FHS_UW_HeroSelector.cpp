#include "FHS_UW_HeroSelector.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Heroes/Hero/FHS_HeroData.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_ButtonDelegateHandler::ButtonClicked()
{
	Cast<UFHS_UW_HeroSelector>(GetOuter())->OnButtonClicked(HeroName);
	
} // ButtonClicked

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_ButtonDelegateHandler::ButtonHovered()
{
	Cast<UFHS_UW_HeroSelector>(GetOuter())->OnButtonHovered(HeroName);
	
} // ButtonHovered

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_ButtonDelegateHandler::ButtonUnhovered()
{
	Cast<UFHS_UW_HeroSelector>(GetOuter())->OnButtonUnhovered(HeroName);
	
} // ButtonUnhovered

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HeroSelector::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
	{
		Create();
	}
	
} // NativePreConstruct

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HeroSelector::NativeConstruct()
{
	Super::NativeConstruct();
	Create();
	
} // NativeConstruct

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HeroSelector::NativeDestruct()
{
	if (HeroPanel != nullptr)
	{
		HeroPanel->ClearChildren();
	}

	if (HeroInfo != nullptr)
	{
		HeroInfo->ClearChildren();
	}
	
	for (UFHS_ButtonDelegateHandler* ButtonDelegate : ButtonDelegates)
	{
		ButtonDelegate->MarkAsGarbage();
	}
	ButtonDelegates.Empty();
	
	Super::NativeDestruct();
	
} // NativeDestruct

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HeroSelector::OnButtonClicked(const FGameplayTag& HeroName)
{
	OnHeroSelected.ExecuteIfBound(HeroesInfoAssets[HeroName].HeroData);
		
} // OnButtonClicked

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HeroSelector::OnButtonHovered(const FGameplayTag& HeroName)
{
	TObjectPtr<UUserWidget>* Widget = HeroesInfoByTag.Find(HeroName);
	if (Widget == nullptr)
	{
		return;
	}

	HeroInfo->SetActiveWidget(*Widget);
	HeroInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
} // OnButtonHovered

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HeroSelector::OnButtonUnhovered(const FGameplayTag& HeroName)
{
	HeroInfo->SetVisibility(ESlateVisibility::Hidden);
	
} // OnButtonUnhovered

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_UW_HeroSelector::Create()
{
	if (HeroPanel == nullptr || HeroInfo == nullptr)
	{
		return;
	}

	HeroPanel->ClearChildren();
	HeroInfo->ClearChildren();
	HeroInfo->SetVisibility(ESlateVisibility::Hidden);

	const int32 NumHeroes = HeroesInfoAssets.Num();
	const int32 PerfectRoot = FMath::CeilToInt32(FMath::Sqrt(static_cast<float>(NumHeroes)));
	const int32 MaxRows = (NumHeroes - 1) / PerfectRoot;

	// Set row/col fills
	for (int32 i = 0; i < PerfectRoot; ++i)
	{
		HeroPanel->SetColumnFill(i, 1.f);
		if ( i <= MaxRows)
		{
			HeroPanel->SetRowFill(i, 1.f);
		}
	}

	TArray<FGameplayTag> HeroNames;
	HeroesInfoAssets.GetKeys(HeroNames);
	
	for (int32 i = 0; i < NumHeroes; ++i)
	{
		const FGameplayTag& HeroName = HeroNames[i];
		TSubclassOf<UUserWidget> HeroInfoDisplayClass = HeroesInfoAssets[HeroName].HeroInfoDisplayWidget.LoadSynchronous();
		TObjectPtr<UFHS_HeroData> HeroData = HeroesInfoAssets[HeroName].HeroData;

		if (HeroData == nullptr || HeroInfoDisplayClass == nullptr)
		{
			continue;
		}

		UUserWidget* HeroInfoWidget = WidgetTree->ConstructWidget<UUserWidget>(HeroInfoDisplayClass);

		// Set grid with heroes name and callbacks
		UButton* Button = WidgetTree->ConstructWidget<UButton>();
		UFHS_ButtonDelegateHandler* ButtonDelegate = NewObject<UFHS_ButtonDelegateHandler>(this);
		
		ButtonDelegate->HeroName = HeroName;
		Button->OnClicked.AddDynamic(ButtonDelegate, &UFHS_ButtonDelegateHandler::ButtonClicked);
		Button->OnHovered.AddDynamic(ButtonDelegate, &UFHS_ButtonDelegateHandler::ButtonHovered);
		Button->OnUnhovered.AddDynamic(ButtonDelegate, &UFHS_ButtonDelegateHandler::ButtonUnhovered);
		ButtonDelegates.Add(ButtonDelegate);

		// Button
		UTextBlock* HeroNameText = WidgetTree->ConstructWidget<UTextBlock>();
		FString AuxLeft, HeroNameShort;
		HeroName.ToString().Split(TEXT("."), &AuxLeft, &HeroNameShort, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		HeroNameText->SetText(FText::FromString(HeroNameShort));
		HeroNameText->SetColorAndOpacity(FLinearColor::Black);
		HeroNameText->SetJustification(ETextJustify::Center);

		UScaleBox* ScaleBox = WidgetTree->ConstructWidget<UScaleBox>();
		ScaleBox->SetContent(HeroNameText);
		
		UPanelSlot* TextSlot = Button->AddChild(ScaleBox);
		HeroPanel->AddChildToGrid(Button, i / PerfectRoot, i % PerfectRoot);

		// Set hero info on switcher
		HeroInfo->AddChild(HeroInfoWidget);
		HeroesInfoByTag.Add(HeroName, HeroInfoWidget);
	}
	
} // Create

// ---------------------------------------------------------------------------------------------------------------------
