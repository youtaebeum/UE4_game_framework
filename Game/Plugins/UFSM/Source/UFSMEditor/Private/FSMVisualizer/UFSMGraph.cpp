/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMGraph.h"
#include "GraphEditAction.h"
#include "../UFSMEditorShared.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSMGraph::UFSMGraph(const FObjectInitializer& OBJ) : Super(OBJ) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const UBlueprint* GetBlueprint(const UStateMachineComponent* FSMC) {
	const UBlueprint* BP = nullptr;
	//
	if (FSMC != nullptr) {
		BP = Cast<UBlueprint>(FSMC);
		if (BP == nullptr) {
			BP = Cast<UBlueprint>(FSMC->GetClass()->ClassGeneratedBy);
	}} return BP;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UFSMGraph::Initialize(const UStateMachineComponent* FSMC) {
	FSM = FSMC;
	//
	if (FSM) {
		ClearGraph();
		//
		int32 Index = 0; FIntPoint XY = FIntPoint(0,0);
		const auto &Machine = AddNode(*(FString("[ FSM ]  ")+FSM->GetName().Replace(TEXT("_GEN_VARIABLE"),TEXT(""))),XY);
		XY.X += 510;
		//
		for (const auto &ST : FSM->STATES) {
			if (Index %2==0) {XY.Y = FMath::Abs(XY.Y); XY.Y += 150;} else {XY.Y = (-XY.Y); XY.Y -= 150;}
			const auto &State = AddNode(*(FString("[ STATE ]  ")+ST.ToString()),XY);
			Machine->GetChildPin()->MakeLinkTo(State->GetParentPin());
			//
			bool HasLink = false;
			for (const auto &LK : FSM->LINKS) {
				if (LK.Key==ST && (LK.Value.NextState != NAME_None) && (FSM->STATES.Contains(LK.Value.NextState))) {
					XY.X += (State->NodeWidth+140);
					const auto &Link = AddNode(*(FString("[ LINK ]  ")+LK.Value.NextState.ToString()),XY);
					State->GetChildPin()->MakeLinkTo(Link->GetParentPin()); HasLink = true;
					//
					if (LK.Value.Function != NAME_None) {
						XY.X += (Link->NodeWidth+130);
						const auto &Callback = AddNode(*(FString("[ FINISH ]  ")+LK.Value.Function.ToString()),XY);
						Link->GetChildPin()->MakeLinkTo(Callback->GetParentPin());
					XY.X -= (State->NodeWidth+130);}
			XY.X -= (Link->NodeWidth+140);}}
			//
			int32 Y = XY.Y;
			const auto &BP = GetBlueprint(FSMC);
			if (BP) {
				FString NID_OnExit = *(FString("OnExit")+ST.ToString().Replace(TEXT(" "),TEXT("")));
				FString NID_OnBegin = *(FString("OnBegin")+ST.ToString().Replace(TEXT(" "),TEXT("")));
				FString NID_OnUpdate = *(FString("OnUpdate")+ST.ToString().Replace(TEXT(" "),TEXT("")));
				//
				bool OnExit_Exists = false;
				bool OnBegin_Exists = false;
				bool OnUpdate_Exists = false;
				//
				TArray<UEdGraph*>Graphs; BP->GetAllGraphs(Graphs);
				for (const auto &GR : Graphs) {
					if (GR->GetName().Replace(TEXT(" "),TEXT(""))==NID_OnExit) {OnExit_Exists = true;}
					if (GR->GetName().Replace(TEXT(" "),TEXT(""))==NID_OnBegin) {OnBegin_Exists = true;}
					if (GR->GetName().Replace(TEXT(" "),TEXT(""))==NID_OnUpdate) {OnUpdate_Exists = true;}
				}
				//
				if (OnBegin_Exists) {
					if (HasLink) {XY.Y += 80;} XY.X += (State->NodeWidth+140);
					const auto &Function = AddNode(*(FString("[ FUNCTION ]  ")+NID_OnBegin),XY);
					State->GetChildPin()->MakeLinkTo(Function->GetParentPin());
				XY.X -= (State->NodeWidth+140);}
				//
				if (OnUpdate_Exists) {
					XY.X += (State->NodeWidth+140);
					if (OnBegin_Exists) {XY.Y += 62;} else if (HasLink) {XY.Y += 80;}
					const auto &Function = AddNode(*(FString("[ FUNCTION ]  ")+NID_OnUpdate),XY);
					State->GetChildPin()->MakeLinkTo(Function->GetParentPin());
				XY.X -= (State->NodeWidth+140);}
				//
				if (OnExit_Exists) {
					XY.X += (State->NodeWidth+140);
					if (OnBegin_Exists||OnUpdate_Exists) {XY.Y += 62;} else if (HasLink) {XY.Y += 80;}
					const auto &Function = AddNode(*(FString("[ FUNCTION ]  ")+NID_OnExit),XY);
					State->GetChildPin()->MakeLinkTo(Function->GetParentPin());
				XY.X -= (State->NodeWidth+140);}
		XY.Y = Y;}
	Index++;}}
}

