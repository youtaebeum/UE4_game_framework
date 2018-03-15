/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IFSM.h"

#include "Net/UnrealNetwork.h"
#include "UFSMStateMachineComponent.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UStateMachineComponent;

class UFSMSettings;
class UStateMachineABP;
class UBTTask_FSM_AddState;
class UBTTask_FSM_SetState;
class UBTTask_FSM_CheckState;
class UBTTask_FSM_RemoveState;

#if WITH_EDITORONLY_DATA
	class UFSMGraph;
	class UFSMGraphNode;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType, meta = (DisplayName = "Transition", Keywords = "FSM Make Transition"))
enum class FSM_Transition : uint8 {
	Succeeded,
	Unchecked,
	Aborted
};

UENUM(meta=(Bitflags))
enum class FSM_Rule {
	/// On Link Transition, Creates Next State if it doesn't exist.
	CreateNext			UMETA(DisplayName = "Create Next State"),
	/// On Link Transition, Returns to this State if Next can't be reach.
	ReturnAborted		UMETA(DisplayName = "Return if Aborted"),
	/// On Link Transition, Destroys this past State once Next State activated successfully.
	RemovesState		UMETA(DisplayName = "Remove Current State")
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSM_ExitState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSM_BeginState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSM_UpdateState);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType, meta = (HideDropdown, RestrictedToClasses=UStateMachineComponent))
struct FSM_BeginEvent {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY(Category = "FSM", BlueprintReadWrite) uint8 StateID;
	UPROPERTY(Category = "FSM", BlueprintReadWrite) uint8 PreviousStateID;
	UPROPERTY(Category = "FSM", BlueprintReadWrite) FName PreviousStateName;
};

USTRUCT(BlueprintType, meta = (HideDropdown, RestrictedToClasses=UStateMachineComponent))
struct FSM_UpdateEvent {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY(Category = "FSM", BlueprintReadWrite) uint8 StateID;
	UPROPERTY(Category = "FSM", BlueprintReadWrite) uint8 StateTime;
};

USTRUCT(BlueprintType, meta = (HideDropdown, RestrictedToClasses=UStateMachineComponent))
struct FSM_ExitEvent {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY(Category = "FSM", BlueprintReadWrite) uint8 StateID;
	UPROPERTY(Category = "FSM", BlueprintReadWrite) uint8 NextStateID;
	UPROPERTY(Category = "FSM", BlueprintReadWrite) FName NextStateName;
};

/// FSM: Finite State Type
USTRUCT(BlueprintType)
struct UFSM_API FSM_State {
	GENERATED_USTRUCT_BODY()
	//
	/// This State's Name.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	FName Name;
	//
	//
	/// This State's Owner FSM.
	UPROPERTY(Category = "FSM", BlueprintReadOnly)
	UStateMachineComponent* Owner;
	//
	/// This State's Entry Event.
	UPROPERTY(Category = "FSM", VisibleDefaultsOnly, BlueprintReadOnly)
	FSM_BeginState OnBeginState;
	//
	/// This State's Update Event.
	UPROPERTY(Category = "FSM", VisibleDefaultsOnly, BlueprintReadOnly)
	FSM_UpdateState OnUpdateState;
	//
	/// This State's Exit Event.
	UPROPERTY(Category = "FSM", VisibleDefaultsOnly, BlueprintReadOnly)
	FSM_ExitState OnExitState;
	//
	//
	/// Constructor.
	FSM_State() {
		Name = NAME_None;
	}
	//
	/// Constructor.
	FSM_State(const TCHAR* Input) {
		Name = FName(Input);
	}
	//
	/// Constructor.
	FSM_State(const FString Input) {
		Name = FName(*Input);
	}
	//
	/// Constructor.
	FSM_State(const FName Input) {
		Name = Input;
	}
};

