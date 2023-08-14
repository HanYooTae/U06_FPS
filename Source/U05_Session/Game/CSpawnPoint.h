#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGameState.h"
#include "CSpawnPoint.generated.h"

UCLASS()
class U05_SESSION_API ACSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSpawnPoint();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE ETeamType GetTeam() { return Team; }
	bool IsBlocked() { return OverlappingActors.Num() > 0; }

private:
	UFUNCTION()
		void BeginOverlap(AActor* OverlapActor, AActor* OtherActor);

	UFUNCTION()
		void EndOverlap(AActor* OverlapActor, AActor* OtherActor);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

	TArray<AActor*> OverlappingActors;

protected:
	UPROPERTY(EditAnywhere)
		ETeamType Team;

	UPROPERTY(EditAnywhere)
		bool bHiddenInGame = false;

};
