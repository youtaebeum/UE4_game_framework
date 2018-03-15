/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMBTTasks.h"
#include "Private/UFSMShared.h"
#include "Private/UFSM_Log.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UBTTask_FSM_AddState::UBTTask_FSM_AddState(const class FObjectInitializer &OBJ) : Super(OBJ) {
	NodeName = "FSM: Add State";
	ForceSuccess = true;
}

EBTNodeResult::Type UBTTask_FSM_AddState::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	//
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	if (TargetFSM == NAME_None) {FSM = Cast<UStateMachineComponent>(Pawn->GetComponentByClass(UStateMachineComponent::StaticClass()));}
	else {for (const auto &UAC : Pawn->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
		if (UAC->GetFName().Compare(TargetFSM)!=0) {continue;}
	FSM = Cast<UStateMachineComponent>(UAC);}}
	//
	if (FSM) {
		if (NewStateName != NAME_None) {
			Result = EBTNodeResult::InProgress;
			if (FSM->Pure_AddState(FSM->GetLastStateID()+1,NewStateName)==FSM_Transition::Succeeded) {
				Result = EBTNodeResult::Succeeded;
				LOG_FSM(Debug,true,1,FColor::Green,NodeName+FString(": Added New FSM State --> ")+NewStateName.ToString());
			} else {
				Result = EBTNodeResult::Failed;
			LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to Add New FSM State: ")+NewStateName.ToString());}
	}} else {
		LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to find FSM Component."));
	Result = EBTNodeResult::Failed;}
	//
	if (ForceSuccess) {Result = EBTNodeResult::Succeeded;} return Result;
}

FString UBTTask_FSM_AddState::GetStaticDescription() const {
	return FString("To Owner Actor's FSM Component.");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UBTTask_FSM_SetState::UBTTask_FSM_SetState(const class FObjectInitializer &OBJ) : Super(OBJ) {
	NodeName = "FSM: Set State";
	ForceSuccess = true;
}

EBTNodeResult::Type UBTTask_FSM_SetState::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	FSM_Transition Transition;
	//
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	if (TargetFSM == NAME_None) {FSM = Cast<UStateMachineComponent>(Pawn->GetComponentByClass(UStateMachineComponent::StaticClass()));}
	else {for (const auto &UAC : Pawn->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
		if (UAC->GetFName().Compare(TargetFSM)!=0) {continue;}
	FSM = Cast<UStateMachineComponent>(UAC);}}
	//
	if (FSM) {
		if (StateName != NAME_None) {
			Result = EBTNodeResult::InProgress;
			FSM->SetState(StateName,Transition);
			if (FSM && Transition==FSM_Transition::Succeeded) {
				Result = EBTNodeResult::Succeeded;
				LOG_FSM(Debug,true,1,FColor::Green,NodeName+FString(": Setting FSM State --> ")+StateName.ToString());
			} else {
				Result = EBTNodeResult::Failed;
			LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to Set FSM State: ")+StateName.ToString());}
	}} else {
		LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to find FSM Component."));
	Result = EBTNodeResult::Failed;}
	//
	if (ForceSuccess) {Result = EBTNodeResult::Succeeded;} return Result;
}

FString UBTTask_FSM_SetState::GetStaticDescription() const {
	return FString("Of Owner Actor's FSM Component.");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UBTTask_FSM_RemoveState::UBTTask_FSM_RemoveState(const class FObjectInitializer &OBJ) : Super(OBJ) {
	NodeName = "FSM: Remove State";
	ForceSuccess = true;
}

EBTNodeResult::Type UBTTask_FSM_RemoveState::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	FSM_Transition Transition;
	//
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	if (TargetFSM == NAME_None) {FSM = Cast<UStateMachineComponent>(Pawn->GetComponentByClass(UStateMachineComponent::StaticClass()));}
	else {for (const auto &UAC : Pawn->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
		if (UAC->GetFName().Compare(TargetFSM)!=0) {continue;}
	FSM = Cast<UStateMachineComponent>(UAC);}}
	//
	if (FSM) {
		if (StateName != NAME_None) {
			Result = EBTNodeResult::InProgress;
			FSM->RemoveState(StateName,Transition);
			if (FSM && Transition==FSM_Transition::Succeeded) {
				Result = EBTNodeResult::Succeeded;
				LOG_FSM(Debug,true,1,FColor::Green,NodeName+FString(": Removing FSM State --> ")+StateName.ToString());
			} else {
				Result = EBTNodeResult::Failed;
			LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to Remove FSM State: ")+StateName.ToString());}
	}} else {
		LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to find FSM Component."));
	Result = EBTNodeResult::Failed;}
	//
	if (ForceSuccess) {Result = EBTNodeResult::Succeeded;} return Result;
}

FString UBTTask_FSM_RemoveState::GetStaticDescription() const {
	return FString("From Owner Actor's FSM Component.");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UBTTask_FSM_CheckState::UBTTask_FSM_CheckState(const class FObjectInitializer &OBJ) : Super(OBJ) {
	NodeName = "FSM: Check State";
	ForceSuccess = true;
}

EBTNodeResult::Type UBTTask_FSM_CheckState::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	//
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) {ForceSuccess ? Result = EBTNodeResult::Succeeded : Result = EBTNodeResult::Aborted; return Result;}
	//
	if (TargetFSM == NAME_None) {FSM = Cast<UStateMachineComponent>(Pawn->GetComponentByClass(UStateMachineComponent::StaticClass()));}
	else {for (const auto &UAC : Pawn->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
		if (UAC->GetFName().Compare(TargetFSM)!=0) {continue;}
	FSM = Cast<UStateMachineComponent>(UAC);}}
	//
	if (FSM) { if (StateName != NAME_None) {
		Result = EBTNodeResult::InProgress;
		if (FSM->FindStateID(StateName) != INDEX_NONE) {
			Result = EBTNodeResult::Succeeded;
			LOG_FSM(Debug,true,1,FColor::Green,NodeName+FString(": FSM State is Valid --> ")+StateName.ToString());
		} else {
			Result = EBTNodeResult::Failed;
		LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to Find FSM State: ")+StateName.ToString());}
	}} else {
		LOG_FSM(Debug,true,1,FColor::Red,NodeName+FString(": Failed to find FSM Component."));
	Result = EBTNodeResult::Failed;}
	//
	if (ForceSuccess) {Result = EBTNodeResult::Succeeded;} return Result;
}

FString UBTTask_FSM_CheckState::GetStaticDescription() const {
	return FString("In Owner Actor's FSM Component.");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////