#pragma once

class STUUtils
{
public:
	template <typename T>
	static T* GetSTUPlayerComponent(APawn* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		return Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass()));
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