/// FSM: State Link Type
USTRUCT(BlueprintType)
struct UFSM_API FSM_Link {
	GENERATED_USTRUCT_BODY()
	//
	/// Once Finished, this State should jump to Next State by default, calling Named Function bellow.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, BlueprintReadOnly, SaveGame)
	FName NextState;
	//
	/// Name a Blueprint Function to call back when transitioning from Current State to Next.
	/// To execute this Default Transition, call 'Finish State' Function from anywhere in Blueprint Graph while this State is active.
	/// NOTE: YOU DON'T HAVE TO USE THIS if it confuses you! It's just a capability frequently requested by advanced Unreal Developers.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, BlueprintReadOnly, SaveGame)
	FName Function;
	//
	/// Define behaviour in which the State Machine should act in order to perform the 'Finish State' Function call.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, BlueprintReadOnly, SaveGame, meta = (Bitmask, BitmaskEnum=FSM_Rule))
	uint8 Rule;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSM_BPD_Initialize,const UStateMachineComponent*,Context);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSM_BPD_UpdateState,uint8,StateID,FName,StateName,float,StateTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSM_BPD_ExitState,uint8,StateID,FName,StateName,FSM_Transition,Transition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSM_BPD_BeginState,uint8,StateID,uint8,PreviousStateID,FName,StateName,FSM_Transition,Transition);

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSM_BP_UpdateState,uint8,StateID,FName,StateName,float,StateTime);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSM_BP_ExitState,uint8,StateID,FName,StateName,FSM_Transition,Transition);
DECLARE_DYNAMIC_DELEGATE_FourParams(FSM_BP_BeginState,uint8,StateID,uint8,PreviousStateID,FName,StateName,FSM_Transition,Transition);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// FSM: Blueprintable Finite State Machine Component
UCLASS(ClassGroup = Synaptech, Category = "Synaptech", hideCategories = ("Collision", "Component Tick", "Sockets", "Variable"), meta = (BlueprintSpawnableComponent, DisplayName = "Finite State Machine", ShortTooltip = "FSM: Blueprintable Finite State Machine System."))
class UFSM_API UStateMachineComponent : public UActorComponent, public IFSMInterface {
	GENERATED_UCLASS_BODY()
	/// Friend Classes.
	friend class UStateMachineABP;
	friend class UBTTask_FSM_SetState;
	friend class UBTTask_FSM_AddState;
	friend class UBTTask_FSM_CheckState;
	friend class UBTTask_FSM_RemoveState;
#if WITH_EDITORONLY_DATA
	friend class UFSMGraph;
	friend class UFSMGraphNode;
#endif
private:
	/// For 'UProperty Reflection' Voodoo only.
	UPROPERTY() FSM_ExitEvent FSM_ExitArguments;
	UPROPERTY() FSM_BeginEvent FSM_BeginArguments;
	UPROPERTY() FSM_UpdateEvent FSM_UpdateArguments;
	UFUNCTION(meta=(BlueprintInternalUseOnly="true")) FSM_ExitEvent FSM_ExitArgs(const FSM_ExitEvent StateInfo){return StateInfo;}
	UFUNCTION(meta=(BlueprintInternalUseOnly="true")) FSM_BeginEvent FSM_BeginArgs(const FSM_BeginEvent StateInfo){return StateInfo;}
	UFUNCTION(meta=(BlueprintInternalUseOnly="true")) FSM_UpdateEvent FSM_UpdateArgs(const FSM_UpdateEvent StateInfo){return StateInfo;}
	//
	/// Begins Default State by ID.
	void SetStartupState(const uint8 ID);
protected:
	/// Component Initialization after Begin Play.
	UPROPERTY()
	bool IsFullyInitialized;
	/// Current FSM State's ID.
	UPROPERTY(ReplicatedUsing=OnREP_StateID, SaveGame)
	uint8 StateID;
	/// Previous FSM State's ID.
	UPROPERTY(ReplicatedUsing=OnREP_PreviousStateID, SaveGame)
	uint8 PreviousStateID;
	/// Current FSM State's Elapsed Time in Seconds.
	UPROPERTY(ReplicatedUsing=OnREP_StateTime, SaveGame)
	float StateTime;
	/// Returns this Default Struct if STATE member not found.
	UPROPERTY()
	FSM_State STATE_Null;
	/// Map of all Native States holden by this FSM.
	UPROPERTY(SaveGame)
	TMap<uint8,FSM_State> STATE;
	//
	/// FSM Blueprint Implementable Event: Begin State (Dynamic Delegate).
	UPROPERTY()
	FSM_BP_BeginState InBegin;
	//
	/// FSM Blueprint Implementable Event: Update State (Dynamic Delegate).
	UPROPERTY(Category = "FSM", BlueprintReadWrite, meta = (DisplayName = "FSM: Update Callback"))
	FSM_BP_UpdateState InUpdate;
	//
	/// FSM Blueprint Implementable Event: Exit State (Dynamic Delegate).
	UPROPERTY()
	FSM_BP_ExitState InExit;

	UPROPERTY()
	bool m_is_initialized_fsm;

