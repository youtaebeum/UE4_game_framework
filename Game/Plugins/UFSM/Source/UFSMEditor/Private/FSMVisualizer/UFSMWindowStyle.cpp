/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMWindowStyle.h"

#include "SlateStyle.h"
#include "IPluginManager.h"
#include "../UFSMEditorShared.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BORDER_BRUSH( RelativePath,...) FSlateBorderBrush(FSMWindowStyle::InContent(RelativePath,TEXT(".png")),__VA_ARGS__)
#define IMAGE_BRUSH(RelativePath,...) FSlateImageBrush(FSMWindowStyle::InContent(RelativePath,TEXT(".png")),__VA_ARGS__)
#define BOX_BRUSH( RelativePath,...) FSlateBoxBrush(FSMWindowStyle::InContent(RelativePath,TEXT(".png")),__VA_ARGS__)
#define TTF_FONT( RelativePath,...) FSlateFontInfo(FSMWindowStyle::InContent(RelativePath,TEXT(".ttf")),__VA_ARGS__)
#define OTF_FONT( RelativePath,...) FSlateFontInfo(FSMWindowStyle::InContent(RelativePath,TEXT(".otf")),__VA_ARGS__)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedPtr<FSlateStyleSet> FSMWindowStyle::StyleSet = nullptr;
TSharedPtr<ISlateStyle> FSMWindowStyle::Get() {return StyleSet;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FSMWindowStyle::InContent(const FString &RelativePath, const TCHAR* Extension) {
	static FString Content = IPluginManager::Get().FindPlugin(TEXT("UFSM"))->GetContentDir();
	return (Content/RelativePath)+Extension;
}

FName FSMWindowStyle::GetStyleSetName() {
	static FName StyleName(TEXT("FSMWindowStyle"));
	return StyleName;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FSMWindowStyle::Initialize() {
	if (StyleSet.IsValid()) {return;}
	//
	const FVector2D Icon16x16(16.0f,16.0f);
	const FVector2D Icon20x20(20.0f,20.0f);
	const FVector2D Icon40x40(40.0f,40.0f);
	const FVector2D Icon64x64(128.0f,128.0f);
	const FVector2D Icon128x128(128.0f,128.0f);
	//
	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("UFSM"))->GetContentDir());
	//
	StyleSet->Set("FSMVisualizerWindow.Tab", new IMAGE_BRUSH(TEXT("Icons/Fsm_Details_16x"),Icon16x16));
	//StyleSet->Set("FSMVisualizerWindow.OpenFSMVisualizerWindow", new IMAGE_BRUSH(TEXT("Icons/Fsm_Toolbar_128x"),Icon40x40));
	//
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FSMWindowStyle::Shutdown() {
	if (StyleSet.IsValid()) {
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FSMWindowStyle::ReloadTextures() {
	if (FSlateApplication::IsInitialized()) {
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef BORDER_BRUSH
#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef TTF_FONT
#undef OTF_FONT

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////