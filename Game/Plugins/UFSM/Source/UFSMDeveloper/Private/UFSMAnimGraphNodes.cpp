/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMAnimGraphNodes.h"

#include "AnimationRuntime.h"
#include "ScopedTransaction.h"
#include "GraphEditorActions.h"
#include "AnimationGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_BlendStateMachines_AnimGraphNode::UFSM_BlendStateMachines_AnimGraphNode(const FObjectInitializer& OBJ) : Super(OBJ) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FText UFSM_BlendStateMachines_AnimGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	return GetControllerDescription();
}

FLinearColor UFSM_BlendStateMachines_AnimGraphNode::GetNodeTitleColor() const {
	return FLinearColor(0.75f,0.75f,0.1f,1.f);
}

FString UFSM_BlendStateMachines_AnimGraphNode::GetNodeCategory() const {
	return FString("FSM");
}

FText UFSM_BlendStateMachines_AnimGraphNode::GetControllerDescription() const {
	return LOCTEXT("FSM_BlendStateMachines_Node", "FSM: Blend State Machines");
}

FText UFSM_BlendStateMachines_AnimGraphNode::GetTooltipText() const {
	return LOCTEXT("FSM_BlendStateMachines_Tooltip", "FSM: Easy blends execution of two Animation State Machines or Cached Poses without modifying bone weights.");
}

bool UFSM_BlendStateMachines_AnimGraphNode::ShowPaletteIconOnNode() const {
	return true;
}

FName UFSM_BlendStateMachines_AnimGraphNode::GetCornerIcon() const {
	return TEXT("");
}

FSlateIcon UFSM_BlendStateMachines_AnimGraphNode::GetIconAndTint(FLinearColor &OutColor) const {
	static FSlateIcon Icon(TEXT("FSMStyle"),TEXT("ClassIcon.StateMachineComponent"));
	OutColor = FLinearColor::White; return Icon;
}

FString UFSM_BlendStateMachines_AnimGraphNode::GetDocumentationLink() const {
	return TEXT("Shared/GraphNodes/Animation");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetMachineState_AnimGraphNode::UFSM_SetMachineState_AnimGraphNode(const FObjectInitializer& OBJ) : Super(OBJ) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FText UFSM_SetMachineState_AnimGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	return GetControllerDescription();
}

FLinearColor UFSM_SetMachineState_AnimGraphNode::GetNodeTitleColor() const {
	return FLinearColor(1.0f,0.f,0.f,1.f);
}

FString UFSM_SetMachineState_AnimGraphNode::GetNodeCategory() const {
	return FString("FSM");
}

FText UFSM_SetMachineState_AnimGraphNode::GetControllerDescription() const {
	return LOCTEXT("FSM_SetState_Node", "FSM: Set State");
}

FText UFSM_SetMachineState_AnimGraphNode::GetTooltipText() const {
	return LOCTEXT("FSM_SetState_Tooltip", "FSM: Set State of Named FSM Component on Actor Owner of this Animation Graph Blueprint.");
}

bool UFSM_SetMachineState_AnimGraphNode::ShowPaletteIconOnNode() const {
	return true;
}

FName UFSM_SetMachineState_AnimGraphNode::GetCornerIcon() const {
	return TEXT("GraphEditor.StateMachine_16x");
}

FSlateIcon UFSM_SetMachineState_AnimGraphNode::GetIconAndTint(FLinearColor &OutColor) const {
	static FSlateIcon Icon(TEXT("FSMStyle"),TEXT("ClassIcon.StateMachineComponent"));
	OutColor = FLinearColor::White; return Icon;
}

FString UFSM_SetMachineState_AnimGraphNode::GetDocumentationLink() const {
	return TEXT("Shared/GraphNodes/Animation");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*UFSM_GetMachineState_AnimGraphNode::UFSM_GetMachineState_AnimGraphNode(const FObjectInitializer& OBJ) : Super(OBJ) {}

FText UFSM_GetMachineState_AnimGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	return GetControllerDescription();
}

FLinearColor UFSM_GetMachineState_AnimGraphNode::GetNodeTitleColor() const {
	return FLinearColor(1.0f,0.f,0.f,1.f);
}

FString UFSM_GetMachineState_AnimGraphNode::GetNodeCategory() const {
	return FString("FSM");
}

FText UFSM_GetMachineState_AnimGraphNode::GetControllerDescription() const {
	return LOCTEXT("FSM_GetState_Node", "FSM: Get State");
}

FText UFSM_GetMachineState_AnimGraphNode::GetTooltipText() const {
	return LOCTEXT("FSM_GetState_Tooltip", "FSM: Get State of Named FSM Component on Actor Owner of this Animation Graph Blueprint.");
}

bool UFSM_GetMachineState_AnimGraphNode::ShowPaletteIconOnNode() const {
	return true;
}

FName UFSM_GetMachineState_AnimGraphNode::GetCornerIcon() const {
	return TEXT("GraphEditor.StateMachine_16x");
}

FSlateIcon UFSM_GetMachineState_AnimGraphNode::GetIconAndTint(FLinearColor &OutColor) const {
	static FSlateIcon Icon(TEXT("FSMStyle"),TEXT("ClassIcon.StateMachineComponent"));
	OutColor = FLinearColor::White; return Icon;
}

FString UFSM_GetMachineState_AnimGraphNode::GetDocumentationLink() const {
	return TEXT("Shared/GraphNodes/Animation");
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////