void UFSMGraph::UpdateGraph() {
	if (!FSM) {return;}
	//
	for (const auto &N : Nodes) {
		const auto &Node = Cast<UFSMGraphNode>(N);
		if (Node) {
			Node->UpdateNode();
			const auto &BP = GetBlueprint(FSM);
			//
			if (BP && Node->NodeTitle.Contains(TEXT("[ FINISH ]"))) {
				FString Callback; Node->NodeTitle.Split(TEXT("\n"),&Callback,nullptr);
				Callback = Callback.Replace(TEXT("[ FINISH ]  "),TEXT(""));
				bool HasCallback = false;
				//
				TArray<UEdGraph*>Graphs; BP->GetAllGraphs(Graphs);
				for (const auto &GR : Graphs) {if (GR->GetName().Replace(TEXT(" "),TEXT(""))==Callback) {HasCallback = true;}}
				if (!HasCallback) {Node->NodeTitle = Node->NodeTitle.Replace(TEXT("[ FINISH ]  "),TEXT("[ ERROR ]  "));}
			}
			//
			if (BP && Node->NodeTitle.Contains(TEXT("[ ERROR ]"))) {
				FString Callback; Node->NodeTitle.Split(TEXT("\n"),&Callback,nullptr);
				Callback = Callback.Replace(TEXT("[ ERROR ]  "),TEXT(""));
				//
				Node->ErrorMsg = TEXT("Default Link Callback is assigned but ")+Callback+("\nin ")+BP->GetName()+(" is nowhere to be found!");
				Node->bHasCompilerMessage = true;
				//
				bool HasCallback = false;
				TArray<UEdGraph*>Graphs; BP->GetAllGraphs(Graphs);
				for (const auto &GR : Graphs) {if (GR->GetName().Replace(TEXT(" "),TEXT(""))==Callback) {HasCallback = true;}}
				if (HasCallback) {Node->NodeTitle = Node->NodeTitle.Replace(TEXT("[ ERROR ]  "),TEXT("[ FINISH ]  "));}
			} else {Node->bHasCompilerMessage = false; Node->ErrorMsg = TEXT("");}
			//
			if (BP && Node->NodeTitle.Contains(TEXT("[ STATE ]")) && Node->NodeComment==(BP->GetName()+TEXT(" :: Current State")) && !Node->NodeTitle.Contains(TEXT("Is Current State: True"))) {
				Node->bCommentBubblePinned = false;
				Node->bCommentBubbleVisible = false;
				Node->NodeComment = TEXT("");
				SelectNode(Nodes[0]);
			} else if (BP && Node->NodeTitle.Contains(TEXT("[ STATE ]")) && Node->NodeTitle.Contains(TEXT("Is Current State: True")) && Node->NodeComment!=(BP->GetName()+TEXT(" :: Current State"))) {
				Node->NodeComment = BP->GetName()+TEXT(" :: Current State");
				Node->bCommentBubblePinned = true;
				Node->bCommentBubbleVisible = true;
			SelectNode(Node);}
			//
			if (BP && Node->NodeTitle.Contains(TEXT("[ STATE ]")) && Node->NodeTitle.Contains(TEXT("Is Current State: True")) && FSM->StateTime>=(Node->Time+5.f)) {
				Node->Time = FSM->StateTime;
				FEdGraphEditAction Action; Action.Graph = this;
				Action.Action = GRAPHACTION_Default;
				NotifyGraphChanged(Action);
			} else if (BP && Node->NodeTitle.Contains(TEXT("[ STATE ]")) && !Node->NodeTitle.Contains(TEXT("Is Current State: True"))) {Node->Time = 0.f;}
	Node->UpdateNode();}}
}

void UFSMGraph::ClearGraph() {
	TArray<UEdGraphNode*> ToRemove = Nodes;
	for (int32 I=0; I<ToRemove.Num(); ++I) {RemoveNode(ToRemove[I]);}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSMGraphNode* const UFSMGraph::AddNode(const TCHAR* Name, const FIntPoint Position) {
	UFSMGraphNode* const Node = static_cast<UFSMGraphNode*>(CreateNode(UFSMGraphNode::StaticClass(),false,false));
	Node->bCanResizeNode = true; Node->SetupNode(Position,FIntPoint(250,100),Name,FSM); return Node;
}

void UFSMGraph::SelectNode(UEdGraphNode* const Node) {
	check(Node); check(Node->GetGraph()==this)
	//
	FEdGraphEditAction Action;
	Action.Action = GRAPHACTION_SelectNode;
	//
	Selection.Add(Node);
	//
	Action.Graph = this;
	Action.Nodes = Selection;
	//
	NotifyGraphChanged(Action);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
