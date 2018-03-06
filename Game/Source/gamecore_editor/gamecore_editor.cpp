// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCore_Editor.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"
#include "Engine/AssetManager.h"
#include "Textures/SlateIcon.h"
#include "Editor/WorkspaceMenuStructure/Public/WorkspaceMenuStructure.h"
#include "Editor/WorkspaceMenuStructure/Public/WorkspaceMenuStructureModule.h"

void FGameCore_EditorModule::StartupModule()
{
	if(!IsRunningCommandlet())
	{
	}
}

void FGameCore_EditorModule::ShutdownModule()
{

}

IMPLEMENT_MODULE(FGameCore_EditorModule, GameCore_Editor);
