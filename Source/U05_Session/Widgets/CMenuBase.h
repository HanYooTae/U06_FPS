#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IMenuInterface.h"
#include "CMenuBase.generated.h"

UCLASS()
class U05_SESSION_API UCMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwingGameInstance(IIMenuInterface* InOwingInterface);

	void Attach();
	void Detach();


protected:
	IIMenuInterface* OwingGameInstance;
};
