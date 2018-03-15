/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMAnimNodes.h"
#include "Private/UFSMShared.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimNode_StateMachine.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FSM_BlendStateMachines

FSM_BlendStateMachines::FSM_BlendStateMachines() : FAnimNode_Base() {}

void FSM_BlendStateMachines::Initialize_AnyThread(const FAnimationInitializeContext &Context) {
	Super::Initialize_AnyThread(Context);
	//
	BasePose.Initialize(Context);
	BlendPose.Initialize(Context);
}

void FSM_BlendStateMachines::Update_AnyThread(const FAnimationUpdateContext &Context) {
	BasePose.Update(Context);
	BlendPose.Update(Context);
}

void FSM_BlendStateMachines::Evaluate_AnyThread(FPoseContext &Output) {
	BasePose.Evaluate(Output);
}

void FSM_BlendStateMachines::CacheBones_AnyThread(const FAnimationCacheBonesContext &Context)  {
	BasePose.CacheBones(Context);
	BlendPose.CacheBones(Context);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FSM_SetMachineState

FSM_SetMachineState::FSM_SetMachineState() : FAnimNode_Base() {
	EveryFrame = false;
}

void FSM_SetMachineState::Initialize_AnyThread(const FAnimationInitializeContext &Context) {
	Super::Initialize_AnyThread(Context);
	BasePose.Initialize(Context);
	//
	if (!Context.AnimInstanceProxy) {return;}
	//
	const auto &FSMC = GetFSMComponentByName(Context,FSM);
	const auto &ASMI = Context.AnimInstanceProxy->GetStateMachineInstanceFromName(ASM);
	//
	if (FSMC && ASMI) {
		const auto &SMD = Context.AnimInstanceProxy->GetMachineDescription(Context.AnimInstanceProxy->GetAnimClassInterface(),ASMI);
		if (SMD) {FSMC->SetState(ASMI->GetCurrentState());}
	}
}

void FSM_SetMachineState::Update_AnyThread(const FAnimationUpdateContext &Context) {
	BasePose.Update(Context);
	//
	if (!EveryFrame) {return;}
	//
	const auto &FSMC = GetFSMComponentByName(Context,FSM);
	const auto &ASMI = Context.AnimInstanceProxy->GetStateMachineInstanceFromName(ASM);
	//
	if (FSMC && ASMI && (ASMI->GetCurrentStateName().Compare(FSMC->GetActiveState().Name)!=0)) {
		const auto &SMD = Context.AnimInstanceProxy->GetMachineDescription(Context.AnimInstanceProxy->GetAnimClassInterface(),ASMI);
	if (SMD) {FSMC->SetState(ASMI->GetCurrentState());}}
}

void FSM_SetMachineState::Evaluate_AnyThread(FPoseContext &Output) {
	BasePose.Evaluate(Output);
}

void FSM_SetMachineState::CacheBones_AnyThread(const FAnimationCacheBonesContext & Context)  {
	BasePose.CacheBones(Context);
}

UStateMachineComponent* FSM_SetMachineState::GetFSMComponentByName(const FAnimationInitializeContext &Context, FName Name) const {
	if (!Context.AnimInstanceProxy||!Context.AnimInstanceProxy->GetSkelMeshComponent()) {return nullptr;}
	const auto &Owner = Context.AnimInstanceProxy->GetSkelMeshComponent()->GetOwner();
	//
	if (Owner) {
		for (const auto &UAC : Owner->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
			if (UAC->GetFName().Compare(Name)!=0) {continue;}
		return Cast<UStateMachineComponent>(UAC);}
	return nullptr;}
	//
	return nullptr;
}

UStateMachineComponent* FSM_SetMachineState::GetFSMComponentByName(const FAnimationUpdateContext &Context, FName Name) const {
	if (!Context.AnimInstanceProxy||!Context.AnimInstanceProxy->GetSkelMeshComponent()) {return nullptr;}
	const auto &Owner = Context.AnimInstanceProxy->GetSkelMeshComponent()->GetOwner();
	//
	if (Owner) {
		for (const auto &UAC : Owner->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
			if (UAC->GetFName().Compare(Name)!=0) {continue;}
		return Cast<UStateMachineComponent>(UAC);}
	return nullptr;}
	//
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FSM_GetMachineState

/*FSM_GetMachineState::FSM_GetMachineState() : FAnimNode_Base() {
	EveryFrame = false;
}

void FSM_GetMachineState::Initialize_AnyThread(const FAnimationInitializeContext &Context) {
	Super::Initialize_AnyThread(Context);
	BasePose.Initialize(Context);
	//
	if (!Context.AnimInstanceProxy) {return;}
	//
	const auto &FSMC = GetFSMComponentByName(Context,FSM);
	const auto &ASMI = Context.AnimInstanceProxy->GetStateMachineInstanceFromName(ASM);
	//
	if (FSMC && ASMI) {ASMI->SetState(Context,(int32)FSMC->GetCurrentStateID());} // Epic is blocking this functionality for no reason.
}

void FSM_GetMachineState::Update_AnyThread(const FAnimationUpdateContext &Context) {
	BasePose.Update(Context);
	//
	if (!EveryFrame) {return;}
	//
	const auto &FSMC = GetFSMComponentByName(Context,FSM);
	const auto &ASMI = Context.AnimInstanceProxy->GetStateMachineInstanceFromName(ASM);
	//
	if (FSMC && ASMI && (ASMI->GetCurrentStateName().Compare(FSMC->GetActiveState().Name)!=0)) {ASMI->SetState(Context,(int32)FSMC->GetCurrentStateID());}
}

void FSM_GetMachineState::Evaluate_AnyThread(FPoseContext &Output) {
	BasePose.Evaluate(Output);
}

void FSM_GetMachineState::CacheBones_AnyThread(const FAnimationCacheBonesContext &Context)  {
	BasePose.CacheBones(Context);
}

UStateMachineComponent* FSM_GetMachineState::GetFSMComponentByName(const FAnimationInitializeContext &Context, FName Name) const {
	if (!Context.AnimInstanceProxy||!Context.AnimInstanceProxy->GetSkelMeshComponent()) {return nullptr;}
	const auto &Owner = Context.AnimInstanceProxy->GetSkelMeshComponent()->GetOwner();
	//
	if (Owner) {
		for (const auto &UAC : Owner->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
			if (UAC->GetFName().Compare(Name)!=0) {continue;}
		return Cast<UStateMachineComponent>(UAC);}
	return nullptr;}
	//
	return nullptr;
}

UStateMachineComponent* FSM_GetMachineState::GetFSMComponentByName(const FAnimationUpdateContext &Context, FName Name) const {
	if (!Context.AnimInstanceProxy||!Context.AnimInstanceProxy->GetSkelMeshComponent()) {return nullptr;}
	const auto &Owner = Context.AnimInstanceProxy->GetSkelMeshComponent()->GetOwner();
	//
	if (Owner) {
		for (const auto &UAC : Owner->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
			if (UAC->GetFName().Compare(Name)!=0) {continue;}
		return Cast<UStateMachineComponent>(UAC);}
	return nullptr;}
	//
	return nullptr;
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////