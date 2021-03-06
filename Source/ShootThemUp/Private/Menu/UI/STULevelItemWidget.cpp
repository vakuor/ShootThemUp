// Shoot Them Up Game. All rights reserved!


#include "Menu/UI/STULevelItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTULevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemUnhovered);
	}
}

void USTULevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}

void USTULevelItemWidget::OnLevelItemHovered()
{
	if(FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void USTULevelItemWidget::OnLevelItemUnhovered()
{
	if(FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;

	if(LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
	}

	if(LevelImage)
	{
		LevelImage->SetBrushFromTexture(Data.LevelThumb);
	}
}

void USTULevelItemWidget::SetSelected(bool IsSelected)
{
	if(FrameImage)
	{
		FrameImage->SetColorAndOpacity(IsSelected ? FLinearColor::Red : FLinearColor::White);
	}
}

