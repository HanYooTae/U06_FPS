#include "FP_FirstPersonCharacter.h"
#include "Global.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CBullet.h"
#include "Game/CPlayerState.h"

#define COLLISION_WEAPON		ECC_GameTraceChannel1

AFP_FirstPersonCharacter::AFP_FirstPersonCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(44.f, 88.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 64.f));
	Camera->bUsePawnControlRotation = true;
	
	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterFP_Mesh"));
	FP_Mesh->SetOnlyOwnerSee(true);
	FP_Mesh->SetupAttachment(Camera);
	FP_Mesh->bCastDynamicShadow = false;
	FP_Mesh->CastShadow = false;

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(FP_Mesh, TEXT("GripPoint"));

	WeaponRange = 5000.0f;
	WeaponDamage = 10.0f;

	GetMesh()->SetOwnerNoSee(true);
	CHelpers::CreateSceneComponent(this, &TP_Gun, "TP_Gun", GetMesh());
	TP_Gun->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	TP_Gun->SetOwnerNoSee(true);

	CHelpers::CreateSceneComponent(this, &FP_GunshotParticle, "FP_GunshotParticle", FP_Gun);
	FP_GunshotParticle->SetupAttachment(FP_Gun, "Muzzle");
	FP_GunshotParticle->SetOnlyOwnerSee(true);
	FP_GunshotParticle->bAutoActivate = false;

	CHelpers::CreateSceneComponent(this, &TP_GunshotParticle, "TP_GunshotParticle", TP_Gun);
	TP_GunshotParticle->SetupAttachment(TP_Gun, "Muzzle");
	TP_GunshotParticle->SetOwnerNoSee(true);
	TP_GunshotParticle->bAutoActivate = false;
}

ACPlayerState* AFP_FirstPersonCharacter::GetSelfPlayerState()
{
	if (SelfPlayerState == nullptr)
		SelfPlayerState = Cast<ACPlayerState>(GetPlayerState());

	return SelfPlayerState;
}

void AFP_FirstPersonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//GetPlayerState();
	SelfPlayerState = Cast<ACPlayerState>(NewController->PlayerState);

	if (HasAuthority() && !!SelfPlayerState)
		SelfPlayerState->Health = 100.f;
}

void AFP_FirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority() == false)
		SetTeamColor(CurrentTeam);
}

void AFP_FirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFP_FirstPersonCharacter::OnFire);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AFP_FirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFP_FirstPersonCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFP_FirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFP_FirstPersonCharacter::LookUpAtRate);
}

void AFP_FirstPersonCharacter::OnFire()
{
	if (FireAnimation != NULL)
	{
		UAnimInstance* AnimInstance = FP_Mesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	if (!!FP_GunshotParticle)
		FP_GunshotParticle->Activate(true);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	FVector ShootDir = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;

	if (PlayerController)
	{
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(StartTrace, CamRot);
		ShootDir = CamRot.Vector();

		StartTrace = StartTrace + ShootDir * ((GetActorLocation() - StartTrace) | ShootDir);
	}

	const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

	OnFire_Server(StartTrace, EndTrace);
}

void AFP_FirstPersonCharacter::OnFire_Server_Implementation(const FVector& LineStart, const FVector& LineEnd)
{
	WeaponTrace(LineStart, LineEnd);
	FireEffect();
}

void AFP_FirstPersonCharacter::FireEffect_Implementation()
{
	// Play TP Fire Montage
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	CheckNull(animInstance);

	CheckNull(TP_FireAnimation);
	animInstance->Montage_Play(TP_FireAnimation);

	// Play Gun Sound
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Play TP_Gunshot Particle
	if (!!TP_GunshotParticle)
		TP_GunshotParticle->Activate(true);

	// Spawn Bullet
	UWorld* world = GetWorld();
	if (!!world)
		world->SpawnActor<ACBullet>(ACBullet::StaticClass(), FP_Gun->GetSocketLocation("Muzzle"), FP_Gun->GetSocketRotation("Muzzle"));
}

void AFP_FirstPersonCharacter::PlayDead_Implementation()
{
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetPhysicsBlendWeight(1);
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFP_FirstPersonCharacter::PlayHit_Implementation()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (!!animInstance)
		animInstance->Montage_Play(TP_HitAnimation);
}

void AFP_FirstPersonCharacter::SetTeamColor_Implementation(ETeamType InTeamType)
{
	FLinearColor teamColor;

	if (InTeamType == ETeamType::Red)
	{
		teamColor = FLinearColor::Red;
	}
	else
	{
		teamColor = FLinearColor::Blue;
	}

	if (DynamicMaterial == nullptr)
	{

		if (DynamicMaterial == nullptr)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
			DynamicMaterial->SetVectorParameterValue("BodyColor", teamColor);

			GetMesh()->SetMaterial(0, DynamicMaterial);
			FP_Mesh->SetMaterial(0, DynamicMaterial);
		}
	}
}

void AFP_FirstPersonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFP_FirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFP_FirstPersonCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFP_FirstPersonCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FHitResult AFP_FirstPersonCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);

	CheckFalseResult(Hit.bBlockingHit, Hit);

	AFP_FirstPersonCharacter* other = Cast<AFP_FirstPersonCharacter>(Hit.GetActor());
	if (!!other)
	{
		FDamageEvent damageEvent;
		other->TakeDamage(WeaponDamage, damageEvent, GetController(), this);
	}

	return Hit;
}

float AFP_FirstPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CheckTrueResult(DamageCauser == this, DamageAmount);

	SelfPlayerState->Health -= DamageAmount;

	// »ç¸Á
	if (SelfPlayerState->Health <= 0)
	{
		PlayDead();

		SelfPlayerState->Death++;

		AFP_FirstPersonCharacter* causer = Cast<AFP_FirstPersonCharacter>(DamageCauser);
		if (!!causer)
			causer->SelfPlayerState->Score++;

		return DamageAmount;
	}

	// È÷Æ®
	PlayHit();

	return DamageAmount;
}

void AFP_FirstPersonCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFP_FirstPersonCharacter, CurrentTeam);
}