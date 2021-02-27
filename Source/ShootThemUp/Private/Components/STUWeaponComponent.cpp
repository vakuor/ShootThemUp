// Shoot Them Up Game. All rights reserved!

#include "Components/STUWeaponComponent.h"


#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::SpawnWeapons()
{
	if (!GetWorld()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	for (auto WeaponData : WeaponDataArray)
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent,
                                               const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
		return;
	}
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponDataArray[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponDataArray.FindByPredicate([&](const FWeaponData& Data)
	{
		return Data.WeaponClass == CurrentWeapon->GetClass();
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	bEquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire()
{
	if(!CanFire()) return;
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
	if(!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}

	for(auto WeaponData : WeaponDataArray)
	{
		auto ReloadFinishedNotify = FindNotifyByClass<USTUReloadFinishedAnimNotify>(WeaponData.ReloadAnimMontage);
		if(!ReloadFinishedNotify) continue;
		ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
		//break; //TODO: needed?
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComponent) return;

	bEquipAnimInProgress = false;
	UE_LOG(LogWeaponComponent, Display, TEXT("Switch Anim"));
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComponent) return;

	bReloadAnimInProgress = false;
	UE_LOG(LogWeaponComponent, Display, TEXT("Reload Anim"));
}

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}


bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipAnimInProgress  && !bReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}
void USTUWeaponComponent::Reload()
{
	if(!CanReload()) return;
	bReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}
