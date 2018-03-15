/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UFSMStateMachineComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/BlueprintAsyncActionBase.h"

#include "UFSMAsyncNodes.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSM_Result);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetState_Async : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	FName StateName;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	/// Exits Previous State and Begins New State set by Name (Asynchronous).
	/// This function cannot be used outside of a FSM Component Blueprint Graph.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State [Async]", Keywords = "FSM Set State Async"))
	static UFSM_SetState_Async* FSM_SetState_Async(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, bool Loop);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetStateID_Async : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	uint8 StateID;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State ID [Async]", Keywords = "FSM Set State ID Async"))
	static UFSM_SetStateID_Async* FSM_SetStateID_Async(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, bool Loop);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetState_ConditionalAsync : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	FSM_Transition Conditional;
	//
	UPROPERTY()
	FName StateName;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State (Conditional) [Async]", Keywords = "FSM Set State Async"))
	static UFSM_SetState_ConditionalAsync* FSM_SetState_ConditionalAsync(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, FSM_Transition Condition, bool Loop);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetStateID_ConditionalAsync : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	FSM_Transition Conditional;
	//
	UPROPERTY()
	uint8 StateID;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State ID (Conditional) [Async]", Keywords = "FSM Set State ID Async"))
	static UFSM_SetStateID_ConditionalAsync* FSM_SetStateID_ConditionalAsync(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, FSM_Transition Condition, bool Loop);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetState_AsyncWithCallbacks : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	FSM_BP_BeginState InBegin;
	//
	UPROPERTY()
	FSM_BP_ExitState InExit;
	//
	UPROPERTY()
	FName StateName;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State [Async] [+Callbacks]", Keywords = "FSM Set State Async"))
	static UFSM_SetState_AsyncWithCallbacks* FSM_SetState_AsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetStateID_AsyncWithCallbacks : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	FSM_BP_BeginState InBegin;
	//
	UPROPERTY()
	FSM_BP_ExitState InExit;
	//
	UPROPERTY()
	uint8 StateID;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State ID [Async] [+Callbacks]", Keywords = "FSM Set State ID Async"))
	static UFSM_SetStateID_AsyncWithCallbacks* FSM_SetStateID_AsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetState_ConditionalAsyncWithCallbacks : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	FSM_Transition Conditional;
	//
	UPROPERTY()
	FSM_BP_BeginState InBegin;
	//
	UPROPERTY()
	FSM_BP_ExitState InExit;
	//
	UPROPERTY()
	FName StateName;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State (Conditional) [Async] [+Callbacks]", Keywords = "FSM Set State Async"))
	static UFSM_SetState_ConditionalAsyncWithCallbacks* FSM_SetState_ConditionalAsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const FName Name, FSM_Transition Condition, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_SetStateID_ConditionalAsyncWithCallbacks : public UBlueprintAsyncActionBase {
	GENERATED_BODY()
private:
	UPROPERTY()
	UStateMachineComponent* Target;
	//
	UPROPERTY()
	FSM_Transition Conditional;
	//
	UPROPERTY()
	FSM_BP_BeginState InBegin;
	//
	UPROPERTY()
	FSM_BP_ExitState InExit;
	//
	UPROPERTY()
	uint8 StateID;
	//
	UPROPERTY()
	float Timer;
	//
	UPROPERTY()
	bool Repeat;
	//
	UPROPERTY()
	FTimerHandle TimerHandle;
	//
	UFUNCTION()
	void Execute();
public:
	virtual void Activate() override;
	//
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnSuccess;
	//
	UPROPERTY(Category = "FSM", BlueprintAssignable)
	FSM_Result OnAborted;
	//
	//
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "FSM", DisplayName = "FSM: Set State ID (Conditional) [Async] [+Callbacks]", Keywords = "FSM Set State ID Async"))
	static UFSM_SetStateID_ConditionalAsyncWithCallbacks* FSM_SetStateID_ConditionalAsyncWithCallbacks(UStateMachineComponent* FSM, const float WaitForSeconds, const uint8 ID, FSM_Transition Condition, bool Loop, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////