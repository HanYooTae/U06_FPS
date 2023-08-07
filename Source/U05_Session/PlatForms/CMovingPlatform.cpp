#include "CMovingPlatform.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"

ACMovingPlatform::ACMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(L"StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'");
	
	if(meshAsset.Succeeded())
		GetStaticMeshComponent()->SetStaticMesh(meshAsset.Object);

	UMaterialInstanceConstant* materialAsset;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&materialAsset, "MaterialInstanceConstant'/Game/Platforms/MAT_MovingPlatform_Inst.MAT_MovingPlatform_Inst'");
	GetStaticMeshComponent()->SetMaterial(0, materialAsset);

	GetStaticMeshComponent()->SetRelativeScale3D(FVector(1.f, 1.f, 0.2f));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
}

void ACMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// 서버의 위치 값을 replicate(Client에게 전파)
	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void ACMovingPlatform::IncreaseActive()
{
	Active++;
}

void ACMovingPlatform::DecreaseActive()
{
	if(Active > 0)
		Active--;
}

void ACMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Active가 1 이상인 경우에만
	if (Active > 0)
	{
		// 서버에서만 이동
		if (HasAuthority() == true)
		{

			FVector location = GetActorLocation();

			float totalDistance = (GlobalStartLocation - GlobalTargetLocation).Size();
			float currentDistance = (location - GlobalStartLocation).Size();

			if (currentDistance >= totalDistance)
			{
				FVector temp = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = temp;
			}

			FVector direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();

			location += direction * Speed * DeltaTime;
			SetActorLocation(location);
		}
	}

	

}