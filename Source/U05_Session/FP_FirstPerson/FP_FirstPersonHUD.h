#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FP_FirstPersonHUD.generated.h"

UCLASS()
class AFP_FirstPersonHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFP_FirstPersonHUD();

	virtual void DrawHUD() override;

private:
	class UTexture2D* CrosshairTex;

};

