// Shoot Them Up Game. All rights reserved!


#include "Pickups/STUHealthPickup.h"


#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	//UE_LOG(LogHealthPickup, Display, TEXT("Health was taken"));
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if(!HealthComponent) return false;
	
	return HealthComponent->TryToAddHealth(HealthAmount);
}
