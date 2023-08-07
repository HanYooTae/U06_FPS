#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CMovingPlatform.generated.h"

UCLASS()
class U05_SESSION_API ACMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACMovingPlatform();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	void IncreaseActive();
	void DecreaseActive();

private:
	// Moving Platform Speed Per Tick
	UPROPERTY(EditAnywhere)
		float Speed = 20.f;

	// 0 : Not Move, else : Move
	UPROPERTY(EditAnywhere)
		int32 Active = 0;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget))
		FVector TargetLocation = FVector(0, 0, 100);

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget))
		FVector GlobalStartLocation;
		FVector GlobalTargetLocation;

private:
	class UStaticMesh* Mesh;
};
