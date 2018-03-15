/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Animation/AnimClassInterface.h"
#include "Runtime/Engine/Classes/Animation/AnimNode_StateMachine.h"

#include "UFSMStateMachineABP.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UStateMachineComponent;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// FSM: Animation Instance capable of linking Animation States to a FSM Component
UCLASS(Category = "FSM", meta = (DisplayName = "FSM Animation Blueprint", ShortTooltip = "FSM: Finite State Machine enabled Animation Blueprint."))
class UFSM_API UStateMachineABP : public UAnimInstance {
	GENERATED_UCLASS_BODY()
private:
	/// FSM Component.
	UPROPERTY()
	UStateMachineComponent* FSM;
public:
	/// On Animation Initialize Event.
	virtual void NativeInitializeAnimation() override;
	/// On Animation Update Event.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//
	/// Gets the IAnimClassInterface associated with this Instance, if there's one.
	IAnimClassInterface* GetClassInterface() {return GetProxyOnAnyThread<FAnimInstanceProxy>().GetAnimClassInterface();}
	//
	//
	/// Enables FSM States takeover.
	/// If True, this Animation Blueprint will try to enforce its Machine States to FSM Component, syncing both. 
	/// This operation however erases ALL Event Bindings previously linked by C++ native code, can't be undone.
	UPROPERTY(Category = "FSM", EditAnywhere)
	bool OverrideFSM;
	//
	/// Target FSM Name to link.
	/// If None, Graph will link to first FSM found.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName TargetFSM = NAME_None;
	//
	/// Target Animation State Machine Name within this Instance, to link States from.
	/// This Animation Graph must have a State Machine with this exact Name.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName TargetASM = NAME_None;
	//
	/// Gets the default FSM Component from Actor Blueprint.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM Get Component", Keywords = "FSM Get Component"))
	UStateMachineComponent* GetFSMComponent();
	//
	/// Gets a FSM Component by Name from Actor Blueprint.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM Get Component", Keywords = "FSM Get Component"))
	UStateMachineComponent* GetFSMComponentByName(FName Name);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////