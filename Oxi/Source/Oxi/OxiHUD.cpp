// ELP 2020

#include "OxiHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AOxiHUD::AOxiHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/Weapons/ELPistol/CrossHair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AOxiHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas

	static float hackXOffset = -8.0f;		// was 0.0f
	static float hackYOffset = -8.0f;		// was 20.0f
	const FVector2D CrosshairDrawPosition( (Center.X + hackXOffset),
										   (Center.Y + hackYOffset));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
