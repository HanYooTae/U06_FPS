#pragma once

#include "CoreMinimal.h"
#include "CMenuBase.h"
#include "CInGame.generated.h"

UCLASS()
class U05_SESSION_API UCInGame : public UCMenuBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
		void CancelButtonPressed();

	UFUNCTION()
		void QutiButtonPressed();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;
};
