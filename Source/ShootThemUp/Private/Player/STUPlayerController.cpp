// Shoot Them Up Game. All rights reserved!


#include "Player/STUPlayerController.h"

#include "STUGameModeBase.h"
#include "Components/STURespawnComponent.h"
#include "GameFramework/GameModeBase.h"

ASTUPlayerController::ASTUPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(GetWorld())
	{
		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
		}
	}
}

void ASTUPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	OnNewPawn.Broadcast(aPawn);
}

void ASTUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
}

void ASTUPlayerController::OnPauseGame()
{
	UWorld* World = GetWorld();
	if(!World || !World->GetAuthGameMode()) return;
	World->GetAuthGameMode()->SetPause(this);
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
	if(State == ESTUMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}
