#include "CSpawnPoint.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"

ACSpawnPoint::ACSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &Capsule, "Capsule");
}

void ACSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Team == ETeamType::Red)
		Capsule->ShapeColor = FColor(255, 0, 0);
	else
		Capsule->ShapeColor = FColor(0, 0, 255);

}

void ACSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

