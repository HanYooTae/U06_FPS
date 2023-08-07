#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

//--------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------
#define CheckNull(p) { if (p == nullptr) return; }
#define CheckNullResult(p, result) { if (p == nullptr) return result; }

#define CheckTrue(p) { if (p == true) return; }
#define CheckTrueResult(p, result) { if (p == true) return result; }

#define CheckFalse(p) { if (p == false) return; }
#define CheckFalseResult(p, result) { if (p == false) return result; }


//--------------------------------------------------------------------------------
// CHelpers
//--------------------------------------------------------------------------------
class U05_SESSION_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutAsset, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"Asset Not Found");
		*OutAsset = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutAsset, FString InPath)
	{
		T* asset = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
		verifyf(!!asset, L"Asset Not Found");

		*OutAsset = asset;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"Class Not Found");
			*OutClass = asset.Class;
	}

	template<typename T>
	static void CreateSceneComponent(AActor* InActor, T** OutComp, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComp = InActor->CreateDefaultSubobject<T>(InName);
		if (!!InParent)
		{
			(*OutComp)->SetupAttachment(InParent);
			return;
		}

		InActor->SetRootComponent(*OutComp);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComp, FName InName)
	{
		*OutComp = InActor->CreateDefaultSubobject<T>(InName);
	}
};