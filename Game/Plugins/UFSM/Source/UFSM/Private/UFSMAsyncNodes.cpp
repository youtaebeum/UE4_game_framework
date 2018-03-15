/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMAsyncNodes.h"
#include "Private/UFSMShared.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetState_Async* UFSM_SetState_Async::FSM_SetState_Async(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, bool Loop) {
	UFSM_SetState_Async* OBJ = NewObject<UFSM_SetState_Async>();
	//
	OBJ->Target = FSM;
	OBJ->Repeat = Loop;
	OBJ->StateName = Name;
	OBJ->Timer = WaitForSeconds;
	//
	return OBJ;
}

void UFSM_SetState_Async::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if (StateName.IsNone()) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State Name to set is 'None'."),ELogVerbosity::Warning); return;}
	if (Timer <= 0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetState_Async::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetState_Async::Execute() {
	FSM_Transition Transition;
	Target->SetState(StateName,Transition);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetStateID_Async* UFSM_SetStateID_Async::FSM_SetStateID_Async(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, bool Loop) {
	UFSM_SetStateID_Async* OBJ = NewObject<UFSM_SetStateID_Async>();
	//
	OBJ->Target = FSM;
	OBJ->Repeat = Loop;
	OBJ->StateID = ID;
	OBJ->Timer = WaitForSeconds;
	//
	return OBJ;
}

void UFSM_SetStateID_Async::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if ((StateID<0)||(StateID>255)) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State ID is of invalid index: (smaller than 0 or greater than 255)."),ELogVerbosity::Warning); return;}
	if (Timer<=0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetStateID_Async::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetStateID_Async::Execute() {
	FSM_Transition Transition;
	Target->SetStateID(StateID,Transition);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetState_ConditionalAsync* UFSM_SetState_ConditionalAsync::FSM_SetState_ConditionalAsync(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, FSM_Transition Condition, bool Loop) {
	UFSM_SetState_ConditionalAsync* OBJ = NewObject<UFSM_SetState_ConditionalAsync>();
	//
	OBJ->Target = FSM;
	OBJ->Repeat = Loop;
	OBJ->StateName = Name;
	OBJ->Timer = WaitForSeconds;
	OBJ->Conditional = Condition;
	//
	return OBJ;
}

void UFSM_SetState_ConditionalAsync::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if (StateName.IsNone()) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State Name to set is 'None'."),ELogVerbosity::Warning); return;}
	if (Timer<=0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetState_ConditionalAsync::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetState_ConditionalAsync::Execute() {
	FSM_Transition Transition;
	Target->SetState_Conditional(StateName,Conditional,Transition);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetStateID_ConditionalAsync* UFSM_SetStateID_ConditionalAsync::FSM_SetStateID_ConditionalAsync(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, FSM_Transition Condition, bool Loop) {
	UFSM_SetStateID_ConditionalAsync* OBJ = NewObject<UFSM_SetStateID_ConditionalAsync>();
	//
	OBJ->Target = FSM;
	OBJ->StateID = ID;
	OBJ->Repeat = Loop;
	OBJ->Timer = WaitForSeconds;
	OBJ->Conditional = Condition;
	//
	return OBJ;
}

void UFSM_SetStateID_ConditionalAsync::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if ((StateID<0)||(StateID>255)) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State ID is of invalid index: (smaller than 0 or greater than 255)."),ELogVerbosity::Warning); return;}
	if (Timer<=0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetStateID_ConditionalAsync::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetStateID_ConditionalAsync::Execute() {
	FSM_Transition Transition;
	Target->SetStateID_Conditional(StateID,Conditional,Transition);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetState_AsyncWithCallbacks* UFSM_SetState_AsyncWithCallbacks::FSM_SetState_AsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	UFSM_SetState_AsyncWithCallbacks* OBJ = NewObject<UFSM_SetState_AsyncWithCallbacks>();
	//
	OBJ->Target = FSM;
	OBJ->Repeat = Loop;
	OBJ->StateName = Name;
	OBJ->Timer = WaitForSeconds;
	OBJ->InBegin = BeginNextStateCallback;
	OBJ->InExit = ExitPreviousStateCallback;
	//
	return OBJ;
}

void UFSM_SetState_AsyncWithCallbacks::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if (StateName.IsNone()) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State Name to set is 'None'."),ELogVerbosity::Warning); return;}
	if (Timer <= 0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetState_AsyncWithCallbacks::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetState_AsyncWithCallbacks::Execute() {
	FSM_Transition Transition;
	Target->SetStateWithCallbacks(StateName,Transition,InExit,InBegin);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetStateID_AsyncWithCallbacks* UFSM_SetStateID_AsyncWithCallbacks::FSM_SetStateID_AsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	UFSM_SetStateID_AsyncWithCallbacks* OBJ = NewObject<UFSM_SetStateID_AsyncWithCallbacks>();
	//
	OBJ->Target = FSM;
	OBJ->Repeat = Loop;
	OBJ->StateID = ID;
	OBJ->Timer = WaitForSeconds;
	OBJ->InBegin = BeginNextStateCallback;
	OBJ->InExit = ExitPreviousStateCallback;
	//
	return OBJ;
}

void UFSM_SetStateID_AsyncWithCallbacks::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if ((StateID<0)||(StateID>255)) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State ID is of invalid index: (smaller than 0 or greater than 255)."),ELogVerbosity::Warning); return;}
	if (Timer <= 0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetStateID_AsyncWithCallbacks::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetStateID_AsyncWithCallbacks::Execute() {
	FSM_Transition Transition;
	Target->SetStateIDWithCallbacks(StateID,Transition,InExit,InBegin);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetState_ConditionalAsyncWithCallbacks* UFSM_SetState_ConditionalAsyncWithCallbacks::FSM_SetState_ConditionalAsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, FSM_Transition Condition, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	UFSM_SetState_ConditionalAsyncWithCallbacks* OBJ = NewObject<UFSM_SetState_ConditionalAsyncWithCallbacks>();
	//
	OBJ->Target = FSM;
	OBJ->Repeat = Loop;
	OBJ->StateName = Name;
	OBJ->Timer = WaitForSeconds;
	OBJ->Conditional = Condition;
	OBJ->InBegin = BeginNextStateCallback;
	OBJ->InExit = ExitPreviousStateCallback;
	//
	return OBJ;
}

void UFSM_SetState_ConditionalAsyncWithCallbacks::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if (StateName.IsNone()) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State Name to set is 'None'."),ELogVerbosity::Warning); return;}
	if (Timer <= 0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetState_ConditionalAsyncWithCallbacks::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetState_ConditionalAsyncWithCallbacks::Execute() {
	FSM_Transition Transition;
	Target->SetState_ConditionalWithCallbacks(StateName,Conditional,Transition,InExit,InBegin);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UFSM_SetStateID_ConditionalAsyncWithCallbacks* UFSM_SetStateID_ConditionalAsyncWithCallbacks::FSM_SetStateID_ConditionalAsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, FSM_Transition Condition, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	UFSM_SetStateID_ConditionalAsyncWithCallbacks* OBJ = NewObject<UFSM_SetStateID_ConditionalAsyncWithCallbacks>();
	//
	OBJ->Target = FSM;
	OBJ->Repeat = Loop;
	OBJ->StateID = ID;
	OBJ->Timer = WaitForSeconds;
	OBJ->Conditional = Condition;
	OBJ->InBegin = BeginNextStateCallback;
	OBJ->InExit = ExitPreviousStateCallback;
	//
	return OBJ;
}

void UFSM_SetStateID_ConditionalAsyncWithCallbacks::Activate() {
	if (Target==nullptr) {FFrame::KismetExecutionMessage(TEXT("FSM: Invalid FSM Component. Cannot Set State for FSM."),ELogVerbosity::Error); return;}
	if ((StateID<0)||(StateID>255)) {FFrame::KismetExecutionMessage(TEXT("FSM: 'Set State Async' Node's State ID is of invalid index: (smaller than 0 or greater than 255)."),ELogVerbosity::Warning); return;}
	if (Timer <= 0.0f) {Timer = 0.2f;}
	//
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UFSM_SetStateID_ConditionalAsyncWithCallbacks::Execute);
	Target->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,Timer,Repeat);
}

void UFSM_SetStateID_ConditionalAsyncWithCallbacks::Execute() {
	FSM_Transition Transition;
	Target->SetStateID_ConditionalWithCallbacks(StateID,Conditional,Transition,InExit,InBegin);
	//
	switch (Transition) {
		case FSM_Transition::Succeeded:
			OnSuccess.Broadcast(); break;
		case FSM_Transition::Unchecked:
		case FSM_Transition::Aborted:
			OnAborted.Broadcast(); break;
	default: break;}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////