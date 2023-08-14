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

	Capsule->SetHiddenInGame(bHiddenInGame);

}

void ACSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ACSpawnPoint::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACSpawnPoint::EndOverlap);
}

void ACSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Capsule->UpdateOverlaps();
}

void ACSpawnPoint::BeginOverlap(AActor* OverlapActor, AActor* OtherActor)
{
	CheckFalse(HasAuthority());

	if (OverlappingActors.Find(OtherActor) < 0)
		OverlappingActors.AddUnique(OtherActor);
}

void ACSpawnPoint::EndOverlap(AActor* OverlapActor, AActor* OtherActor)
{
	CheckFalse(HasAuthority());

	if (OverlappingActors.Find(OtherActor) >= 0)
		OverlappingActors.Remove(OtherActor);

}

