#include "CInGame.h"
#include "Global.h"
#include "Components/Button.h"


bool UCInGame::Initialize()
{
	bool bSuccess = Super::Initialize();
	CheckFalseResult(bSuccess, false);

	CheckNullResult(CancelButton, false);
	CancelButton->OnClicked.AddDynamic(this, &UCInGame::CancelButtonPressed);

	CheckNullResult(QuitButton, false);
	QuitButton->OnClicked.AddDynamic(this, &UCInGame::QutiButtonPressed);

	return true;
}

void UCInGame::CancelButtonPressed()
{
	Detach();
}

void UCInGame::QutiButtonPressed()
{
	CheckNull(OwingGameInstance);

	Detach();
	OwingGameInstance->ReturnToMainMenu();
}
