#include "CBullet.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACBullet::ACBullet()
{
	CHelpers::CreateSceneComponent(this, &Mesh, "Mesh");
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");


	UStaticMesh* meshAsset;
	CHelpers::GetAsset(&meshAsset, "StaticMesh'/Engine/BasicShapes/Sphere.Sphere'");
	Mesh->SetStaticMesh(meshAsset);

	UMaterial* materialAsset;
	CHelpers::GetAsset(&materialAsset, "Material'/Game/Materials/MAT_Bullet.MAT_Bullet'");
	Mesh->SetMaterial(0, materialAsset);

	Mesh->SetRelativeScale3D(FVector(1.f, 0.025f, 0.025f));

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->bCastDynamicShadow = false;


	Projectile->InitialSpeed = 1200; //2E+4f;
	Projectile->MaxSpeed = 1200; //2E+4f;
	Projectile->ProjectileGravityScale = 0.f;

	
	
}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		bReplicates = true;
		SetReplicateMovement(true);
	}

	SetLifeSpan(3.f);
}
