/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMStateMachineABP.h"
#include "Private/UFSMShared.h"
#include "Private/UFSM_Log.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UStateMachineABP::UStateMachineABP(const class FObjectInitializer &OBJ) : Super(OBJ) {
	OverrideFSM = true;
}

UStateMachineComponent* UStateMachineABP::GetFSMComponent() {
	const auto &Owner = TryGetPawnOwner(); if (Owner) {
		return Cast<UStateMachineComponent>(Owner->GetComponentByClass(UStateMachineComponent::StaticClass()));
	} else {return NULL;}
}

UStateMachineComponent* UStateMachineABP::GetFSMComponentByName(FName Name) {
	const auto &Owner = TryGetPawnOwner(); if (Owner) {
		for (const auto &UAC : Owner->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
			if (UAC->GetFName().Compare(Name)!=0) {continue;}
		return Cast<UStateMachineComponent>(UAC);}
	return NULL;} return NULL;
}

void UStateMachineABP::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	FSM_Transition Transition;
	//
	if (TargetFSM == NAME_None) {FSM = GetFSMComponent();}
	else {FSM = GetFSMComponentByName(TargetFSM);}
	const auto &ASM = GetStateMachineInstanceFromName(TargetASM);
	//
	if (FSM && ASM && OverrideFSM) {
		const auto &MD = GetMachineDescription(this->AnimInstanceProxy->GetAnimClassInterface(),ASM);
		if (MD) {
			FSM->Clear(true); uint32 ID = 0;
			for (const auto &State : MD->States) {
				FSM->AddState(ID,State.StateName,Transition);
				FSM->LINKS.Empty(); FSM_Link Link;
				for (const auto &Transit : MD->Transitions) {
					if (Transit.PreviousState != ID) {continue;}
					if (!MD->States.IsValidIndex(Transit.NextState)) {continue;}
					Link.NextState = MD->States[Transit.NextState].StateName;
				break;}
				Link.Function = FName(*FString(FString("OnFinish")+State.StateName.ToString().Replace(TEXT(" "),TEXT(""))));
				FSM->LINKS.Add(State.StateName,Link);
			ID++;}
	FSM->SetState(ASM->GetCurrentState());}}
}

void UStateMachineABP::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	//
	if (FSM && OverrideFSM) {
		const auto &ASM = GetStateMachineInstanceFromName(TargetASM);
		if (ASM && ASM->GetCurrentStateName().Compare(FSM->GetActiveState().Name)!=0)
		{FSM->SetState(ASM->GetCurrentState());}
	} else if (!FSM) {
		if (TargetFSM == NAME_None) {FSM = GetFSMComponent();}
		else {FSM = GetFSMComponentByName(TargetFSM);}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////