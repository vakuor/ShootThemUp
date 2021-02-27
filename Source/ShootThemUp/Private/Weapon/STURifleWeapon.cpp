// Shoot Them Up Game. All rights reserved!


#include "Weapon/STURifleWeapon.h"

#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All)
void ASTURifleWeapon::StartFire()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("Fire!"));

	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("Fire stop!"));

	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
	if(!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd)){
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	
	if(HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

		//if( HitResult.Actor.IsValid() && HitResult.Actor->IsA())
		MakeDamage(HitResult);
		UE_LOG(LogRifleWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	} else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}

	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;//SocketTransform.GetLocation();
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);//SocketTransform.GetRotation().GetForwardVector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto Target = HitResult.GetActor();
	if(!Target) return;
	
	Target->TakeDamage(WeaponDamage, FDamageEvent(), GetPlayerController(), this);
		
}
