#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSessionRow.generated.h"

UCLASS()
class U05_SESSION_API UCSessionRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSelfIndex(class UCMenu* InParent, uint32 InIndex);

private:
	UFUNCTION()
		void OnClicked();

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SessionName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HostUserName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ConnectionFractions;

	UPROPERTY(BlueprintReadOnly)
		bool bSelfClicked;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

private:
	class UCMenu* Parent;
	uint32 SelfIndex;
};
