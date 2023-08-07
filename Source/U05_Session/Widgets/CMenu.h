#pragma once

#include "CoreMinimal.h"
#include "CMenuBase.h"
#include "CMenu.generated.h"


USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

public:
	FString Name;
	uint16 MaxPlayers;
	uint16 CurrentPlayers;
	FString HostUserName;
};


UCLASS()
class U05_SESSION_API UCMenu : public UCMenuBase
{
	GENERATED_BODY()
	
public:
	UCMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

public:
	void SetSessionList(TArray<FSessionData> InSessionDatas);
	void SetSelectedRowIndex(uint32 InIndex);

private:
	UFUNCTION() void HostServer();
	UFUNCTION() void JoinServer();
	UFUNCTION() void OpenJoinMenu();
	UFUNCTION() void OpenMainMenu();
	UFUNCTION() void OpenHostMenu();
	UFUNCTION() void QuitGame();

private:
	void SelectedSessionRow();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmJoinMenuButton;
	
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* SessionList;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* SessionNameText;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelHostMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmHostMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* HostMenu;

private:
	TSubclassOf<class UUserWidget> SessionRowClass;
	
	TOptional<uint32> SelectedRowIndex;

};