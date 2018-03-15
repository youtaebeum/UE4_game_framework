/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UFSMStateMachineComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimNodeBase.h"

#include "UFSMAnimNodes.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FSM: Blend State Machines

USTRUCT(Category = "FSM", BlueprintType, meta = (DisplayName = "FSM: Blend State Machines", ShortTooltip = "FSM: Easy blend of two Animation State Machines or Cached Poses."))
struct UFSM_API FSM_BlendStateMachines : public FAnimNode_Base {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite)
	FPoseLink BasePose;
	//
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite)
	FPoseLink BlendPose;
public:
	FSM_BlendStateMachines();
	virtual void Evaluate_AnyThread(FPoseContext &Output) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext &Context) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext &Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext &Context) override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FSM: Set State

USTRUCT(Category = "FSM", BlueprintType, meta = (DisplayName = "FSM: Set State", ShortTooltip = "FSM: Set State of Named FSM Component on Actor Owner of this Animation Graph Blueprint."))
struct UFSM_API FSM_SetMachineState : public FAnimNode_Base {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite)
	FPoseLink BasePose;
	//
	/// The Target FSM Component, State Machine, to Set State.
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite, meta=(PinShownByDefault))
	FName FSM;
	//
	/// The Target Animation State Machine Graph, from this Blueprint, to sync State from.
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite, meta=(PinShownByDefault))
	FName ASM;
	//
	/// If enabled, FSM Component will check every frame if Anim Graph's State has changed; if so then States are synchronized again.
	UPROPERTY(Category = "FSM", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta=(PinShownByDefault))
	bool EveryFrame;
private:
	/// FSM Component Initialization.
	UStateMachineComponent* GetFSMComponentByName(const FAnimationInitializeContext &Context, FName Name) const;
	UStateMachineComponent* GetFSMComponentByName(const FAnimationUpdateContext &Context, FName Name) const;
public:
	FSM_SetMachineState();
	//
	virtual void Evaluate_AnyThread(FPoseContext &Output) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext &Context) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext &Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext &Context) override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FSM: Get State

/// Epic is blocking this functionality for no reason.
/*USTRUCT(Category = "FSM", meta = (DisplayName = "FSM: Get State", ShortTooltip = "FSM: Get State of Named FSM Component on Actor Owner of this Animation Graph Blueprint."))
struct UFSM_API FSM_GetMachineState : public FAnimNode_Base {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite)
	FPoseLink BasePose;
	//
	/// The Target FSM Component, State Machine, to sync State from.
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite, meta=(PinShownByDefault))
	FName FSM;
	//
	/// The Target Animation State Machine Graph, from this Blueprint, to Set State.
	UPROPERTY(Category = "FSM", EditAnywhere, BlueprintReadWrite, meta=(PinShownByDefault))
	FName ASM;
	//
	/// If enabled, Animation Graph will check every frame if FSM Component's State has changed; if so then States are synchronized again.
	UPROPERTY(Category = "FSM", AdvancedDisplay, EditAnywhere, BlueprintReadWrite, meta=(PinShownByDefault))
	bool EveryFrame;
private:
	/// FSM Component Initialization.
	UStateMachineComponent* GetFSMComponentByName(const FAnimationInitializeContext &Context, FName Name) const;
	UStateMachineComponent* GetFSMComponentByName(const FAnimationUpdateContext &Context, FName Name) const;
public:
	FSM_GetMachineState();
	//
	virtual void Evaluate_AnyThread(FPoseContext &Output) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext &Context) override;
	virtual void Initialize_AnyThread(const FAnimationInitializeContext &Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext &Context) override;
};*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////