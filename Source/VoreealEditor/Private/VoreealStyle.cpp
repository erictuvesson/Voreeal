#include "VoreealEditorPrivatePCH.h"
#include "VoreealStyle.h"

#include "SlateStyle.h"
#include "IPluginManager.h"

#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FVoreealStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )
#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(StyleSet->RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define TTF_CORE_FONT(RelativePath, ...) FSlateFontInfo(StyleSet->RootToCoreContentDir(RelativePath, TEXT(".ttf") ), __VA_ARGS__)

FString FVoreealStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("Voreeal"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr<FSlateStyleSet> FVoreealStyle::StyleSet = nullptr;
TSharedPtr<class ISlateStyle> FVoreealStyle::Get() { return StyleSet; }

FName FVoreealStyle::GetStyleSetName()
{
	static FName VoreealStyleName(TEXT("VoreealStyle"));
	return VoreealStyleName;
}

void FVoreealStyle::Initialize()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	// Basic Volume Editor
	{
		StyleSet->Set("BasicVolumeEditor.EnterViewMode", new IMAGE_PLUGIN_BRUSH("Icons/icon_Voreeal_ViewVolume_40x", Icon40x40));
		StyleSet->Set("BasicVolumeEditor.EnterEditMode", new IMAGE_PLUGIN_BRUSH("Icons/icon_Voreeal_EditVolume_40x", Icon40x40));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

#undef IMAGE_PLUGIN_BRUSH
#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef TTF_FONT
#undef TTF_CORE_FONT

void FVoreealStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}