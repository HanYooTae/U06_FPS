#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CButton.generated.h"

UCLASS()
class U05_SESSION_API ACButton : public AActor
{
	GENERATED_BODY()
	
public:
	ACButton();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
		void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly)
		TArray<class ACMovingPlatform*> Platforms;
};
