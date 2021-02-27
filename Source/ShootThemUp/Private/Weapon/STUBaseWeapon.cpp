// Shoot Them Up Game. All rights reserved!


#include "Weapon/STUBaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "STUBaseCharacter.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
	//UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"));
}

void ASTUBaseWeapon::StopFire()
{
	//UE_LOG(LogBaseWeapon, Display, TEXT("Fire stop!"));
}

void ASTUBaseWeapon::MakeShot()
{
	//UE_LOG(LogBaseWeapon, Display, TEXT("MakeShot"));
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if(!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if(!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;//SocketTransform.GetLocation();
	const FVector ShootDirection = ViewRotation.Vector();//SocketTransform.GetRotation().GetForwardVector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if(!GetWorld()) return;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

}

void ASTUBaseWeapon::DecreaseAmmo()
{
	CurrentAmmo.Bullets--;
	LogAmmo();

	if(IsClipEmpty() && !IsAmmoEmpty())
	{
		ChangeClip();
	}
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip()
{
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	if(!CurrentAmmo.Infinite)
	{
		CurrentAmmo.Clips--;
	}
	UE_LOG(LogBaseWeapon, Display, TEXT("----- Change Clip -----"));
}

void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

