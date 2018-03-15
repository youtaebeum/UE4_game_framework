/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UFSMStateMachineComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "UFSMBTTasks.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UStateMachineComponent;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// FSM: Behavior Tree Task, Link to the FSM Component: Add State
UCLASS(Category = "FSM", hideCategories = (Blackboard, Task), meta = (DisplayName = "FSM: Add State", ShortTooltip = "FSM: Add State to Component."))
class UFSM_API UBTTask_FSM_AddState : public UBTTask_BlackboardBase {
	GENERATED_UCLASS_BODY()
private:
	/// FSM Component.
	UPROPERTY()
	UStateMachineComponent* FSM;
	//
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
public:
	/// Name of target FSM Component to link.
	/// If None, Task will link to first FSM found.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName TargetFSM = NAME_None;
	/// Name for New State to Add.
	/// If State is added, Task will succeed.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName NewStateName = NAME_None;
	//
	/// Enables Debug Logs.
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool Debug;
	/// Forces Task succeed even if its actions have failed to execute.
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool ForceSuccess;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// FSM: Behavior Tree Task, Link to the FSM Component: Set State
UCLASS(Category = "FSM", hideCategories = (Blackboard, Task), meta = (DisplayName = "FSM: Set State", ShortTooltip = "FSM: Set State of Component."))
class UFSM_API UBTTask_FSM_SetState : public UBTTask_BlackboardBase {
	GENERATED_UCLASS_BODY()
private:
	/// FSM Component.
	UPROPERTY()
	UStateMachineComponent* FSM;
	//
	/// BT API: Execute Task.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
	/// BT API: Task Description.
	virtual FString GetStaticDescription() const override;
public:
	/// Name of target FSM Component to link.
	/// If None, Task will link to first FSM found.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName TargetFSM = NAME_None;
	/// Name of State to Set.
	/// If State is found and set active, Task will succeed.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName StateName;
	//
	/// Enables Debug Logs.
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool Debug;
	/// Forces Task to succeed even if its actions have failed to execute.
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool ForceSuccess;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// FSM: Behavior Tree Task, Link to the FSM Component: Remove State
UCLASS(Category = "FSM", hideCategories = (Blackboard, Task), meta = (DisplayName = "FSM: Remove State", ShortTooltip = "FSM: Remove State to Component."))
class UFSM_API UBTTask_FSM_RemoveState : public UBTTask_BlackboardBase {
	GENERATED_UCLASS_BODY()
private:
	/// FSM Component.
	UPROPERTY()
	UStateMachineComponent* FSM;
	//
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
public:
	/// Name of target FSM Component to link.
	/// If None, Task will link to first FSM found.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName TargetFSM = NAME_None;
	/// Name of State to Remove.
	/// If State is removed, Task will succeed.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName StateName;
	//
	/// Enables Debug Logs.
	UPROPERTY(Category = "FSM", EditAnywhere)
	bool Debug;
	/// Forces Task to succeed even if its actions have failed to execute.
	UPROPERTY(Category = "FSM", EditAnywhere)
	bool ForceSuccess;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// FSM: Behavior Tree Task, Link to the FSM Component: Check State
UCLASS(Category = "FSM", hideCategories = (Blackboard, Task), meta = (DisplayName = "FSM: Check State", ShortTooltip = "FSM: Check State in Component. Succeed if Active State equals given State Name."))
class UFSM_API UBTTask_FSM_CheckState : public UBTTask_BlackboardBase {
	GENERATED_UCLASS_BODY()
private:
	/// FSM Component.
	UPROPERTY()
	UStateMachineComponent* FSM;
	//
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
public:
	/// Name of target FSM Component to link.
	/// If None, Task will link to first FSM found.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName TargetFSM = NAME_None;
	/// Name of State to Check as Active in FSM Component.
	/// If Active State Name matches, Task will succeed.
	UPROPERTY(Category = "FSM", EditAnywhere)
	FName StateName;
	//
	/// Enables Debug Logs.
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool Debug;
	/// Forces Task to succeed even if its actions have failed to execute.
	UPROPERTY(Category = "Settings", EditAnywhere)
	bool ForceSuccess;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////