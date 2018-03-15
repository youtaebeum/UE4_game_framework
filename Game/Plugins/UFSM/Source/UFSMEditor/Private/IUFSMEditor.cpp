/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IUFSMEditor.h"
#include "UFSMEditor.h"

#include "UFSMGraph.h"
#include "UFSMGraphNode.h"
#include "UFSMGraphWidget.h"
#include "UFSMEditorShared.h"

#include "Editor/BlueprintGraph/Classes/K2Node_EditablePinBase.h"
#include "Editor/BlueprintGraph/Classes/K2Node_FunctionEntry.h"
#include "Editor/BlueprintGraph/Classes/EdGraphSchema_K2.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "PropertyEditing.h"
#include "SDockTab.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FSMEditor::StartupModule() {
	IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	SY_AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Synaptech")),LOCTEXT("SynaptechCategory","Synaptech"));
	//
	{///
		TSharedRef<IAssetTypeActions> ACT_FSM = MakeShareable(new FATA_FSM); AssetTools.RegisterAssetTypeActions(ACT_FSM);
	}///
	//
	//
	FSMStyle::Initialize();
	FSMWindowStyle::Initialize();
	FSMWindowStyle::ReloadTextures();
	FSMCommands::Register();
	//
	FLevelEditorModule &LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	//
	{///
		FPropertyEditorModule &PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModule.RegisterCustomClassLayout("StateMachineComponent",FOnGetDetailCustomizationInstance::CreateStatic(&FSMCustomDetails::MakeInstance));
	}///
	//
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FSMVisualizerTabName,FOnSpawnTab::CreateRaw(this,&FSMEditor::OnSpawnFSMVisualizerTab))
		.SetDisplayName(LOCTEXT("FSMVisualizerWindowTitle","FSM Visualizer"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
	//
	//
	RegisterSettings();
}

void FSMEditor::ShutdownModule() {
	FSMStyle::Shutdown();
	FSMWindowStyle::Shutdown();
	FSMCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FSMVisualizerTabName);
	if (UObjectInitialized()) {UnregisterSettings();}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FSMEditor::AddMenuExtension(FMenuBuilder &Builder) {
	Builder.AddMenuEntry(FSMCommands::Get().OpenFSMVisualizerWindow);
}

void FSMEditor::AddToolbarExtension(FToolBarBuilder &Builder) {
	Builder.AddToolBarButton(FSMCommands::Get().OpenFSMVisualizerWindow);
}

TSharedRef<SDockTab> FSMEditor::OnSpawnFSMVisualizerTab(const FSpawnTabArgs &SpawnTabArgs) {
	TSharedRef<SDockTab> TAB = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	TAB->SetTabIcon(FSMWindowStyle::Get()->GetBrush(TEXT("FSMVisualizerWindow.Tab")));
	TAB->SetContent(SNew(UFSMGraphWidget));
	//
	return TAB;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
T* GetBuilder(IDetailLayoutBuilder* DetailBuilder) {
	TArray<TWeakObjectPtr<UObject>> OutObjects;
	DetailBuilder->GetObjectsBeingCustomized(OutObjects);
	//
	T* OBJ = nullptr;
	if (OutObjects.Num()>0) {
		OBJ = Cast<T>(OutObjects[0].Get());
	} return OBJ;
}

UBlueprint* GetBuilderBlueprint(UObject* OBJ) {
	UBlueprint* BP = nullptr;
	//
	if (OBJ != nullptr) {
		BP = Cast<UBlueprint>(OBJ);
		if (BP == nullptr) {
			BP = Cast<UBlueprint>(OBJ->GetClass()->ClassGeneratedBy);
	}} return BP;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedRef<IDetailCustomization> FSMCustomDetails::MakeInstance() {
	return MakeShareable(new FSMCustomDetails);
}

void FSMCustomDetails::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) {
	const auto &FSM = GetBuilder<UStateMachineComponent>(&DetailBuilder);
	if (FSM->STATES.Num()==0) {return;}
	//
	IDetailCategoryBuilder &Category = DetailBuilder.EditCategory("FSM");
	Category.AddCustomRow(FText::FromString("Visualize"))
	.NameContent()
	[///
		SNew(SHorizontalBox)
		.ToolTipText(FText::FromString("Displays this FSM's Hierarchy Graph."))
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.Padding(0,0,5,0)
		.AutoWidth()
		[///
			SNew(SImage)
			.Image(FSMStyle::Get()->GetBrush("ClassIcon.StateMachineComponent"))
		]///
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[///
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Text(FText::FromString("Visualize"))
		]///
	]///
	.ValueContent()
	.MinDesiredWidth(150)
	.MaxDesiredWidth(300)
	[///
		SNew(SButton)
		.ButtonStyle(FEditorStyle::Get(),"FlatButton.Success")
		.HAlign(HAlign_Center)
		.OnClicked(FOnClicked::CreateStatic(&FSMCustomDetails::VisualizeFSM,&DetailBuilder))
		.ForegroundColor(FSlateColor::UseForeground())
		[///
			SNew(STextBlock)
			.Font(FEditorStyle::GetFontStyle(TEXT("BoldFont")))
			.Text(LOCTEXT("ViewStates","FSM States"))
		]///
	];///
	//
	//
	Category.AddCustomRow(FText::FromString("Generate"))
	.NameContent()
	[///
		SNew(SHorizontalBox)
		.ToolTipText(FText::FromString("Generates FSM's State Functions on Blueprint Graph."))
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.Padding(0,0,5,0)
		.AutoWidth()
		[///
			SNew(SImage)
			.Image(FSMStyle::Get()->GetBrush("ClassIcon.StateMachineComponent"))
		]///
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[///
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Text(FText::FromString("Generate"))
		]///
	]///
	.ValueContent()
	.MinDesiredWidth(150)
	.MaxDesiredWidth(300)
	[///
		SNew(SButton)
		.ButtonStyle(FEditorStyle::Get(),"FlatButton.Success")
		.OnClicked(FOnClicked::CreateStatic(&FSMCustomDetails::AddFunctions,&DetailBuilder))
		.ForegroundColor(FSlateColor::UseForeground())
		.HAlign(HAlign_Center)
		[///
			SNew(STextBlock)
			.Font(FEditorStyle::GetFontStyle(TEXT("BoldFont")))
			.Text(LOCTEXT("AddFunctions","+Functions"))
		]///
	];///
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FReply FSMCustomDetails::VisualizeFSM(IDetailLayoutBuilder* DetailBuilder) {
	const auto &FSM = GetBuilder<UStateMachineComponent>(DetailBuilder);
	//
	if (FSM) {
		TSharedRef<SDockTab> TAB = FGlobalTabmanager::Get()->InvokeTab(FSMVisualizerTabName);
		TSharedRef<UFSMGraphWidget> Widget = StaticCastSharedRef<UFSMGraphWidget>(TAB->GetContent());
		if (Widget->FSMGraph.Get()->FSM==*&FSM) {return FReply::Handled();}
		else {Widget->FSMGraph.Get()->Initialize(*&FSM);}
	} return FReply::Handled();
}

FReply FSMCustomDetails::AddFunctions(IDetailLayoutBuilder* DetailBuilder) {
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	const auto &OBJ = GetBuilder<UStateMachineComponent>(DetailBuilder);
	const auto &BP = GetBuilderBlueprint(OBJ);
	//
	for (const auto &State : OBJ->STATES) {
		FString NID_OnExit = *(FString("OnExit")+State.ToString().Replace(TEXT(" "),TEXT("")));
		FString NID_OnBegin = *(FString("OnBegin")+State.ToString().Replace(TEXT(" "),TEXT("")));
		FString NID_OnUpdate = *(FString("OnUpdate")+State.ToString().Replace(TEXT(" "),TEXT("")));
		FString NID_OnFinish = *(FString("OnFinish")+State.ToString().Replace(TEXT(" "),TEXT("")));
		//
		if (BP) {
			bool OnExit_Exists = false;
			bool OnBegin_Exists = false;
			bool OnUpdate_Exists = false;
			bool OnFinish_Exists = false;
			//
			TArray<UEdGraph*>Graphs; BP->GetAllGraphs(Graphs);
			for (const auto &GR : Graphs) {
				if (GR->GetName().Replace(TEXT(" "),TEXT(""))==NID_OnExit) {OnExit_Exists = true;}
				if (GR->GetName().Replace(TEXT(" "),TEXT(""))==NID_OnBegin) {OnBegin_Exists = true;}
				if (GR->GetName().Replace(TEXT(" "),TEXT(""))==NID_OnUpdate) {OnUpdate_Exists = true;}
				if (GR->GetName().Replace(TEXT(" "),TEXT(""))==NID_OnFinish) {OnFinish_Exists = true;}
			}///
			//
			if (!OnBegin_Exists) {
				UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(BP,*NID_OnBegin,UEdGraph::StaticClass(),UEdGraphSchema_K2::StaticClass());
				FBlueprintEditorUtils::AddFunctionGraph(BP,Graph,false,UStateMachineComponent::StaticClass());
				Graph->bAllowRenaming = false; Graph->bAllowDeletion = true; Graph->bEditable = true;
				//
				TArray<UK2Node_FunctionEntry*> UK2_Nodes;
				Graph->GetNodesOfClass(UK2_Nodes);
				//
				for (const auto &Function : UK2_Nodes) {
					if (Function->GetNodeTitle(ENodeTitleType::MenuTitle).ToString().Replace(TEXT(" "),TEXT(""))==NID_OnBegin) {
						Function->CreateUserDefinedPinsForFunctionEntryExit(OBJ->FindFunctionChecked(TEXT("FSM_BeginArgs")),true);
						Function->AddExtraFlags((FUNC_BlueprintEvent|FUNC_BlueprintCallable|FUNC_Public));
						FKismetUserDeclaredFunctionMetadata Meta;
						  Meta.ToolTip = FText::FromString(TEXT("'On Begin'  function executed when FSM's [")+State.ToString()+FString("] State is set active."));
						  Meta.Category = FText::FromString(*(FString(TEXT("FSM: "))+State.ToString()));
						  Meta.Keywords = FText::FromString(TEXT("FSM Begin State"));
						  Meta.InstanceTitleColor = FLinearColor::Yellow;
						  Meta.HasLatentFunctions = false;
						  Meta.bCallInEditor = false;
			Function->MetaData = Meta;}}}
			//
			if (!OnUpdate_Exists) {
				UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(BP,*NID_OnUpdate,UEdGraph::StaticClass(),UEdGraphSchema_K2::StaticClass());
				FBlueprintEditorUtils::AddFunctionGraph(BP,Graph,false,UStateMachineComponent::StaticClass());
				Graph->bAllowRenaming = false; Graph->bAllowDeletion = true; Graph->bEditable = true;
				//
				TArray<UK2Node_FunctionEntry*> UK2_Nodes;
				Graph->GetNodesOfClass(UK2_Nodes);
				//
				for (const auto &Function : UK2_Nodes) {
					if (Function->GetNodeTitle(ENodeTitleType::MenuTitle).ToString().Replace(TEXT(" "),TEXT(""))==NID_OnUpdate) {
						Function->CreateUserDefinedPinsForFunctionEntryExit(OBJ->FindFunctionChecked(TEXT("FSM_UpdateArgs")),true);
						Function->AddExtraFlags((FUNC_BlueprintEvent|FUNC_BlueprintCallable|FUNC_Public));
						FKismetUserDeclaredFunctionMetadata Meta;
						  Meta.ToolTip = FText::FromString(TEXT("'On Update'  function executed when FSM's [")+State.ToString()+FString("] State is active and ticking."));
						  Meta.Category = FText::FromString(*(FString(TEXT("FSM: "))+State.ToString()));
						  Meta.Keywords = FText::FromString(TEXT("FSM Update State"));
						  Meta.InstanceTitleColor = FLinearColor::Yellow;
						  Meta.HasLatentFunctions = false;
						  Meta.bCallInEditor = false;
			Function->MetaData = Meta;}}}
			//
			if (!OnExit_Exists) {
				UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(BP,*NID_OnExit,UEdGraph::StaticClass(),UEdGraphSchema_K2::StaticClass());
				FBlueprintEditorUtils::AddFunctionGraph(BP,Graph,false,UStateMachineComponent::StaticClass());
				Graph->bAllowRenaming = false; Graph->bAllowDeletion = true; Graph->bEditable = true;
				//
				TArray<UK2Node_FunctionEntry*> UK2_Nodes;
				Graph->GetNodesOfClass(UK2_Nodes);
				//
				for (const auto &Function : UK2_Nodes) {
					if (Function->GetNodeTitle(ENodeTitleType::MenuTitle).ToString().Replace(TEXT(" "),TEXT(""))==NID_OnExit) {
						Function->CreateUserDefinedPinsForFunctionEntryExit(OBJ->FindFunctionChecked(TEXT("FSM_ExitArgs")),true);
						Function->AddExtraFlags((FUNC_BlueprintEvent|FUNC_BlueprintCallable|FUNC_Public));
						FKismetUserDeclaredFunctionMetadata Meta;
						  Meta.ToolTip = FText::FromString(TEXT("'On Exit'  function executed when FSM's [")+State.ToString()+FString("] State is set inactive and stop ticking."));
						  Meta.Category = FText::FromString(*(FString(TEXT("FSM: "))+State.ToString()));
						  Meta.Keywords = FText::FromString(TEXT("FSM Exit State"));
						  Meta.InstanceTitleColor = FLinearColor::Yellow;
						  Meta.HasLatentFunctions = false;
						  Meta.bCallInEditor = false;
			Function->MetaData = Meta;}}}
			//
			if (!OnFinish_Exists) {
				UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(BP,*NID_OnFinish,UEdGraph::StaticClass(),UEdGraphSchema_K2::StaticClass());
				FBlueprintEditorUtils::AddFunctionGraph(BP,Graph,false,UStateMachineComponent::StaticClass());
				Graph->bAllowRenaming = false; Graph->bAllowDeletion = true; Graph->bEditable = true;
				//
				TArray<UK2Node_FunctionEntry*> UK2_Nodes;
				Graph->GetNodesOfClass(UK2_Nodes);
				//
				for (const auto &Function : UK2_Nodes) {
					if (Function->GetNodeTitle(ENodeTitleType::MenuTitle).ToString().Replace(TEXT(" "),TEXT(""))==NID_OnFinish) {
						Function->CreateUserDefinedPinsForFunctionEntryExit(OBJ->FindFunctionChecked(TEXT("FSM_ExitArgs")),true);
						Function->AddExtraFlags((FUNC_BlueprintEvent|FUNC_BlueprintCallable|FUNC_Public));
						FKismetUserDeclaredFunctionMetadata Meta;
						  Meta.ToolTip = FText::FromString(TEXT("'On Finish'  function executed when FSM's [")+State.ToString()+FString("] State exits by calling 'Finish State'."));
						  Meta.Category = FText::FromString(*(FString(TEXT("FSM: "))+State.ToString()));
						  Meta.Keywords = FText::FromString(TEXT("FSM Finish State"));
						  Meta.InstanceTitleColor = FLinearColor::Yellow;
						  Meta.HasLatentFunctions = false;
						  Meta.bCallInEditor = false;
			Function->MetaData = Meta;}}}
	}} return FReply::Handled();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FSMEditor,UFSMEditor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////