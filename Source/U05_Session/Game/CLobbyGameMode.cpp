#include "CLobbyGameMode.h"
#include "Global.h"
#include "CGameinstance.h"

void ACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;
	CLog::Print("Player : " + FString::FromInt(NumberOfPlayers));

	if (NumberOfPlayers >= 3)
	{
		CLog::Print("Ready to play");

		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ACLobbyGameMode::StartSession, 10);
	}
}

void ACLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
	CLog::Print("Player : " + FString::FromInt(NumberOfPlayers));
}

void ACLobbyGameMode::StartSession()
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());
	CheckNull(gameInstance);
	gameInstance->StartSession();

	UWorld* world = GetWorld();
	CheckNull(world);

	//bUseSeamlessTravel = true;
	world->ServerTravel("/Game/Maps/FPS?Listen");
}
