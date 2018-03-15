/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UFSMBTTasks.h"
#include "UFSMAnimNodes.h"
#include "UFSMAsyncNodes.h"
#include "UFSMStateMachineABP.h"
#include "UFSMStateMachineComponent.h"

#include "UFSMClasses.h"
#include "UFSM.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Default FSM Settings
UCLASS(ClassGroup = Synaptech, Category = "Synaptech", config = Game)
class UFSM_API UFSMSettings : public UObject {
	GENERATED_BODY()
	//
	UFSMSettings();
public:
	/** If enabled, FSM Components will propagate State changes to the FSM Interface API.
	That means that any Blueprint that implements 'FSM Interface' will receive notifications of a State change from all FSM Components present in the Map. */
	UPROPERTY(Category = "General Settings", config, EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Enable State Propagation to 'FSM Interfaces'"))
	bool EnableInterfaceStatePropagation;
	//
	/** If enabled, FSM Components will automatically execute named Blueprint Functions to trigger from correlated State Events.
	This is performed through the UPROPERTY reflection system, if tight performance is a concern then consider using the manual SWITCH method instead. */
	UPROPERTY(Category = "General Settings", config, EditAnywhere, BlueprintReadOnly)
	bool BlueprintAutoFlowFSM;
	//
	/** If enabled, FSM Components will always Tick, regardless of Component being Active or not. */
	UPROPERTY(Category = "General Settings", config, EditAnywhere, BlueprintReadOnly)
	bool AlwaysTickFSM;
	//
	/** If enabled, FSM Components will quit firing warnings when using [Client] or [Server] RPCs about Local Authority. */
	UPROPERTY(Category = "General Settings", config, EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Suppress RPC Warnings"))
	bool SuppressRPCWarnings;
	//
	//
	/** New FSM Components will always Tick at this set Interval by default. */
	UPROPERTY(Category = "General Settings", config, EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "100"))
	float DefaultTickInterval;
	//
	/** New FSM Components will always be automatically set to generate these initial States. */
	UPROPERTY(Category = "General Settings", config, EditAnywhere, BlueprintReadOnly)
	TArray<FName> DefaultStates;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////