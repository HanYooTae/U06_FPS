#include "CGameInstance.h"
#include "Global.h"
#include "Widgets/CMenuBase.h"
#include "Widgets/CMenu.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("GameSession");
const static FName SESSION_SETTINGS_KEY = TEXT("SessionKey");

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	CLog::Log("GameInstance::Constructor Called");

	CHelpers::GetClass(&MenuWidgetClass, "/Game/Widgets/WB_Menu");
	CHelpers::GetClass(&InGameWidgetClass, "/Game/Widgets/WB_InGame");
	//CLog::Log(MenuWidgetClass->GetName());
}

void UCGameInstance::Init()
{
	Super::Init();

	CLog::Log("GameInstance::Init Called");

	IOnlineSubsystem* oss = IOnlineSubsystem::Get();
	
	if (!!oss)
	{
		CLog::Log("OSS Name : " + oss->GetSubsystemName().ToString());
		
		// SessionEvent Binding
		SessionInterface = oss->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnJoinSessionComplete);

		}
	}
	else
	{
		CLog::Log("OSS Not Found!");
	}

	GEngine->OnNetworkFailure().AddUObject(this, &UCGameInstance::OnNetworkFailure);
}

void UCGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;

		if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			sessionSettings.bIsLANMatch = true;
			sessionSettings.bUsesPresence = false;
		}
		else    // Steam
		{
			sessionSettings.bIsLANMatch = false;
			sessionSettings.bUsesPresence = true;
		}

		sessionSettings.NumPublicConnections = 5;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.Set(SESSION_SETTINGS_KEY, DesiredSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
}

void UCGameInstance::StartSession()
{
	CheckFalse(SessionInterface.IsValid());
	SessionInterface->StartSession(SESSION_NAME);
}

void UCGameInstance::LoadMenu()
{
	CheckNull(MenuWidgetClass);
	
	Menu = CreateWidget<UCMenu>(this, MenuWidgetClass);
	CheckNull(Menu);

	Menu->SetOwingGameInstance(this);
	Menu->Attach();
}

void UCGameInstance::LoadInGameMenu()
{
	CheckNull(InGameWidgetClass);

	UCMenuBase* inGameWidget = CreateWidget<UCMenuBase>(this, InGameWidgetClass);
	CheckNull(inGameWidget);

	inGameWidget->SetOwingGameInstance(this);
	inGameWidget->Attach();
}

void UCGameInstance::Host(const FString InSessionName)
{
	DesiredSessionName = InSessionName;

	if (SessionInterface.IsValid())
	{
		auto session = SessionInterface->GetNamedSession(SESSION_NAME);

		if (!!session)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}

		else
		{
			CreateSession();
		}
	}

}

void UCGameInstance::Join(uint32 InSessionIndex)
{
	CLog::Print("Join to " + InSessionIndex);
	
	CheckFalse(SessionInterface.IsValid());
	
	if (!!Menu)
		Menu->Detach();

	//GetEngine()->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(L"Join to %s", InAddress));

	
	
	CheckFalse(SessionSearch.IsValid());
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[InSessionIndex]);
}

void UCGameInstance::ReturnToMainMenu()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::FindSession()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		CLog::Log("Starting Find Session");

		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UCGameInstance::OnCreateSessionComplete(FName InSessionName, bool InSuccess)
{
	//UE_LOG(LogTemp, Error, TEXT("CreateSessionComplete"));

	// 技记 积己 角菩
	if (InSuccess == false)
	{
		CLog::Log("Could not create Session!!");
		return;
	}
	
	//  技记 积己 己傍
	CLog::Log("Session Name : " + InSessionName.ToString());

	if (!!Menu)
		Menu->Detach();

	CLog::Print("Host");
	//-> Everybody Move to Play Map
	UWorld* world = GetWorld();
	CheckNull(world);

	world->ServerTravel("/Game/Maps/Lobby?listen");
}

void UCGameInstance::OnDestroySessionComplete(FName InSessionName, bool InSuccess)
{
	UE_LOG(LogTemp, Error, TEXT("DestroySessionComplete"));

	if(InSuccess == true)
		CreateSession();
}

void UCGameInstance::OnFindSessionComplete(bool InSuccess)
{
	if (InSuccess == true && Menu != nullptr && SessionSearch.IsValid())
	{
		TArray<FSessionData> foundSession;

		CLog::Log("Finished Find Session");

		SessionSearch->SearchResults;

		CLog::Log("=====<Find Session Result>=====");
		for (const auto& searchResult : SessionSearch->SearchResults)
		{
			CLog::Log(" -> Session ID : " + searchResult.GetSessionIdStr());
			CLog::Log("Ping : " + FString::FromInt(searchResult.PingInMs));

			FSessionData data;
			data.MaxPlayers = searchResult.Session.SessionSettings.NumPublicConnections;
			data.CurrentPlayers = data.MaxPlayers - searchResult.Session.NumOpenPublicConnections;
			data.HostUserName = searchResult.Session.OwningUserName;

			FString sessionName;
			if (searchResult.Session.SessionSettings.Get(SESSION_SETTINGS_KEY, sessionName))
			{
				CLog::Log("Session.Value() => " + sessionName);
				data.Name = sessionName;
			}
			else
			{
				CLog::Log("Session Settings Key Not Found");
			}

			foundSession.Add(data);
		}
		CLog::Log("===============================");

		Menu->SetSessionList(foundSession);
	}
}

void UCGameInstance::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult)
{
	FString address;

	// 炼牢 角菩
	if (SessionInterface->GetResolvedConnectString(InSessionName, address) == false)
	{
		switch (InResult)
		{
		case EOnJoinSessionCompleteResult::SessionIsFull:
			CLog::Log("SessionIsFull");
			break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			CLog::Log("SessionDoesNotExist");
			break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			CLog::Log("CouldNotRetrieveAddress");
			break;
		case EOnJoinSessionCompleteResult::AlreadyInSession:
			CLog::Log("AlreadyInSession");
			break;
		case EOnJoinSessionCompleteResult::UnknownError:
			CLog::Log("UnknownError");
		}

		return;
	}

	// 炼牢 己傍
	CLog::Print("Join to " + address);

	//UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//GetWorld()->GetFirstPlayerController();
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::OnNetworkFailure(UWorld* InWorld, UNetDriver* InNetDriver, ENetworkFailure::Type InFailureReason, const FString& InErrorMessage)
{
	CLog::Print("Network Error Message : " + InErrorMessage);

	ReturnToMainMenu();
}
