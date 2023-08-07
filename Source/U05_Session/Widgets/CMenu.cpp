#include "CMenu.h"
#include "Components/Button.h"
#include "Global.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "CSessionRow.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

UCMenu::UCMenu(const FObjectInitializer& ObjectInitializer)
{
	CHelpers::GetClass(&SessionRowClass, "/Game/Widgets/WB_SessionRow");
}

bool UCMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	CheckFalseResult(bSuccess, false);


	CheckNullResult(HostButton, false);
	HostButton->OnClicked.AddDynamic(this, &UCMenu::OpenHostMenu);

	CheckNullResult(CancelHostMenuButton, false);
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UCMenu::OpenMainMenu);

	CheckNullResult(ConfirmHostMenuButton, false);
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UCMenu::HostServer);

	CheckNullResult(JoinButton, false);
	JoinButton->OnClicked.AddDynamic(this, &UCMenu::OpenJoinMenu);

	CheckNullResult(CancelJoinMenuButton, false);
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UCMenu::OpenMainMenu);

	CheckNullResult(ConfirmJoinMenuButton, false);
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UCMenu::JoinServer);

	CheckNullResult(QuitButton, false);
	QuitButton->OnClicked.AddDynamic(this, &UCMenu::QuitGame);

	return true;
}

void UCMenu::SetSessionList(TArray<FSessionData> InSessionDatas)
{
	UWorld* world = GetWorld();
	CheckNull(world);


	SessionList->ClearChildren();

	uint32 i = 0;
	for (const auto& data : InSessionDatas)
	{
		UCSessionRow* sessionRow = CreateWidget<UCSessionRow>(world, SessionRowClass);
		CheckNull(sessionRow);

		sessionRow->SessionName->SetText(FText::FromString(data.Name));
		sessionRow->HostUserName->SetText(FText::FromString(data.HostUserName));

		FString fractionStr = FString::Printf(L"%d/%d", data.CurrentPlayers, data.MaxPlayers);
		sessionRow->ConnectionFractions->SetText(FText::FromString(fractionStr));

		sessionRow->SetSelfIndex(this, i++);

		SessionList->AddChild(sessionRow);
	}
}

void UCMenu::SetSelectedRowIndex(uint32 InIndex)
{
	SelectedRowIndex = InIndex;
	SelectedRowIndex.IsSet();

	SelectedSessionRow();
}

void UCMenu::HostServer()
{
	CheckNull(OwingGameInstance);

	FString sessionName = SessionNameText->Text.ToString();

	OwingGameInstance->Host(sessionName);
}

void UCMenu::JoinServer()
{
	CheckNull(OwingGameInstance);

	//SelectedRowIndex = 100;

	//CheckNull(IPAddressField);
	//const FString& address = IPAddressField->GetText().ToString();

	if (SelectedRowIndex.IsSet())
	{
		CLog::Log("SelectedRowIndex : " + FString::FromInt(SelectedRowIndex.GetValue()));
		OwingGameInstance->Join(SelectedRowIndex.GetValue());
	}
	else
	{
		CLog::Log("SelectedRowIndex is not set");
	}


	
}

void UCMenu::OpenJoinMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(JoinMenu);
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if(!!OwingGameInstance)
		OwingGameInstance->FindSession();
}

void UCMenu::OpenMainMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(MainMenu);
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UCMenu::OpenHostMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(HostMenu);
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UCMenu::QuitGame()
{
	UWorld* world = GetWorld();
	CheckNull(world);

	APlayerController* controller = world->GetFirstPlayerController();
	CheckNull(controller);
	
	controller->ConsoleCommand("Quit");
}

void UCMenu::SelectedSessionRow()
{
	for (int32 i = 0; i < SessionList->GetChildrenCount(); i++)
	{
		UCSessionRow* sessionRow = Cast<UCSessionRow>(SessionList->GetChildAt(i));
		if (!!sessionRow)
			sessionRow->bSelfClicked = (SelectedRowIndex.IsSet() && i == SelectedRowIndex);
	}
}
