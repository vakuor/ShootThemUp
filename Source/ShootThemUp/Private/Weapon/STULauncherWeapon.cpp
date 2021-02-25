// Shoot Them Up Game. All rights reserved!


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
	MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
	const FTransform SpawnTransfrom(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransfrom);
	// set params
	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransfrom);
}
