#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FP_FirstPersonCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class USoundBase;
class UAnimMontage;

UCLASS(config=Game)
class AFP_FirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Components
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
		USkeletalMeshComponent* FP_Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Mesh")
		USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* TP_Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* FP_GunshotParticle;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* TP_GunshotParticle;

public:
	AFP_FirstPersonCharacter();

protected:
	virtual void BeginPlay() override;

public:
	// Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* TP_FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float WeaponRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float WeaponDamage;

protected:
	void OnFire();

	UFUNCTION(Reliable, Server)
		void OnFire_Server(const FVector& LineStart, const FVector& LienEnd);
		void OnFire_Server_Implementation(const FVector& LineStart, const FVector& LienEnd);

	UFUNCTION(NetMulticast, Reliable)
		void FireEffect();
		void FireEffect_Implementation();

	void MoveForward(float Val);

	void MoveRight(float Val);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	FORCEINLINE class USkeletalMeshComponent* GetFP_Mesh() const { return FP_Mesh; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};