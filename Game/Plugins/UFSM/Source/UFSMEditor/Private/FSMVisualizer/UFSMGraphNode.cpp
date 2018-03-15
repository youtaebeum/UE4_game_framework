/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMGraphNode.h"
#include "EdGraphSchema_K2.h"
#include "../UFSMEditorShared.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSMGraphNode::UFSMGraphNode(const FObjectInitializer& OBJ) : Super(OBJ), NodeTitle() {
	ParentPin = nullptr;
	ChildPin	= nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UFSMGraphNode::AllocateDefaultPins() {
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	//
	/// Execution Pins.
	ParentPin = CreatePin(EGPD_Input,K2Schema->PC_Exec,TEXT(""),nullptr,false,false,TEXT(""));
	ChildPin = CreatePin(EGPD_Output,K2Schema->PC_Exec,TEXT(""),nullptr,false,false,TEXT(""));
	//
	ParentPin->bHidden = true;
	ChildPin->bHidden = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UFSMGraphNode::SetupNode(const FIntPoint &NodePosition, const FIntPoint &NodeSize, const FString &Title, const UStateMachineComponent* FSMC) {
	if (FSMC) {FSM = FSMC;} ClearStats();
	//
	NodeTitle = Title.Replace(TEXT("_GEN_VARIABLE"),TEXT(""));
	NodeTitle = NodeTitle.Replace(TEXT("Default__"),TEXT("")).Replace(TEXT("_C"),TEXT(""));
	//
	NodePosX = NodePosition.X;
	NodePosY = NodePosition.Y;
	NodeWidth = NodeSize.X;
	NodeHeight = NodeSize.Y;
	//
	AllocateDefaultPins();
	UpdateNode();
}

void UFSMGraphNode::UpdateNode() {
	ClearStats(); FString Title = NodeTitle;
	NodeTitle.Split(TEXT("\n"),&Title,nullptr);
	//
	if (FSM && Title.Contains(TEXT("[ FSM ]"))) {
		ReplicateStateID += (FSM->ReplicateStateID) ? TEXT("True") : TEXT("False");
		ReplicatePreviousStateID += (FSM->ReplicatePreviousStateID) ? TEXT("True") : TEXT("False");
		ReplicateStateTime += (FSM->ReplicateStateTime) ? TEXT("True") : TEXT("False");
		Debug += (FSM->Debug) ? TEXT("True") : TEXT("False");
		//
		NodeTitle = Title+TEXT("\n\n")+TEXT("States: ")+FString::FromInt(FSM->STATES.Num())+TEXT("\n");
		NodeTitle += ReplicateStateID+TEXT("\n")+ReplicatePreviousStateID+TEXT("\n")+ReplicateStateTime+TEXT("\n")+Debug+TEXT("\n");
	} else if (FSM && Title.Contains(TEXT("[ STATE ]"))) {
		IsCurrentState += (FSM->GetStateName()==*(Title.Replace(TEXT("[ STATE ]  "),TEXT("")))) ? TEXT("True") : TEXT("False");
		StateID += FString::FromInt(FSM->FindStateInList(*(Title.Replace(TEXT("[ STATE ]  "),TEXT("")))));
		StateTime += (FSM->GetStateName()==*(Title.Replace(TEXT("[ STATE ]  "),TEXT("")))) ? FString::FromInt(Time) : TEXT("0.00");
		//
		NodeTitle = Title+TEXT("\n")+IsCurrentState+TEXT("\n")+StateID+TEXT("\n")+StateTime+TEXT("\n");
	} else if (FSM && Title.Contains(TEXT("[ LINK ]"))) {
		NodeTitle = Title+TEXT("\n")+TEXT("Default State Transition");
	} else if (FSM && Title.Contains(TEXT("[ FINISH ]"))) {
		NodeTitle = Title+TEXT("\n")+TEXT("Default Link Callback");
	} else if (FSM && Title.Contains(TEXT("[ ERROR ]"))) {
		NodeTitle = Title+TEXT("\n")+TEXT("Linked Callback Function: NOT FOUND!");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UEdGraphPin* UFSMGraphNode::GetParentPin() const {
	return ParentPin;
}

UEdGraphPin* UFSMGraphNode::GetChildPin() const {
	return ChildPin;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UFSMGraphNode::AddChild(UFSMGraphNode* ChildNode) {
	UEdGraphPin* PinParent = ChildNode->GetParentPin();
	if (ensure(PinParent)) {
		ChildPin->MakeLinkTo(PinParent);
	} ChildPin->bHidden = true;
	PinParent->bHidden = true;
}

void UFSMGraphNode::ClearStats() {
	Debug = FString(TEXT("Debug: "));
	StateID = FString(TEXT("State ID: "));
	StateTime = FString(TEXT("State Time: "));
	IsCurrentState = FString(TEXT("Is Current State: "));
	ReplicateStateID = FString(TEXT("Replicate State ID: "));
	ReplicateStateTime = FString(TEXT("Replicate State Time: "));
	ReplicatePreviousStateID = FString(TEXT("Replicate Previous ID: "));
}

FText UFSMGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	return FText::FromString(NodeTitle);
}

FLinearColor UFSMGraphNode::GetNodeTitleColor() const {
	if (NodeTitle.Contains(TEXT("Is Current State: True"))) {return FLinearColor::FromSRGBColor(FColor::Emerald);}
	else if (NodeTitle.Contains(TEXT("[ STATE ]"))) {return FLinearColor::FromSRGBColor(FColor::Red);}
	//
	if (NodeTitle.Contains(TEXT("[ FSM ]"))) {return FLinearColor::FromSRGBColor(FColor::Purple);}
	if (NodeTitle.Contains(TEXT("[ LINK ]"))) {return FLinearColor::FromSRGBColor(FColor::Orange);}
	if (NodeTitle.Contains(TEXT("[ ERROR ]"))) {return FLinearColor::FromSRGBColor(FColor::Black);}
	if (NodeTitle.Contains(TEXT("[ FINISH ]"))) {return FLinearColor::FromSRGBColor(FColor::White);}
	if (NodeTitle.Contains(TEXT("[ FUNCTION ]"))) {return FLinearColor::FromSRGBColor(FColor::Blue);}
	//
	return FLinearColor::Black;
}

FSlateIcon UFSMGraphNode::GetIconAndTint(FLinearColor& OutColor) const {
	if (NodeTitle.Contains(TEXT("Is Current State: True"))) {return FSlateIconFinder::FindIcon("PlayWorld.Simulate.Small");} else
	if (NodeTitle.Contains(TEXT("[ STATE ]"))) {return FSlateIconFinder::FindIcon("GraphEditor.StateMachine_16x");}
	//
	if (NodeTitle.Contains(TEXT("[ ERROR ]"))) {return FSlateIconFinder::FindIcon("Kismet.Status.Error");}
	if (NodeTitle.Contains(TEXT("[ LINK ]"))) {return FSlateIconFinder::FindIcon("GraphEditor.Conduit_16x");}
	if (NodeTitle.Contains(TEXT("[ FSM ]"))) {return FSlateIconFinder::FindIcon("GraphEditor.Sequence_16x");}
	if (NodeTitle.Contains(TEXT("[ FINISH ]"))) {return FSlateIconFinder::FindIcon("GraphEditor.Function_16x");}
	if (NodeTitle.Contains(TEXT("[ FUNCTION ]"))) {return FSlateIconFinder::FindIcon("GraphEditor.Function_16x");}
	//
	return FSlateIconFinder::FindIcon("GraphEditor.Default_16x");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////