	//
	//
	/// Updates FSM's Current State every Tick.
	virtual void TickFSM(float DeltaTime);
	/// Rebuild Links, Default State Transitions.
	virtual void LinkStates();
	/// Clears Dynamic Events and all Items from 'STATE' Container.
	/// @Unlink: State Links are also erased if true.
	virtual void Clear(bool Unlink);
	//
	//
	/// Broadcasts FSM's Current State Entry Event.
	UFUNCTION()
	virtual void Begin(const uint8 InState, const FSM_BP_BeginState &Callback);
	/// Broadcasts FSM's Current State Update Event.
	UFUNCTION()
	virtual void Update(const uint8 UpState, const FSM_BP_UpdateState &Callback);
	/// Broadcasts FSM's Current State Exit Event.
	UFUNCTION()
	virtual bool Exit(const uint8 ExitState, const uint8 NextState, FSM_Transition &Transition, const FSM_BP_ExitState &Callback);
public:
	/// {4.13} New Performance Stuff.
	virtual UWorld* GetTickableGameObjectWorld() const {return GetWorld();}
	//
	/// Called when Component is activated.
	virtual void Activate(bool bReset) override;
	//
	/// Called when Component is activated.
	virtual void SetActive(bool bNewActive, bool bReset) override;
	//
	/// Called when Component is activated.
	virtual void ToggleActive() override;
	//
	/// Called when Component is deactivated.
	virtual void Deactivate() override;
	//
	/// Called after Component is built.
	virtual void PostInitProperties() override;
	//
	/// Called when Game starts.
	virtual void BeginPlay() override;
	//
	/// Enables Tick Every Frame.
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	//
	/// Enables Conditional Replication.
	virtual void PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker) override;
	//
	/// FSM Component Initialization.
	virtual void InitializeFSM();
	//
	//
	/// Exits Previous State and Begins New State ID.
	void SetState(const uint8 ID);
	//
	/// Exits Previous State and Begins New State by Name.
	void SetState(const FName Name);
	//
	/// Adds a New State to this FSM.
	void AddState(const uint8 ID, const FName Name);
	//
	/// Adds a New Link Between Two States.
	void AddTransition(const FName State, const FName NextState);
	//
	/// Destroy a FSM's State by ID.
	void RemoveState(const uint8 ID);
	//
	/// Destroy a FSM's State by ID & Name.
	void RemoveState(const uint8 ID, const FName Name);
	//
	/// Finds the ID of a State by Name.
	int8 FindStateID(const FName Name) const;
	//
	/// Finds the ID of a State by Name from the STATES Array.
	int8 FindStateInList(const FName Name) const;
	//
	/// Returns ID of last State in FSM list.
	uint8 GetLastStateID() const;
	//
	/// Returns Currently Active State's Name.
	FName GetStateName() const;
	//
	/// Gets the State Struct by ID.
	FSM_State* GetState(const uint8 ID);
	//
	/// Gets the State Struct by ID.
	const FSM_State* GetStatePtr(const uint8 ID) const;
	//
	/// Gets the State Struct by Name.
	const FSM_State &GetStateRef(const FName Name) const;
	//
	//
	/// FSM Base Properties Replication Mode.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, AdvancedDisplay)
	TEnumAsByte<ELifetimeCondition> ReplicationMode;
	//
	/// Enables Network Replication for State ID.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, AdvancedDisplay)
	bool ReplicateStateID;
	//
	/// Enables Network Replication for Previous State ID.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, AdvancedDisplay)
	bool ReplicatePreviousStateID;
	//
	/// Enables Network Replication for Current State Time.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, AdvancedDisplay)
	bool ReplicateStateTime;
	//
	/// Enables Debug Logs.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, AdvancedDisplay)
	bool Debug;
	//
	/// Set a Blueprint Enum here to import States Automatically.
	UPROPERTY(Category = "FSM", EditDefaultsOnly)
	UUserDefinedEnum* Enumerator;
	//
	/// Default List of States this FSM holds when instantiated.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "FSM States"))
	TArray<FName> STATES;
	//
	/// Default Startup State this FSM is set to when begin play.
	/// 'Exit' events will NOT execute prior to entering an initial State.
	/// This will be overridden by AnimBP's Default State if syncing States from Animation State Machine.
	/// If none or not found, default initial State falls back to State of index zero.
	UPROPERTY(Category = "FSM", EditDefaultsOnly, meta = (DisplayName = "Startup State"))
	FName StartupState;
	//
	/// List of Default State Transitions this FSM holds when instantiated.
	/// Cannot expand this list manually! To edit this chain, first provide a 'Blueprint Enum' to the 'Enumerator' field.
	/// To execute a Default Transition, simply call 'Finish State' Function from anywhere in Blueprint Graph while given State is active.
	/// 'Finish State' Function will execute this transition automatically for you and call the 'Named Function' provided if any.
	/// Consider 'Named Function' a callback where you can check, validate conditions or revert this State Transition.
	/// Note: You don't NEED to use this if you feel confused about how this works! You can still Set State by Name or ID as usual.
	UPROPERTY(Category = "FSM|Transition", EditFixedSize, EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "FSM Default Links"))
	TMap<FName,FSM_Link> LINKS;
	//
	//
	/// FSM: Initialization Interface.
	UFUNCTION(Category = "FSM", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "FSM: On Initialized", Keywords = "FSM Initialization Interface"))
	void OnInitializedFSM(const UStateMachineComponent* Context);
	//
	/// FSM: On Any 'On Begin' Event.
	UFUNCTION(Category = "FSM", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "FSM: On Any Begin", Keywords = "FSM Begin Event"))
	void OnAnyBeginState(const UStateMachineComponent* Context, const FName WithStateName, const uint8 WithStateID, const uint8 WithPrevious);
	//
	/// FSM: On Any 'On Update' Event.
	UFUNCTION(Category = "FSM", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "FSM: On Any Update", Keywords = "FSM Update Event"))
	void OnAnyUpdateState(const UStateMachineComponent* Context, const FName WithStateName, const uint8 WithStateID, const float WithStateTime);
	//
	/// FSM: On Any 'On Exit' Event.
	UFUNCTION(Category = "FSM", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "FSM: On Any Exit", Keywords = "FSM Exit Event"))
	void OnAnyExitState(const UStateMachineComponent* Context, const FName WithStateName, const uint8 WithStateID);
	//
	//
	/// Enables this FSM Component, making sure ALL other FSM Components attached are set to disabled.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Activate", Keywords = "FSM Activate This", ExpandEnumAsExecs = "Transition"))
	void ActivateFSM(FSM_Transition &Transition);
	//
	/// Disables this FSM Component, picks another FSM and set it enabled to be the 'Currently Active FSM'.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Go To Machine", Keywords = "FSM GoTo", ExpandEnumAsExecs = "Transition"))
	void GoToFSM(UStateMachineComponent* TargetMachine, FSM_Transition &Transition);
	//
	/// Completely overwrites all the list of States within this Machine.
	/// Using Array of Names as Template Data to generate new States. 
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Make States", Keywords = "FSM Make States", ExpandEnumAsExecs = "Transition"))
	void MakeStates(const TArray<FName> &NewStates, FSM_Transition &Transition);
	//
	/// Completely overwrites all the list of States within this Machine.
	/// Using Blueprint Enumerator of Names as Template Data to generate new States.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Make States", Keywords = "FSM Make States from Enumerator", ExpandEnumAsExecs = "Transition"))
	void MakeStatesEnumerated(const UUserDefinedEnum* Enumeration, FSM_Transition &Transition);
	//
	/// Adds a New State to this FSM.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Add State", Keywords = "FSM Add State", ExpandEnumAsExecs = "Transition"))
	void AddState(const uint8 ID, const FName Name, FSM_Transition &Transition);
	//
	//
	/// Exits Previous State and Begins New State by Name.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State", Keywords = "FSM Set State", ExpandEnumAsExecs = "Transition"))
	void SetState(const FName Name, FSM_Transition &Transition);
	//
	/// Exits Previous State and Begins New State by ID.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State ID", Keywords = "FSM Set State ID", ExpandEnumAsExecs = "Transition"))
	void SetStateID(const uint8 ID, FSM_Transition &Transition);
	//
	/// Exits Previous State and Begins New State by Value.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State Value", Keywords = "FSM Set State Value", ExpandEnumAsExecs = "Transition"))
	void SetStateValue(const FSM_State &Value, FSM_Transition &Transition);
	//
	/// Exits Previous State and Begins New State by Name; Cancels transition if condition is not met.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State (Conditional)", Keywords = "FSM Set State", ExpandEnumAsExecs = "Transition"))
	void SetState_Conditional(const FName Name, FSM_Transition Condition, FSM_Transition &Transition);
	//
	/// Exits Previous State and Begins New State by ID; Cancels transition if condition is not met.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State ID (Conditional)", Keywords = "FSM Set State ID", ExpandEnumAsExecs = "Transition"))
	void SetStateID_Conditional(const uint8 ID, FSM_Transition Condition, FSM_Transition &Transition);
	//
	/// Exits Previous State and Begins New State by Value; Cancels transition if condition is not met.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State Value (Conditional)", Keywords = "FSM Set State Value", ExpandEnumAsExecs = "Transition"))
	void SetStateValue_Conditional(const FSM_State &Value, FSM_Transition Condition, FSM_Transition &Transition);

	/// Exits Previous State and Begins New State by Name (+Callbacks).
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State [+Callbacks]", Keywords = "FSM Set State", ExpandEnumAsExecs = "Transition"))
	void SetStateWithCallbacks(const FName Name, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
	//
	/// Exits Previous State and Begins New State by ID (+Callbacks).
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State ID [+Callbacks]", Keywords = "FSM Set State ID", ExpandEnumAsExecs = "Transition"))
	void SetStateIDWithCallbacks(const uint8 ID, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
	//
	/// Exits Previous State and Begins New State by Value (+Callbacks).
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State Value [+Callbacks]", Keywords = "FSM Set State Value", ExpandEnumAsExecs = "Transition"))
	void SetStateValueWithCallbacks(const FSM_State &Value, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
	//
	/// Exits Previous State and Begins New State by Name; Cancels transition if condition is not met (+Callbacks).
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State (Conditional) [+Callbacks]", Keywords = "FSM Set State", ExpandEnumAsExecs = "Transition"))
	void SetState_ConditionalWithCallbacks(const FName Name, FSM_Transition Condition, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
	//
	/// Exits Previous State and Begins New State by ID; Cancels transition if condition is not met (+Callbacks).
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State ID (Conditional) [+Callbacks]", Keywords = "FSM Set State ID", ExpandEnumAsExecs = "Transition"))
	void SetStateID_ConditionalWithCallbacks(const uint8 ID, FSM_Transition Condition, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
	//
	/// Exits Previous State and Begins New State by Value; Cancels transition if condition is not met (+Callbacks).
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Set State Value (Conditional) [+Callbacks]", Keywords = "FSM Set State Value", ExpandEnumAsExecs = "Transition"))
	void SetStateValue_ConditionalWithCallbacks(const FSM_State &Value, FSM_Transition Condition, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback);
	//
	//
	/// Destroy a State from this FSM.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Remove State", Keywords = "FSM Remove State", ExpandEnumAsExecs = "Transition"))
	void RemoveState(const FName Name, FSM_Transition &Transition);
	//
	/// Exits Current State, picks Transition Rules from the 'LINKS' list and switch to Next Default State.
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Finish State", Keywords = "FSM Finish State", ExpandEnumAsExecs = "Transition"))
	void FinishState(FSM_Transition &Transition);
	//
	/// Exits Current State, picks Transition Rules from the 'LINKS' list and switch to Next Default State (+Callback).
	UFUNCTION(Category = "FSM", BlueprintCallable, meta = (DisplayName = "FSM: Finish State", Keywords = "FSM Finish State", ExpandEnumAsExecs = "Transition"))
	void FinishStateWithCallback(FSM_Transition &Transition, const FSM_BP_ExitState &Callback);
	//
	//
	/// [MULTIPLAYER] Disables this FSM Component, picks another FSM and set it enabled to be the 'Currently Active FSM'.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Go To Machine", Keywords = "FSM GoTo"))
	void SERVER_GoToFSM(UStateMachineComponent* TargetMachine, const bool Validate = true);
	//
	/// [MULTIPLAYER] Enables this FSM Component, making sure SERVER other FSM Components attached are set to disabled.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Activate", Keywords = "FSM Activate This"))
	void SERVER_ActivateFSM(const bool Validate = true);
	//
	/// [MULTIPLAYER]
	/// Completely overwrites all the list of States within this Machine.
	/// Using Array of Names as Template Data to generate new States. 
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Make States", Keywords = "FSM Make States"))
	void SERVER_MakeStates(const TArray<FName> &NewStates, const bool Validate = true);
	//
	/// [MULTIPLAYER] 
	/// Completely overwrites all the list of States within this Machine.
	/// Using Blueprint Enumerator of Names as Template Data to generate new States.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Make States from Enumerator", Keywords = "FSM Make States"))
	void SERVER_MakeStatesEnumerated(const UUserDefinedEnum* Enumeration, const bool Validate = true);
	//
	/// [MULTIPLAYER] Adds a New State to this FSM.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Add State", Keywords = "FSM Add State"))
	void SERVER_AddState(const uint8 ID, const FName Name, const bool Validate = true);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by Name.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Set State", Keywords = "FSM Set State"))
	void SERVER_SetState(const FName Name, const bool Validate = true);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by ID.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Set State ID", Keywords = "FSM Set State"))
	void SERVER_SetStateID(const uint8 ID, const bool Validate = true);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by Value.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Set State Value", Keywords = "FSM Set State"))
	void SERVER_SetStateValue(const FSM_State &Value, const bool Validate = true);
	//
	/// [MULTIPLAYER] Destroy a State from this FSM.
	UFUNCTION(Category = "FSM", Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Remove State", Keywords = "FSM Remove State"))
	void SERVER_RemoveState(const FName Name, const bool Validate = true);
	//
	/// [MULTIPLAYER] Exits Current State, picks Transition Rules from the 'LINKS' list and switch to Next Default State.
	UFUNCTION(Category = "FSM", BlueprintCallable, Server, Reliable, WithValidation, meta = (DisplayName = "[SERVER] FSM: Finish State", Keywords = "FSM Finish State"))
	void SERVER_FinishState(const bool Validate = true);
	//
	//
	/// [MULTIPLAYER] Disables this FSM Component, picks another FSM and set it enabled to be the 'Currently Active FSM'.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Go To Machine", Keywords = "FSM GoTo"))
	void CLIENT_GoToFSM(UStateMachineComponent* TargetMachine);
	//
	/// [MULTIPLAYER] Enables this FSM Component, making sure ALL other FSM Components attached are set to disabled.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Activate", Keywords = "FSM Activate This"))
	void CLIENT_ActivateFSM();
	//
	/// [MULTIPLAYER]
	/// Completely overwrites all the list of States within this Machine.
	/// Using Array of Names as Template Data to generate new States. 
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Make States", Keywords = "FSM Make States"))
	void CLIENT_MakeStates(const TArray<FName> &NewStates);
	//
	/// [MULTIPLAYER] 
	/// Completely overwrites all the list of States within this Machine.
	/// Using Blueprint Enumerator of Names as Template Data to generate new States.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Make States from Enumerator", Keywords = "FSM Make States"))
	void CLIENT_MakeStatesEnumerated(const UUserDefinedEnum* Enumeration);
	//
	/// [MULTIPLAYER] Adds a New State to this FSM.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Add State", Keywords = "FSM Add State"))
	void CLIENT_AddState(const uint8 ID, const FName Name);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by Name.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Set State", Keywords = "FSM Set State"))
	void CLIENT_SetState(const FName Name);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by ID.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Set State ID", Keywords = "FSM Set State"))
	void CLIENT_SetStateID(const uint8 ID);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by Value.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Set State Value", Keywords = "FSM Set State"))
	void CLIENT_SetStateValue(const FSM_State &Value);
	//
	/// [MULTIPLAYER] Destroy a State from this FSM.
	UFUNCTION(Category = "FSM", Client, Reliable, BlueprintCallable, meta = (DisplayName = "[CLIENT] FSM: Remove State", Keywords = "FSM Remove State"))
	void CLIENT_RemoveState(const FName Name);
	//
	/// [MULTIPLAYER] Exits Current State, picks Transition Rules from the 'LINKS' list and switch to Next Default State.
	UFUNCTION(Category = "FSM", BlueprintCallable, Client, Reliable, meta = (DisplayName = "[CLIENT] FSM: Finish State", Keywords = "FSM Finish State"))
	void CLIENT_FinishState();
	//
	//
	/// [MULTIPLAYER] Disables this FSM Component, picks another FSM and set it enabled to be the 'Currently Active FSM'.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Go To Machine", Keywords = "FSM GoTo"))
	void MULTI_GoToFSM(UStateMachineComponent* TargetMachine);
	//
	/// [MULTIPLAYER] Enables this FSM Component, making sure ALL other FSM Components attached are set to disabled.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Activate", Keywords = "FSM Activate This"))
	void MULTI_ActivateFSM();
	//
	/// [MULTIPLAYER]
	/// Completely overwrites all the list of States within this Machine.
	/// Using Array of Names as Template Data to generate new States. 
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Make States", Keywords = "FSM Make States"))
	void MULTI_MakeStates(const TArray<FName> &NewStates);
	//
	/// [MULTIPLAYER] 
	/// Completely overwrites all the list of States within this Machine.
	/// Using Blueprint Enumerator of Names as Template Data to generate new States.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Make States from Enumerator", Keywords = "FSM Make States"))
	void MULTI_MakeStatesEnumerated(const UUserDefinedEnum* Enumeration);
	//
	/// [MULTIPLAYER] Adds a New State to this FSM.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Add State", Keywords = "FSM Add State"))
	void MULTI_AddState(const uint8 ID, const FName Name);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by Name.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Set State", Keywords = "FSM Set State"))
	void MULTI_SetState(const FName Name);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by ID.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Set State ID", Keywords = "FSM Set State"))
	void MULTI_SetStateID(const uint8 ID);
	//
	/// [MULTIPLAYER] Exits Previous State and Begins New State by Value.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Set State Value", Keywords = "FSM Set State"))
	void MULTI_SetStateValue(const FSM_State &Value);
	//
	/// [MULTIPLAYER] Destroy a State from this FSM.
	UFUNCTION(Category = "FSM", NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Remove State", Keywords = "FSM Remove State"))
	void MULTI_RemoveState(const FName Name);
	//
	/// [MULTIPLAYER] Exits Current State, picks Transition Rules from the 'LINKS' list and switch to Next Default State.
	UFUNCTION(Category = "FSM", BlueprintCallable, NetMulticast, Reliable, meta = (DisplayName = "[ALL] FSM: Finish State", Keywords = "FSM Finish State"))
	void MULTI_FinishState();
	//
	//
	/// Returns Current State's Loop Time in Seconds.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Get Time", Keywords = "FSM Get Time"))
	float GetTime() {return StateTime;}
	//
	/// Returns Currently Active State's Name.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Get Current State Name", Keywords = "FSM Get State Name"))
	FName GetCurrentStateName() {return GetActiveState().Name;}
	//
	/// Returns Currently Active State's ID.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Get Current State ID", Keywords = "FSM Get State ID"))
	uint8 GetCurrentStateID() {return StateID;}
	//
	/// Returns Previously Active State's ID.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Get Previous State ID", Keywords = "FSM Get Last State ID"))
	uint8 GetPreviousStateID() {return PreviousStateID;}
	//
	/// Returns Previously Active State's Name.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Get Previous State Name", Keywords = "FSM Get Last State Name"))
	FName GetPreviousStateName() {return (GetStatePtr(PreviousStateID)) ? GetStatePtr(PreviousStateID)->Name : STATE_Null.Name;}
	//
	/// Gets the Active State Struct.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Get Active State", Keywords = "FSM Get State"))
	FSM_State &GetActiveState();
	//
	/// Returns Component Initialization State.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Is Component Fully Initialized ?", Keywords = "FSM Is Initialized"))
	bool IsInitializedFSM() {return IsFullyInitialized;}
	//
	//
	/// Adds a New State to this FSM.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Add State", Keywords = "FSM Add State"))
	FSM_Transition Pure_AddState(const uint8 ID, const FName Name);
	//
	/// Exits Previous State and Begins New State by Name.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Set State", Keywords = "FSM Set State"))
	FSM_Transition Pure_SetState(const FName Name);
	//
	/// Exits Previous State and Begins New State by ID.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Set State ID", Keywords = "FSM Set State"))
	FSM_Transition Pure_SetStateByID(const uint8 ID);
	//
	/// Destroy a State from this FSM.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Remove State", Keywords = "FSM Remove State"))
	FSM_Transition Pure_RemoveState(const FName Name);
	//
	/// Exits Current State, picks Transition Rules from the 'LINKS' list and switch to Next Default State.
	UFUNCTION(Category = "FSM", BlueprintPure, meta = (DisplayName = "FSM: Finish State", Keywords = "FSM Finish State"))
	FSM_Transition Pure_FinishState();
	//
	//
	/// FSM Blueprint Self-Implementable Event: On Construct This.
	UFUNCTION(Category = "FSM", BlueprintImplementableEvent, meta = (DisplayName = "FSM: On Construct", Keywords = "FSM On Construct"))
	void Event_OnConstruct();
	//
	/// FSM Blueprint Self-Implementable Event: On Begin State.
	UFUNCTION(Category = "FSM", BlueprintImplementableEvent, meta = (DisplayName = "FSM: On Begin State", Keywords = "FSM On Begin Event State"))
	void Event_OnBeginState(uint8 CurrentState, uint8 PreviousState, FName StateName, FSM_Transition Transition);
	//
	/// FSM Blueprint Self-Implementable Event: On Update State.
	UFUNCTION(Category = "FSM", BlueprintImplementableEvent, meta = (DisplayName = "FSM: On Update State", Keywords = "FSM On Update Event State"))
	void Event_OnUpdateState(uint8 CurrentState, FName StateName, float StateTimer);
	//
	/// FSM Blueprint Self-Implementable Event: On Exit State.
	UFUNCTION(Category = "FSM", BlueprintImplementableEvent, meta = (DisplayName = "FSM: On Exit State", Keywords = "FSM On Exit Event State"))
	void Event_OnExitState(uint8 CurrentState, FName StateName, FSM_Transition Transition);
	//
	//
	UFUNCTION(Category = "FSM", BlueprintNativeEvent, meta = (DisplayName = "FSM: On Replicate State ID", Keywords = "FSM On Replicate State ID"))
	void OnREP_StateID(uint8 &ID);
	//
	UFUNCTION(Category = "FSM", BlueprintNativeEvent, meta = (DisplayName = "FSM: On Replicate Previous State ID", Keywords = "FSM On Replicate State ID"))
	void OnREP_PreviousStateID(uint8 &PreviousID);
	//
	UFUNCTION(Category = "FSM", BlueprintNativeEvent, meta = (DisplayName = "FSM: On Replicate State Time", Keywords = "FSM On Replicate State Time"))
	void OnREP_StateTime(float &Time);
	//
	//
	/// FSM Blueprint Implementable Event: On Component Fully Initialized.
	UPROPERTY(Category = "FSM", BlueprintAssignable, meta = (DisplayName = "FSM: On Initialized"))
	FSM_BPD_Initialize OnInitialized;
	//
	/// FSM Blueprint Implementable Event: On Begin State (Multicast Dynamic Delegate).
	UPROPERTY(Category = "FSM", BlueprintAssignable, meta = (DisplayName = "FSM: On Begin State"))
	FSM_BPD_BeginState OnBegin;
	//
	/// FSM Blueprint Implementable Event: On Update State (Multicast Dynamic Delegate).
	UPROPERTY(Category = "FSM", BlueprintAssignable, meta = (DisplayName = "FSM: On Update State"))
	FSM_BPD_UpdateState OnUpdate;
	//
	/// FSM Blueprint Implementable Event: On Exit State (Multicast Dynamic Delegate).
	UPROPERTY(Category = "FSM", BlueprintAssignable, meta = (DisplayName = "FSM: On Exit State"))
	FSM_BPD_ExitState OnExit;
	//
	//
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent &PECP) override;
#endif
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FORCEINLINE bool operator == (FSM_State &FSA, FSM_State &FSB) {
	return ((FSA.Name==FSB.Name)&&(FSA.Owner==FSB.Owner));
}

FORCEINLINE bool operator == (const FSM_State &FSA, FSM_State &FSB) {
	return ((FSA.Name==FSB.Name)&&(FSA.Owner==FSB.Owner));
}

FORCEINLINE bool operator == (FSM_State &FSA, const FSM_State &FSB) {
	return ((FSA.Name==FSB.Name)&&(FSA.Owner==FSB.Owner));
}

FORCEINLINE bool operator == (const FSM_State &FSA, const FSM_State &FSB) {
	return ((FSA.Name==FSB.Name)&&(FSA.Owner==FSB.Owner));
}

FORCEINLINE bool operator != (FSM_State &FSA, FSM_State &FSB) {
	return ((FSA.Name!=FSB.Name)&&(FSA.Owner!=FSB.Owner));
}

FORCEINLINE bool operator != (const FSM_State &FSA, FSM_State &FSB) {
	return ((FSA.Name!=FSB.Name)&&(FSA.Owner!=FSB.Owner));
}

FORCEINLINE bool operator != (FSM_State &FSA, const FSM_State &FSB) {
	return ((FSA.Name!=FSB.Name)&&(FSA.Owner!=FSB.Owner));
}

FORCEINLINE bool operator != (const FSM_State &FSA, const FSM_State &FSB) {
	return ((FSA.Name!=FSB.Name)&&(FSA.Owner!=FSB.Owner));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FORCEINLINE FArchive &operator << (FArchive &Ar, FSM_Transition &TR) {
	uint8 Bytes = (uint8)TR; Ar << Bytes;
	if (Ar.IsLoading()) {TR = (FSM_Transition)TR;} return Ar;
}

FORCEINLINE FArchive &operator << (FArchive &Ar, FSM_Rule &RL) {
	uint8 Bytes = (uint8)RL; Ar << Bytes;
	if (Ar.IsLoading()) {RL = (FSM_Rule)RL;} return Ar;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////