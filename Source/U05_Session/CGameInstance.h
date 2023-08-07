#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/IMenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/EngineBaseTypes.h"
#include "CGameInstance.generated.h"

UCLASS()
class U05_SESSION_API UCGameInstance : public UGameInstance, public IIMenuInterface
{
	GENERATED_BODY()

public:
	UCGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

public:
	UFUNCTION(BlueprintCallable, Exec)
		void LoadMenu();

	UFUNCTION(BlueprintCallable, Exec)
		void LoadInGameMenu();

	UFUNCTION(Exec)
		void Host(const FString InSessionName) override;

	UFUNCTION(Exec)
		void Join(uint32 InSessionIndex) override;

		void ReturnToMainMenu() override;

		void FindSession() override;

private:
	void OnCreateSessionComplete(FName InSessionName, bool InSuccess);
	void OnDestroySessionComplete(FName InSessionName, bool InSuccess);
	void OnFindSessionComplete(bool InSuccess);
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult);
	void OnNetworkFailure(UWorld* InWorld, UNetDriver* InNetDriver, ENetworkFailure::Type InFailureReason, const FString& InErrorMessage);

	void CreateSession();

public:
	void StartSession();

private:
	TSubclassOf<UUserWidget> MenuWidgetClass;
	TSubclassOf<UUserWidget> InGameWidgetClass;
	
	class UCMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredSessionName;
};
