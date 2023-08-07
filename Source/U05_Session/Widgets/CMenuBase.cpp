#include "CMenuBase.h"
#include "Global.h"

void UCMenuBase::SetOwingGameInstance(IIMenuInterface* InOwingInterface)
{
	OwingGameInstance = InOwingInterface;
}

void UCMenuBase::Attach()
{
	AddToViewport();

	bIsFocusable = true;

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* world = GetWorld();
	CheckNull(world);
	APlayerController* controller = world->GetFirstPlayerController();
	CheckNull(controller);

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
}

void UCMenuBase::Detach()
{
	RemoveFromParent();

	bIsFocusable = false;

	FInputModeGameOnly inputMode;

	UWorld* world = GetWorld();
	CheckNull(world);
	APlayerController* controller = world->GetFirstPlayerController();
	CheckNull(controller);

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = false;
}