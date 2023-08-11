#include "CPlayerState.h"
#include "Net/UnrealNetwork.h"

ACPlayerState::ACPlayerState(const FObjectInitializer& ObjectInitializer)
{
	Health = 100.f;
	Death = 0.f;
	Team = ETeamType::Blue;
}

void ACPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPlayerState, Health);
	DOREPLIFETIME(ACPlayerState, Death);
	DOREPLIFETIME(ACPlayerState, Team);
}