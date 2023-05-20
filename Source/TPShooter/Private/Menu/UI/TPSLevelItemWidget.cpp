// Third Player Shooter, All Rights Reserved


#include "Menu/UI/TPSLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTPSLevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;

	if (LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
	}

	if (LevelImage)
	{
		LevelImage->SetBrushFromTexture(Data.LevelThumb);
	}
}

void UTPSLevelItemWidget::SetSelected(bool IsSelected)
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UTPSLevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &UTPSLevelItemWidget::OnLevelItemClickedHandle);
		LevelSelectButton->OnHovered.AddDynamic(this, &UTPSLevelItemWidget::OnLevelItemHoveredHandle);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &UTPSLevelItemWidget::OnLevelItemUnhoveredHandle);
	}
}

void UTPSLevelItemWidget::OnLevelItemClickedHandle()
{
	OnLevelSelected.Broadcast(LevelData);
}

void UTPSLevelItemWidget::OnLevelItemHoveredHandle()
{
	if (LevelImage)
	{
		FLinearColor Yellow = FLinearColor::FromSRGBColor(FColor(255, 200, 0, 255));
		LevelImage->SetColorAndOpacity(Yellow);
	}
}

void UTPSLevelItemWidget::OnLevelItemUnhoveredHandle()
{
	if (LevelImage)
	{
		LevelImage->SetColorAndOpacity(FLinearColor::White);
	}
}
