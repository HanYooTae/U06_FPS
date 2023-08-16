#include "FP_FirstPersonHUD.h"
#include "Global.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "FP_FirstPersonCharacter.h"
#include "Game/CPlayerState.h"

AFP_FirstPersonHUD::AFP_FirstPersonHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AFP_FirstPersonHUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	const FVector2D CrosshairDrawPosition( (Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
										   (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)) );

	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	AFP_FirstPersonCharacter* player = Cast<AFP_FirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(player);

	ACPlayerState* playerState = player->GetSelfPlayerState();
	CheckNull(playerState);

	FString str = "";
	float health = playerState->Health;
	float score = playerState->Score;
	float death = playerState->Death;

	str = "Health : " + FString::SanitizeFloat(health, 0);
	DrawText(str, FLinearColor::Black, 50, 50);

	str = "Score : " + FString::SanitizeFloat(score, 0);
	DrawText(str, FLinearColor::Black, 50, 70);

	str = "Death : " + FString::SanitizeFloat(death, 0);
	DrawText(str, FLinearColor::Black, 50, 90);
}
