// Shoot Them Up Game. All rights reserved!

#pragma once
#include "Player/STUPlayerState.h"

class STUUtils
{
public:
	template <typename T>
	static T* GetSTUPlayerComponent(AActor* PlayerActor)
	{
		if (!PlayerActor) return nullptr;

		return Cast<T>(PlayerActor->GetComponentByClass(T::StaticClass()));
	}

	static bool AttachActorToSocket(AActor* Actor, USceneComponent* SceneComponentTo,
                                               const FName& SocketName)
	{
		if (!Actor || !SceneComponentTo) return false;
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Actor->AttachToComponent(SceneComponentTo, AttachmentRules, SocketName);
		return true;
	}

	static bool AreEnemies(AController* Controller1, AController* Controller2)
	{
		if(!Controller1 || !Controller2 || Controller1 == Controller2) return false;

		const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
		const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}
};

// From STUPlayerHUDWidget

/*
USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
{
const auto Player = GetOwningPlayerPawn();
if(!Player) return nullptr;

const auto HealthComponent = Cast<USTUHealthComponent>(Player->GetComponentByClass(USTUHealthComponent::StaticClass()));
return HealthComponent;
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
const auto Player = GetOwningPlayerPawn();
if(!Player) return nullptr;

const auto WeaponComponent = Cast<USTUWeaponComponent>(Player->GetComponentByClass(USTUWeaponComponent::StaticClass()));
return WeaponComponent;
}*/
