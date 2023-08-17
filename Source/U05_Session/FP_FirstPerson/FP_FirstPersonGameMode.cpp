#include "FP_FirstPersonGameMode.h"
#include "FP_FirstPersonHUD.h"
#include "FP_FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Game/CGameState.h"
#include "Game/CPlayerState.h"
#include "Game/CSpawnPoint.h"
#include "Global.h"
#include "EngineUtils.h"

AFP_FirstPersonGameMode::AFP_FirstPersonGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default Pawn Class
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// HUD Class
	HUDClass = AFP_FirstPersonHUD::StaticClass();

	// GameState Class
	GameStateClass = ACGameState::StaticClass();

	// PlayerState Class
	PlayerStateClass = ACPlayerState::StaticClass();
}

void AFP_FirstPersonGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CLog::Print("PostLogin Called");

	// Get Player State & Get Player Pawn
	ACPlayerState* playerState = Cast<ACPlayerState>(NewPlayer->PlayerState);
	CheckNull(playerState);

	// Team Distribution
	AFP_FirstPersonCharacter* playerPawn = Cast<AFP_FirstPersonCharacter>(NewPlayer->GetPawn());
	CheckNull(playerPawn);

	if (RedTeamPlayers.Num() > BlueTeamPlayers.Num())
	{
		BlueTeamPlayers.Add(playerPawn);
		playerState->Team = ETeamType::Blue;
	}
	else
	{
		RedTeamPlayers.Add(playerPawn);
		playerState->Team = ETeamType::Red;
	}

	// Team Setting
	playerPawn->CurrentTeam = playerState->Team;
	playerPawn->SetTeamColor(playerPawn->CurrentTeam);

	// Get Spawn Points
	UWorld* world = GetWorld();
	CheckNull(world);

	SpawnHost(world);

	MoveToSpawnPoint(playerPawn);
}

void AFP_FirstPersonGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CLog::Print("WaitingPlayers : " + FString::FromInt(WaitingPlayers.Num()), 1);

	if (WaitingPlayers.Num() > 0)
	{
		for (const auto& player : WaitingPlayers)
		{
			MoveToSpawnPoint(player);
		}
	}
}

void AFP_FirstPersonGameMode::SpawnHost(UWorld* world)
{
	APlayerController* hostController = world->GetFirstPlayerController();

	AFP_FirstPersonCharacter* hostPawn = Cast<AFP_FirstPersonCharacter>(hostController->GetPawn());
	CheckNull(hostPawn);

	MoveToSpawnPoint(hostPawn);
}

void AFP_FirstPersonGameMode::Respawn(AFP_FirstPersonCharacter* InPlayer)
{
	// 컨트롤러 끊기
	AController* controller = InPlayer->GetController();
	InPlayer->DetachFromControllerPendingDestroy();

	// 다시 스폰시키기
	AFP_FirstPersonCharacter* player =  GetWorld()->SpawnActor<AFP_FirstPersonCharacter>(DefaultPawnClass);
	CheckNull(player);

	// 다시 빙의
	controller->Possess(player);

	// 기존의 팀정보 유지
	//player->GetSelfPlayerState();
	ACPlayerState* playerState = Cast<ACPlayerState>(controller->PlayerState);
	player->CurrentTeam = playerState->Team;
	player->SetTeamColor(playerState->Team);

	// 스폰 포인트로 이동
	MoveToSpawnPoint(player);
}

void AFP_FirstPersonGameMode::MoveToSpawnPoint(AFP_FirstPersonCharacter* InPlayer)
{
	TArray<ACSpawnPoint*>* targetPoints;

	if (InPlayer->CurrentTeam == ETeamType::Red)
		targetPoints = &RedTeamSpawnPoints;
	else
		targetPoints = &BlueTeamSpawnPoints;

	for (const auto& target : *targetPoints)
	{
		// 스폰 포인트가 블락되어 있지 않은 경우
		if (target->IsBlocked() == false)
		{
			InPlayer->SetActorLocation(target->GetActorLocation());
			target->UpdateOverlaps();

			if (WaitingPlayers.Find(InPlayer) >= 0)
				WaitingPlayers.Remove(InPlayer);

			return;
		}
	}
	// 스폰 포인트가 블락되어 있는 경우
	if (WaitingPlayers.Find(InPlayer) < 0)
		WaitingPlayers.Add(InPlayer);
}

void AFP_FirstPersonGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	CheckNull(world);

	for (TActorIterator<ACSpawnPoint> it(world); it; ++it)
	{
		if ((*it)->GetTeam() == ETeamType::Red)
			RedTeamSpawnPoints.Add(*it);
		else
			BlueTeamSpawnPoints.Add(*it);
	}

	//for (const auto& player : RedTeamPlayers)
	//	MoveToSpawnPoint(player);
	//
	//for (const auto& player : BlueTeamPlayers)
	//	MoveToSpawnPoint(player);

	
}
