// Shoot Them Up Game. All rights reserved!


#include "STUPlayerCharacter.h"

#include "Components/STUCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit): Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASTUPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MouseUp", this, &ASTUPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("MouseRight", this, &ASTUPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartSprint);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnEndSprint);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Released, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Released, WeaponComponent, &USTUWeaponComponent::Reload);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ASTUPlayerCharacter::SwitchCamera);
}

void ASTUPlayerCharacter::LookUp(float Amount)
{
	//UE_LOG(LogBaseCharacter, Display, TEXT("Pitch amount: %.2f"), Amount);
	AddControllerPitchInput(Amount);
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//todo: убрать
	CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//todo: убрать
	CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
	const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (auto MeshChild: MeshChildren)
	{
		const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if(MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh);
		}
	}
}

void ASTUPlayerCharacter::SwitchCamera()
{
	if(CameraComponent && SpringArmComponent && HealthComponent && !HealthComponent->IsDead())
	{
		
		if(IsFirstPerson)
		{
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
			CameraComponent->AttachToComponent(SpringArmComponent, AttachmentRules);
			CameraComponent->bUsePawnControlRotation = false;
		}
		else
		{
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld, false);
			CameraComponent->AttachToComponent(GetMesh(), AttachmentRules, "HeadSocket");
			//GetMesh()->SkeletalMesh->Skeleton->GetRig().Nodes[0]
			CameraComponent->bUsePawnControlRotation = true;
			//CameraComponent->SetupAttachment(GetMesh());
		}
		IsFirstPerson = !IsFirstPerson;
	}
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::OnStartSprint()
{
	//CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
	WantsToRun = true;
}

void ASTUPlayerCharacter::OnEndSprint()
{
	//CharacterMovementComponent->MaxWalkSpeed = 600;
	WantsToRun = false;
}

bool ASTUPlayerCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	
}
