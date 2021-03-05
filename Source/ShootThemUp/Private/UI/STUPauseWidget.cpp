// Shoot Them Up Game. All rights reserved!


#include "UI/STUPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void USTUPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
	}
}

void USTUPauseWidget::OnClearPause()
{
	UWorld* World = GetWorld();
	if(!World || !World->GetAuthGameMode()) return;

	World->GetAuthGameMode()->ClearPause();
}
