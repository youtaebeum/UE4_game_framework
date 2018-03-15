/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMStateMachineComponent.h"
#include "Private/UFSMShared.h"

#include "Engine/UserDefinedEnum.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UStateMachineComponent::UStateMachineComponent(const FObjectInitializer &OBJ) : Super(OBJ) {
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	//
	ReplicationMode = ELifetimeCondition::COND_OwnerOnly;
	//
	SetComponentTickEnabled(true);
	SetIsReplicated(true);
	SetNetAddressable();
	SetActive(true,false);
	//
	ReplicatePreviousStateID = true;
	IsFullyInitialized = false;
	ReplicateStateTime = true;
	ReplicateStateID = true;
	Debug = false;

	m_is_initialized_fsm = false;
	//
	STATE_Null.Name = FSM_NULL;
	//
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (Settings) {
		PrimaryComponentTick.TickInterval = Settings->DefaultTickInterval;
		STATES = Settings->DefaultStates;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// UStateMachineComponent

void UStateMachineComponent::PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker) {
	Super::PreReplication(ChangedPropertyTracker);
	//
	DOREPLIFETIME_ACTIVE_OVERRIDE(UStateMachineComponent,StateID,ReplicateStateID);
	DOREPLIFETIME_ACTIVE_OVERRIDE(UStateMachineComponent,PreviousStateID,ReplicatePreviousStateID);
	DOREPLIFETIME_ACTIVE_OVERRIDE(UStateMachineComponent,StateTime,ReplicateStateTime);
}

void UStateMachineComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//
	DOREPLIFETIME_CONDITION(UStateMachineComponent,StateID,ReplicationMode);
	DOREPLIFETIME_CONDITION(UStateMachineComponent,PreviousStateID,ReplicationMode);
	DOREPLIFETIME_CONDITION(UStateMachineComponent,StateTime,ReplicationMode);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::Activate(bool bReset) {
	Super::Activate(bReset);
	//
	if (!IsFullyInitialized) {return;}
	FSM_Transition Transition = FSM_Transition::Unchecked;
	OnBegin.Broadcast(StateID,PreviousStateID,STATE.FindChecked(StateID).Name,Transition);
}

void UStateMachineComponent::SetActive(bool bNewActive, bool bReset) {
	Super::SetActive(bNewActive,bReset);
	//
	if (!IsFullyInitialized) {return;}
	FSM_Transition Transition = FSM_Transition::Unchecked;
	//
	if (bNewActive) {
		OnBegin.Broadcast(StateID,PreviousStateID,STATE.FindChecked(StateID).Name,Transition);
	} else {
		OnExit.Broadcast(StateID,STATE.FindChecked(StateID).Name,Transition);
	}
}

void UStateMachineComponent::ToggleActive() {
	Super::ToggleActive();
	//
	if (!IsFullyInitialized) {return;}
	FSM_Transition Transition = FSM_Transition::Unchecked;
	//
	if (IsActive()) {
		OnBegin.Broadcast(StateID,PreviousStateID,STATE.FindChecked(StateID).Name,Transition);
	} else {
		OnExit.Broadcast(StateID,STATE.FindChecked(StateID).Name,Transition);
	}
}

void UStateMachineComponent::Deactivate() {
	Super::Deactivate();
	//
	FSM_Transition Transition = FSM_Transition::Unchecked;
	if (!IsFullyInitialized) {return;}
	OnExit.Broadcast(StateID,STATE.FindChecked(StateID).Name,Transition);
}

void UStateMachineComponent::PostInitProperties() {
	Super::PostInitProperties();
	//
	if (!GetOwner()) {return;}
	OnInitialized.Clear();
	Event_OnConstruct();
}

void UStateMachineComponent::BeginPlay() {
	Super::BeginPlay();

	if (m_is_initialized_fsm == false)
	{
		//
		FSM_Transition Transition;
		MakeStates(STATES, Transition);
		InitializeFSM();
	}
}

void UStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
	//
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->AlwaysTickFSM) {
		if (!IsActive()) {return;}
	} TickFSM(DeltaTime);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::TickFSM(float DeltaTime) {
	if (StateID!=INDEX_NONE) {
		StateTime += DeltaTime;
		Update(StateID,InUpdate);
	}///
	//
	if (Debug && GetOwner()) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET==NM_DedicatedServer)||(NET==NM_ListenServer)) {
			LOG_FSM(Debug,false,-1,FColor::Cyan,FString("[SERVER]: ")+GetOwner()->GetName()+FString(" :: ")+GetName()+FString(" :: ")+GetActiveState().Name.ToString()+FString(" : State Time : ")+FString::FromInt(StateTime));
		} else if (NET==NM_Client) {LOG_FSM(Debug,false,-1,FColor::Cyan,FString("[CLIENT]: ")+GetOwner()->GetName()+FString(" :: ")+GetName()+FString(" :: ")+GetActiveState().Name.ToString()+FString(" : State Time : ")+FString::FromInt(StateTime));}
	else {LOG_FSM(Debug,false,-1,FColor::Cyan,GetOwner()->GetName()+FString(" :: ")+GetName()+FString(" :: ")+GetActiveState().Name.ToString()+FString(" : State Time : ")+FString::FromInt(StateTime));}}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::InitializeFSM() {
	m_is_initialized_fsm = true;
	SetStartupState(FindStateID(StartupState));
	//
	if (!GetOwner()->IsValidLowLevelFast()||!GetOwner()->GetClass()->IsChildOf<ACharacter>()) {
		Execute_OnInitializedFSM(this,this);
	return;}
	//
	const auto &CHA = Cast<ACharacter>(GetOwner());
	if (!CHA) {Execute_OnInitializedFSM(this,this); return;}
	//
	USkeletalMeshComponent* SMC = CHA->GetMesh();
	if (!SMC) {Execute_OnInitializedFSM(this,this); return;}
	//
	UStateMachineABP* ABP = Cast<UStateMachineABP>(SMC->GetAnimInstance());
	if (!ABP) {Execute_OnInitializedFSM(this,this); return;}
	if (ABP->TargetFSM != this->GetFName()) {Execute_OnInitializedFSM(this,this); return;}
	//
	const auto &ASM = ABP->GetStateMachineInstanceFromName(ABP->TargetASM);
	if (!ASM) {Execute_OnInitializedFSM(this,this); return;}
	//
	if (ABP->OverrideFSM) {
		const auto &MD = ABP->GetMachineDescription(ABP->GetClassInterface(),ASM);
		if (MD) {
			FSM_Transition Transition;
			Clear(true); uint32 ID = 0;
			for (const auto &State : MD->States) {
				AddState(ID,State.StateName,Transition);
				LINKS.Empty(); FSM_Link Link;
				for (const auto &Transit : MD->Transitions) {
					if (Transit.PreviousState != ID) {continue;}
					if (!MD->States.IsValidIndex(Transit.NextState)) {continue;}
					Link.NextState = MD->States[Transit.NextState].StateName;
				break;}
				Link.Function = FName(*FString(FString("OnFinish")+State.StateName.ToString().Replace(TEXT(" "),TEXT(""))));
			LINKS.Add(State.StateName,Link); ID++;}
	SetStartupState(ASM->GetCurrentState());}}
	//
	Execute_OnInitializedFSM(this,this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::OnInitializedFSM_Implementation(const UStateMachineComponent* Context) {
	if (Context != this) {return;}
	//
	for (TObjectIterator<AActor> Actor; Actor; ++Actor) {
		const auto &Interface = Cast<IFSMInterface>(*Actor);
		if (Interface) {Interface->Execute_OnInitializedFSM(*Actor,Context);} else
		if (Actor->GetClass()->ImplementsInterface(UFSMInterface::StaticClass())) {
			IFSMInterface::Execute_OnInitializedFSM(*Actor,Context);
	}} IsFullyInitialized = true;
	//
	OnInitialized.Broadcast(Context);
}

void UStateMachineComponent::OnAnyBeginState_Implementation(const UStateMachineComponent* Context, const FName WithStateName, const uint8 WithStateID, const uint8 WithPrevious) {
	if (Context != this) {return;}
	//
	for (TObjectIterator<AActor> Actor; Actor; ++Actor) {
		const auto &Interface = Cast<IFSMInterface>(*Actor);
		if (Interface) {Interface->Execute_OnAnyBeginState(*Actor,Context,WithStateName,WithStateID,WithPrevious);} else
		if (Actor->GetClass()->ImplementsInterface(UFSMInterface::StaticClass())) {
			IFSMInterface::Execute_OnAnyBeginState(*Actor,Context,WithStateName,WithStateID,WithPrevious);
	}}//////
}

void UStateMachineComponent::OnAnyUpdateState_Implementation(const UStateMachineComponent* Context, const FName WithStateName, const uint8 WithStateID, const float WithStateTime) {
	if (Context != this) {return;}
	//
	for (TObjectIterator<AActor> Actor; Actor; ++Actor) {
		const auto &Interface = Cast<IFSMInterface>(*Actor);
		if (Interface) {Interface->Execute_OnAnyUpdateState(*Actor,Context,WithStateName,WithStateID,WithStateTime);} else
		if (Actor->GetClass()->ImplementsInterface(UFSMInterface::StaticClass())) {
			IFSMInterface::Execute_OnAnyUpdateState(*Actor,Context,WithStateName,WithStateID,WithStateTime);
	}}//////
}

void UStateMachineComponent::OnAnyExitState_Implementation(const UStateMachineComponent* Context, const FName WithStateName, const uint8 WithStateID) {
	if (Context != this) {return;}
	//
	for (TObjectIterator<AActor> Actor; Actor; ++Actor) {
		const auto &Interface = Cast<IFSMInterface>(*Actor);
		if (Interface) {Interface->Execute_OnAnyExitState(*Actor,Context,WithStateName,WithStateID);} else
		if (Actor->GetClass()->ImplementsInterface(UFSMInterface::StaticClass())) {
			IFSMInterface::Execute_OnAnyExitState(*Actor,Context,WithStateName,WithStateID);
	}}//////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::Begin(const uint8 InState, const FSM_BP_BeginState &Callback) {
	if (!IsActive()) {return;}
	if (!STATE.Contains(InState)) {return;}
	STATE.FindChecked(InState).OnBeginState.Broadcast();
	//
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (Settings->BlueprintAutoFlowFSM) {
		FString N_State_A = STATE[InState].Name.ToString();
		FString N_State_B = STATE[InState].Name.ToString().Replace(TEXT(" "),TEXT(""));
		FName NID_State_A = *(FString("On Begin ")+N_State_A);
		FName NID_State_B = *(FString("OnBegin")+N_State_B);
		//
		UFunction* Function_I = FindFunction(NID_State_A);
		UFunction* Function_II = FindFunction(NID_State_B);
		//
		FSM_BeginArguments.StateID = StateID;
		FSM_BeginArguments.PreviousStateID = PreviousStateID;
		FSM_BeginArguments.PreviousStateName = GetStatePtr(PreviousStateID)->Name;
		//
		if (Function_I && Function_I->NumParms==0) {ProcessEvent(Function_I,NULL);} else
		if (Function_I && Function_I->NumParms==1) {ProcessEvent(Function_I,&FSM_BeginArguments);}
		else if (Function_I) {LOG_PIE(true,ESeverity::Error,this,Function_I,TEXT("FSM Automatic Graph Flow execution currently doesn't support calling ''On Begin'' functions that contains dynamic number of parameters!"));}
		//
		if (Function_II && Function_II->NumParms==0) {ProcessEvent(Function_II,NULL);} else
		if (Function_II && Function_II->NumParms==1) {ProcessEvent(Function_II,&FSM_BeginArguments);}
		else if (Function_II) {LOG_PIE(true,ESeverity::Error,this,Function_II,TEXT("FSM Automatic Graph Flow execution currently doesn't support calling ''On Begin'' functions that contains dynamic number of parameters!"));}
		//
		if ((Function_I==nullptr)&&(Function_II==nullptr)) {LOG_PIE(Debug,ESeverity::Warning,this,FFIND(TEXT("Begin")),FString("''Blueprint Auto Flow FSM'' is enabled; Trying to invoke ''On Begin'' for (")+N_State_A+FString(") State, but (")+NID_State_B.ToString()+(") function have not been found!"));}
	} if (Settings->EnableInterfaceStatePropagation) {Execute_OnAnyBeginState(this,this,GetActiveState().Name,StateID,PreviousStateID);}
	//
	OnBegin.Broadcast(InState,PreviousStateID,STATE.FindChecked(InState).Name,FSM_Transition::Succeeded);
	Callback.ExecuteIfBound(InState,PreviousStateID,STATE.FindChecked(InState).Name,FSM_Transition::Succeeded);
	Event_OnBeginState(InState,PreviousStateID,STATE.FindChecked(InState).Name,FSM_Transition::Succeeded);
}

void UStateMachineComponent::Update(const uint8 UpState, const FSM_BP_UpdateState &Callback) {
	if (!IsActive()) {return;}
	if (!STATE.Contains(UpState)) {return;}
	STATE.FindChecked(UpState).OnUpdateState.Broadcast();
	//
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (Settings->BlueprintAutoFlowFSM) {
		FString N_State_A = STATE[UpState].Name.ToString();
		FString N_State_B = STATE[UpState].Name.ToString().Replace(TEXT(" "),TEXT(""));
		FName NID_State_A = *(FString("On Update ")+N_State_A);
		FName NID_State_B = *(FString("OnUpdate")+N_State_B);
		//
		UFunction* Function_I = FindFunction(NID_State_A);
		UFunction* Function_II = FindFunction(NID_State_B);
		//
		FSM_UpdateArguments.StateID = StateID;
		FSM_UpdateArguments.StateTime = StateTime;
		//
		if (Function_I && Function_I->NumParms==0) {ProcessEvent(Function_I,NULL);} else
		if (Function_I && Function_I->NumParms==1) {ProcessEvent(Function_I,&FSM_UpdateArguments);}
		else if (Function_I) {LOG_PIE(true,ESeverity::Error,this,Function_I,TEXT("FSM Automatic Graph Flow execution currently doesn't support calling ''On Update'' functions that contains dynamic number of parameters!"));}
		//
		if (Function_II && Function_II->NumParms==0) {ProcessEvent(Function_II,NULL);} else
		if (Function_II && Function_II->NumParms==1) {ProcessEvent(Function_II,&FSM_UpdateArguments);}
		else if (Function_II) {LOG_PIE(true,ESeverity::Error,this,Function_II,TEXT("FSM Automatic Graph Flow execution currently doesn't support calling ''On Update'' functions that contains dynamic number of parameters!"));}
	} if (Settings->EnableInterfaceStatePropagation) {Execute_OnAnyUpdateState(this,this,GetActiveState().Name,StateID,StateTime);}
	//
	OnUpdate.Broadcast(UpState,STATE.FindChecked(UpState).Name,StateTime);
	Callback.ExecuteIfBound(UpState,STATE.Find(UpState)->Name,StateTime);
	Event_OnUpdateState(UpState,STATE.Find(UpState)->Name,StateTime);
}

bool UStateMachineComponent::Exit(const uint8 ExitState, const uint8 NextState, FSM_Transition &Transition, const FSM_BP_ExitState &Callback) {
	if (!IsActive()) {return false;}
	if (!STATE.Contains(ExitState)) {return false;}
	if (Transition!=FSM_Transition::Succeeded) {return false;}
	//
	STATE.FindChecked(ExitState).OnExitState.Broadcast();
	//
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (Settings->BlueprintAutoFlowFSM) {
		FString N_State_A = STATE[ExitState].Name.ToString();
		FString N_State_B = STATE[ExitState].Name.ToString().Replace(TEXT(" "),TEXT(""));
		FName NID_State_A = *(FString("On Exit ")+N_State_A);
		FName NID_State_B = *(FString("OnExit")+N_State_B);
		//
		UFunction* Function_I = FindFunction(NID_State_A);
		UFunction* Function_II = FindFunction(NID_State_B);
		//
		FSM_ExitArguments.StateID = StateID;
		FSM_ExitArguments.NextStateID = NextState;
		FSM_ExitArguments.NextStateName = GetStatePtr(NextState)->Name;
		//
		if (Function_I && Function_I->NumParms==0) {ProcessEvent(Function_I,NULL);} else
		if (Function_I && Function_I->NumParms==1) {ProcessEvent(Function_I,&FSM_ExitArguments);}
		else if (Function_I) {LOG_PIE(true,ESeverity::Error,this,Function_I,TEXT("FSM Automatic Graph Flow execution currently doesn't support calling ''On Exit'' functions that contains dynamic number of parameters!"));}
		//
		if (Function_II && Function_II->NumParms==0) {ProcessEvent(Function_II,NULL);} else
		if (Function_II && Function_II->NumParms==1) {ProcessEvent(Function_II,&FSM_ExitArguments);}
		else if (Function_II) {LOG_PIE(true,ESeverity::Error,this,Function_II,TEXT("FSM Automatic Graph Flow execution currently doesn't support calling ''On Exit'' functions that contains dynamic number of parameters!"));}
		if ((Function_I==nullptr)&&(Function_II==nullptr)) {LOG_PIE(Debug,ESeverity::Warning,this,FFIND(TEXT("Exit")),FString("''Blueprint Auto Flow FSM'' is enabled; Trying to invoke ''On Exit'' for (")+N_State_A+FString(") State, but (")+NID_State_B.ToString()+(") function have not been found!"));}
	} if (Settings->EnableInterfaceStatePropagation) {Execute_OnAnyExitState(this,this,GetActiveState().Name,StateID);}
	//
	OnExit.Broadcast(ExitState,STATE.FindChecked(ExitState).Name,FSM_Transition::Succeeded);
	Callback.ExecuteIfBound(ExitState,STATE.Find(ExitState)->Name,FSM_Transition::Succeeded);
	Event_OnExitState(ExitState,STATE.Find(ExitState)->Name,FSM_Transition::Succeeded);
	//
	return true;
}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::SetStartupState(const uint8 ID) {
	if (!IsActive()) {return;}
	if (!STATE.Contains(ID)) {Begin(0,InBegin); return;}
	//
	StateID = ID;
	StateTime = 0.0f;
	PreviousStateID = StateID;
	//
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetState(const uint8 ID) {
	if (!STATE.Contains(ID)) {return;}
	if (!IsActive()) {return;}
	//
	FSM_Transition Transition = FSM_Transition::Succeeded;
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateTime = 0.0f;
	 StateID = ID;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetState(const FName Name) {
	if (!IsActive()) {return;}
	//
	const auto &ID = FindStateID(Name);
	if (ID == INDEX_NONE) {return;}
	//
	FSM_Transition Transition = FSM_Transition::Succeeded;
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::AddState(const uint8 ID, const FName Name) {
	if (STATE.Contains(ID)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("AddState")),FString("Can't add State, ID already exists!")); return;}
	if (ID>255) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("AddState")),FString("Warning: Max uint8 size exceeded!"));}
	//
	for (const auto &State : STATE) {
		if (State.Key != ID) {continue;}
		if (State.Value.Name.Compare(Name)==0) {
			LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("AddState")),FString("Can't add State, ID already exists."));
	return;}}
	//
	STATE.Add(ID,FSM_State(*&Name));
	STATE[ID].Owner = this;
	STATES.Add(Name);
	//
	LinkStates();
}

void UStateMachineComponent::AddTransition(const FName State, const FName NextState) {
	if (!STATE.Contains(FindStateID(State))) {
		LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("AddTransition")),FString("Can't add Transition from Inexistent State."));
	return;}
	//
	FSM_Link Link; Link.NextState = NextState;
	Link.Function = FName(*FString(FString("OnFinish")+State.ToString().Replace(TEXT(" "),TEXT(""))));
	if (LINKS.Contains(State)) {
		LINKS.Emplace(State,Link);
	} else {LINKS.Add(State,Link);}
}

void UStateMachineComponent::RemoveState(const uint8 ID) {
	if (STATE.Contains(ID)) {
		STATE[ID].OnBeginState.Clear();
		STATE[ID].OnUpdateState.Clear();
		STATE[ID].OnExitState.Clear();
	STATE.Remove(ID);}
}

void UStateMachineComponent::RemoveState(const uint8 ID, const FName Name) {
	if (STATE.Contains(ID) && STATE[ID].Name.Compare(Name)==0) {
		STATE[ID].OnBeginState.Clear();
		STATE[ID].OnUpdateState.Clear();
		STATE[ID].OnExitState.Clear();
	STATE.Remove(ID);}
}

int8 UStateMachineComponent::FindStateID(const FName Name) const {
	TArray<FSM_State> DUMP;
	STATE.GenerateValueArray(DUMP);
	//
	for (const auto &State : DUMP) {
		if (State.Name==Name) {return *STATE.FindKey(State);}
	} return INDEX_NONE;
}

int8 UStateMachineComponent::FindStateInList(const FName Name) const {
	int8 ID = 0;
	for (const auto &State : STATES) {
		if (ID>255) {return INDEX_NONE;}
		if (State==Name) {return ID;}
	ID++;} return INDEX_NONE;
}

uint8 UStateMachineComponent::GetLastStateID() const {
	if (STATE.Num()<=1) {return 0;}
	return STATE.Num()-1;
}

FName UStateMachineComponent::GetStateName() const {
	if (!STATE.Contains(StateID)) {return STATE_Null.Name;}
	return STATE.FindChecked(StateID).Name;
}

FSM_State* UStateMachineComponent::GetState(const uint8 ID) {
	if (!STATE.Contains(ID)) {return &STATE_Null;}
	return &STATE.FindChecked(ID);
}

const FSM_State* UStateMachineComponent::GetStatePtr(const uint8 ID) const {
	if (!STATE.Contains(ID)) {return &STATE_Null;}
	return &STATE.FindChecked(ID);
}

const FSM_State &UStateMachineComponent::GetStateRef(const FName Name) const {
	for (const auto &State : STATE) {
		if (State.Value.Name==Name) {return State.Value;}
	} return STATE_Null;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::GoToFSM(UStateMachineComponent* TargetMachine, FSM_Transition &Transition) {
	if (!GetOwner()||!TargetMachine) {Transition = FSM_Transition::Aborted; return;}
	//
	TargetMachine->SetActive(true,false);
	this->SetActive(false,false);
	//
	Transition = FSM_Transition::Succeeded;
}

void UStateMachineComponent::ActivateFSM(FSM_Transition &Transition) {
	if (!GetOwner()) {Transition = FSM_Transition::Aborted;}
	Transition = FSM_Transition::Unchecked;
	//
	for (const auto &C : GetOwner()->GetComponentsByClass(UStateMachineComponent::StaticClass())) {
		const auto &SM = Cast<UStateMachineComponent>(C); if (SM) {
			if (SM->GetName() != GetName()) {SM->SetActive(false,false); continue;}
		SM->SetActive(true,false); Transition = FSM_Transition::Succeeded; return;}
	} Transition = FSM_Transition::Aborted;
}

void UStateMachineComponent::MakeStates(const TArray<FName> &NewStates, FSM_Transition &Transition) {
	if (NewStates.Num()==0) {Transition = FSM_Transition::Aborted; return;}
	Transition = FSM_Transition::Unchecked;
	TArray<FName> COPY = NewStates;
	//
	for (auto &State : STATE) {
		State.Value.OnBeginState.Clear();
		State.Value.OnUpdateState.Clear();
		State.Value.OnExitState.Clear();
	} STATE.Empty(); STATES.Empty();
	//
	for (int32 I=0; I<COPY.Num(); ++I) {
		if (COPY[I] == NAME_None) {continue;}
		STATE.Add(I,COPY[I]);
		STATE[I].Owner = this;
		STATES.Add(COPY[I]);
	} Transition = FSM_Transition::Succeeded;
	//
	LinkStates();
}

void UStateMachineComponent::MakeStatesEnumerated(const UUserDefinedEnum* Enumeration, FSM_Transition &Transition) {
	if (!Enumeration||!Enumeration->IsValidLowLevel()) {Transition = FSM_Transition::Aborted; return;}
	Transition = FSM_Transition::Unchecked; Clear(false);
	//
	if (Enumeration->IsValidLowLevel()) {
		TMap<FName,FSM_Link> COPY = LINKS;
		LINKS.Empty(); STATES.Empty();
		TArray<FText> Enums; uint32 Index = 1;
		Enumerator->DisplayNameMap.GenerateValueArray(Enums);
		for (const auto &EN : Enums) {
			STATES.Add(FName(*EN.ToString())); FSM_Link Link;
			Link.NextState = (Enums.IsValidIndex(Index)) ? FName(*Enums[Index].ToString()) : NAME_None;
			Link.Function = FName(*FString(FString("OnFinish")+EN.ToString().Replace(TEXT(" "),TEXT(""))));
			LINKS.Add(FName(*EN.ToString()),Link);
		Index++;}
		//
		for (auto &LK : COPY) {
			if (COPY.Contains(LK.Key) && LINKS.Contains(LK.Key)) {
				LINKS.Emplace(LK.Key,COPY.FindChecked(LK.Key));
	}} COPY.Empty();}
	//
	Transition = FSM_Transition::Succeeded;
}

void UStateMachineComponent::AddState(const uint8 ID, const FName Name, FSM_Transition &Transition) {
	Transition = FSM_Transition::Unchecked;
	if (STATE.Contains(ID)) {
		LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("AddState")),FString("Can't add State, ID already exists."));
	Transition = FSM_Transition::Aborted; return;}
	if (ID>255) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("AddState")),FString("Warning: Max uint8 size exceeded!"));}
	//
	for (const auto &State : STATE) {
		if (State.Key != ID) {continue;}
		if (State.Value.Name.Compare(Name)==0) {
			LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("AddState")),FString("Can't add State, ID already exists."));
			Transition = FSM_Transition::Aborted;
	return;}}
	//
	STATE.Add(ID,FSM_State(*&Name));
	STATE[ID].Owner = this;
	STATES.Add(Name);
	Transition = FSM_Transition::Succeeded;
	//
	LinkStates();
}

void UStateMachineComponent::SetState(const FName Name, FSM_Transition &Transition) {
	if (!IsActive()) {return;}
	//
	const auto &ID = FindStateID(Name);
	Transition = FSM_Transition::Succeeded;
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetStateID(const uint8 ID, FSM_Transition &Transition) {
	if (!IsActive()) {return;}
	//
	Transition = FSM_Transition::Succeeded;
	if (!STATE.Contains(ID)) {Transition = FSM_Transition::Aborted; return;}
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetStateValue(const FSM_State &Value, FSM_Transition &Transition) {
	if (!IsActive()) {return;}
	//
	Transition = FSM_Transition::Succeeded;
	const auto &ID = FindStateID(Value.Name);
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetState_Conditional(const FName Name, FSM_Transition Condition, FSM_Transition &Transition) {
	if (!IsActive()) {return;}
	//
	const auto &ID = FindStateID(Name);
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	if (Exit(StateID,ID,Condition,InExit)) {
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);}
	//
	Transition = Condition;
}

void UStateMachineComponent::SetStateID_Conditional(const uint8 ID, FSM_Transition Condition, FSM_Transition &Transition) {
	if (!IsActive()) {return;}
	if (!STATE.Contains(ID)) {Transition = FSM_Transition::Aborted; return;}
	//
	if (Exit(StateID,ID,Condition,InExit)) {
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);}
	//
	Transition = Condition;
}

void UStateMachineComponent::SetStateValue_Conditional(const FSM_State &Value, FSM_Transition Condition, FSM_Transition &Transition) {
	if (!IsActive()) {return;}
	//
	const auto &ID = FindStateID(Value.Name);
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	if (Exit(StateID,ID,Condition,InExit)) {
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);}
	//
	Transition = Condition;
}

void UStateMachineComponent::SetStateWithCallbacks(const FName Name, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	if (!IsActive()) {return;}
	//
	const auto &ID = FindStateID(Name);
	Transition = FSM_Transition::Succeeded;
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetStateIDWithCallbacks(const uint8 ID, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	if (!IsActive()) {return;}
	//
	Transition = FSM_Transition::Succeeded;
	if (!STATE.Contains(ID)) {Transition = FSM_Transition::Aborted; return;}
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetStateValueWithCallbacks(const FSM_State &Value, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	if (!IsActive()) {return;}
	//
	Transition = FSM_Transition::Succeeded;
	const auto &ID = FindStateID(Value.Name);
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	InExit = ExitPreviousStateCallback;
	InBegin = BeginNextStateCallback;
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
}

void UStateMachineComponent::SetState_ConditionalWithCallbacks(const FName Name, FSM_Transition Condition, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	if (!IsActive()) {return;}
	//
	const auto &ID = FindStateID(Name);
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	InExit = ExitPreviousStateCallback;
	InBegin = BeginNextStateCallback;
	//
	if (Exit(StateID,ID,Condition,InExit)) {
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);}
	//
	Transition = Condition;
}

void UStateMachineComponent::SetStateID_ConditionalWithCallbacks(const uint8 ID, FSM_Transition Condition, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	if (!IsActive()) {return;}
	//
	if (!STATE.Contains(ID)) {Transition = FSM_Transition::Aborted; return;}
	//
	InExit = ExitPreviousStateCallback;
	InBegin = BeginNextStateCallback;
	//
	if (Exit(StateID,ID,Condition,InExit)) {
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);}
	//
	Transition = Condition;
}

void UStateMachineComponent::SetStateValue_ConditionalWithCallbacks(const FSM_State &Value, FSM_Transition Condition, FSM_Transition &Transition, const FSM_BP_ExitState &ExitPreviousStateCallback, const FSM_BP_BeginState &BeginNextStateCallback) {
	if (!IsActive()) {return;}
	//
	const auto &ID = FindStateID(Value.Name);
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	InExit = ExitPreviousStateCallback;
	InBegin = BeginNextStateCallback;
	//
	if (Exit(StateID,ID,Condition,InExit)) {
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);}
	//
	Transition = Condition;
}

void UStateMachineComponent::RemoveState(const FName Name, FSM_Transition &Transition) {
	const auto &ID = FindStateID(Name);
	Transition = FSM_Transition::Unchecked;
	if (ID == INDEX_NONE) {Transition = FSM_Transition::Aborted; return;}
	//
	if (STATE.Contains(ID) && STATE[ID].Name.Compare(Name)==0) {
		STATE[ID].OnBeginState.Clear();
		STATE[ID].OnUpdateState.Clear();
		STATE[ID].OnExitState.Clear();
		STATE.Remove(ID);
		STATES.Remove(Name);
		LINKS.Remove(Name);
	Transition = FSM_Transition::Succeeded;}
}

void UStateMachineComponent::FinishState(FSM_Transition &Transition) {
	Transition = FSM_Transition::Unchecked;
	const auto State = GetActiveState().Name;
	auto Next = LINKS.Find(State);
	//
	if (State != NAME_None && Next != NULL) {
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::CreateNext)) {
			AddState(GetLastStateID()+1,Next->NextState,Transition);
		Next = LINKS.Find(State);}
		//
		FSM_ExitArguments.StateID = StateID;
		FSM_ExitArguments.NextStateName = Next->NextState;
		FSM_ExitArguments.NextStateID = FindStateID(Next->NextState);
		if (Next) {SetState(Next->NextState,Transition);} else {Transition = FSM_Transition::Aborted;}
		//
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::ReturnAborted) && Transition == FSM_Transition::Aborted) {SetState(State);}
		if (Transition == FSM_Transition::Aborted) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("FinishState")),FString(": LINK Transition Aborted!")); return;}
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::RemovesState) && GetActiveState().Name != State) {RemoveState(State,Transition);}
		//
		UFunction* Function = FindFunction(Next->Function);
		if (Function && Function->NumParms==0) {ProcessEvent(Function,NULL);} else
		if (Function && Function->NumParms==1) {ProcessEvent(Function,&FSM_ExitArguments);}
		//
		if (!Next) {return;}
		LOG_FSM(Debug,true,3.f,FColor::White,GetName()+FString(": Executing LINK Transition:: [")+State.ToString()+FString("]---------->(")+Next->NextState.ToString()+FString(")"));
	} else {LOG_FSM(Debug,true,3.f,FColor::Red,GetName()+FString(" [")+FString("LINK")+FString("]: Failed to execute Default Transition for Current State!"));}
}

void UStateMachineComponent::FinishStateWithCallback(FSM_Transition &Transition, const FSM_BP_ExitState &Callback) {
	Transition = FSM_Transition::Unchecked;
	const auto State = GetActiveState().Name;
	auto Next = LINKS.Find(State);
	//
	if (State != NAME_None && Next != NULL) {
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::CreateNext)) {
			AddState(GetLastStateID()+1,Next->NextState,Transition);
		Next = LINKS.Find(State);}
		//
		InExit = Callback;
		FSM_ExitArguments.StateID = StateID;
		FSM_ExitArguments.NextStateName = Next->NextState;
		FSM_ExitArguments.NextStateID = FindStateID(Next->NextState);
		if (Next) {SetStateWithCallbacks(Next->NextState,Transition,InExit,InBegin);} else {Transition = FSM_Transition::Aborted;}
		//
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::ReturnAborted) && Transition == FSM_Transition::Aborted) {SetState(State);}
		if (Transition == FSM_Transition::Aborted) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("FinishState")),FString(": LINK Transition Aborted!")); return;}
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::RemovesState) && GetActiveState().Name != State) {RemoveState(State,Transition);}
		//
		UFunction* Function = FindFunction(Next->Function);
		if (Function && Function->NumParms==0) {ProcessEvent(Function,NULL);} else
		if (Function && Function->NumParms==1) {ProcessEvent(Function,&FSM_ExitArguments);}
		//
		if (!Next) {return;}
		LOG_FSM(Debug,true,3.f,FColor::White,GetName()+FString(": Executing LINK Transition:: [")+State.ToString()+FString("]---------->(")+Next->NextState.ToString()+FString(")"));
	} else {LOG_FSM(Debug,true,3.f,FColor::Red,GetName()+FString(" [")+FString("LINK")+FString("]: Failed to execute Default Transition for Current State!"));}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::LinkStates() {
	TMap<FName,FSM_Link> COPY = LINKS;
	LINKS.Empty(); uint32 Index = 1;
	//
	for (const auto &ST : STATES) {
		FSM_Link Link;
		Link.NextState = (STATES.IsValidIndex(Index)) ? STATES[Index] : NAME_None;
		Link.Function = FName(*FString(FString("OnFinish")+ST.ToString().Replace(TEXT(" "),TEXT(""))));
		LINKS.Add(ST,Link); Index++;
	} for (auto &LK : COPY) {
		if (COPY.Contains(LK.Key) && LINKS.Contains(LK.Key)) {
			LINKS.Emplace(LK.Key,COPY.FindChecked(LK.Key));
	}} COPY.Empty();
}

void UStateMachineComponent::Clear(bool Unlink) {
	for (auto &State : STATE) {
		State.Value.OnBeginState.Clear();
		State.Value.OnUpdateState.Clear();
		State.Value.OnExitState.Clear();
	} STATE.Empty(); STATES.Empty();
	//
	if (Unlink) {LINKS.Empty();}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool UStateMachineComponent::SERVER_GoToFSM_Validate(UStateMachineComponent* TargetMachine, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_GoToFSM")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_GoToFSM_Implementation(UStateMachineComponent* TargetMachine, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_GoToFSM")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_GoToFSM")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; GoToFSM(TargetMachine,Transition);
}

bool UStateMachineComponent::SERVER_ActivateFSM_Validate(const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_ActivateFSM")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_ActivateFSM_Implementation(const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_ActivateFSM")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_ActivateFSM")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; ActivateFSM(Transition);
}

bool UStateMachineComponent::SERVER_MakeStates_Validate(const TArray<FName> &NewStates, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_MakeStates")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_MakeStates_Implementation(const TArray<FName> &NewStates, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_MakeStates")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_MakeStates")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; MakeStates(NewStates,Transition);
}

bool UStateMachineComponent::SERVER_MakeStatesEnumerated_Validate(const UUserDefinedEnum* Enumeration, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_MakeStatesEnumerated")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_MakeStatesEnumerated_Implementation(const UUserDefinedEnum* Enumeration, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_MakeStatesEnumerated")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_MakeStatesEnumerated")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; MakeStatesEnumerated(Enumeration,Transition);
}

bool UStateMachineComponent::SERVER_AddState_Validate(const uint8 ID, const FName Name, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_AddState")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_AddState_Implementation(const uint8 ID, const FName Name, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_AddState")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_AddState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; AddState(ID,Name,Transition);
}

bool UStateMachineComponent::SERVER_SetState_Validate(const FName Name, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_SetState")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_SetState_Implementation(const FName Name, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_SetState")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_SetState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; SetState(Name,Transition);
}

bool UStateMachineComponent::SERVER_SetStateID_Validate(const uint8 ID, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_SetStateID")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_SetStateID_Implementation(const uint8 ID, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_SetStateID")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_SetStateID")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; SetStateID(ID,Transition);
}

bool UStateMachineComponent::SERVER_SetStateValue_Validate(const FSM_State &Value, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_SetStateValue")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_SetStateValue_Implementation(const FSM_State &Value, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_SetStateValue")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_SetStateValue")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; SetStateValue(Value,Transition);
}

bool UStateMachineComponent::SERVER_RemoveState_Validate(const FName Name, const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_RemoveState")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_RemoveState_Implementation(const FName Name, const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_RemoveState")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_RemoveState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; RemoveState(Name,Transition);
}

bool UStateMachineComponent::SERVER_FinishState_Validate(const bool Validate) {
	if (!Validate) {
		LOG_PIE(ESeverity::Error,GetOwner(),FFIND(TEXT("SERVER_FinishState")),FString("[Client-to-Server] RPC Validation Failed!"));
	} return Validate;
}

void UStateMachineComponent::SERVER_FinishState_Implementation(const bool Validate) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if ((NET!=NM_DedicatedServer)&&(NET!=NM_ListenServer)) {LOG_PIE(Debug,ESeverity::Info,GetOwner(),FFIND(TEXT("SERVER_FinishState")),FString("Executing [Client-to-Server] RPC on Client!"));}
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("SERVER_FinishState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; FinishState(Transition);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::CLIENT_GoToFSM_Implementation(UStateMachineComponent* TargetMachine) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_GoToFSM")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; GoToFSM(TargetMachine,Transition);
}

void UStateMachineComponent::CLIENT_ActivateFSM_Implementation() {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_ActivateFSM")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; ActivateFSM(Transition);
}

void UStateMachineComponent::CLIENT_MakeStates_Implementation(const TArray<FName> &NewStates) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_MakeStates")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; MakeStates(NewStates,Transition);
}

void UStateMachineComponent::CLIENT_MakeStatesEnumerated_Implementation(const UUserDefinedEnum* Enumeration) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_MakeStatesEnumerated")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; MakeStatesEnumerated(Enumeration,Transition);
}

void UStateMachineComponent::CLIENT_AddState_Implementation(const uint8 ID, const FName Name) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_AddState")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; AddState(ID,Name,Transition);
}

void UStateMachineComponent::CLIENT_SetState_Implementation(const FName Name) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_SetState")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; SetState(Name,Transition);
}

void UStateMachineComponent::CLIENT_SetStateID_Implementation(const uint8 ID) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_SetStateID")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; SetStateID(ID,Transition);
}

void UStateMachineComponent::CLIENT_SetStateValue_Implementation(const FSM_State &Value) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_SetStateValue")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; SetStateValue(Value,Transition);
}

void UStateMachineComponent::CLIENT_RemoveState_Implementation(const FName Name) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_RemoveState")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; RemoveState(Name,Transition);
}

void UStateMachineComponent::CLIENT_FinishState_Implementation() {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		const ENetMode NET = GetOwner()->GetNetMode();
		if (NET!=NM_Client) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("CLIENT_FinishState")),FString("Executing [Server-to-Client] RPC on Server!"));}
	} FSM_Transition Transition; FinishState(Transition);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::MULTI_GoToFSM_Implementation(UStateMachineComponent* TargetMachine) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_GoToFSM")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; GoToFSM(TargetMachine,Transition);
}

void UStateMachineComponent::MULTI_ActivateFSM_Implementation() {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_ActivateFSM")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; ActivateFSM(Transition);
}

void UStateMachineComponent::MULTI_MakeStates_Implementation(const TArray<FName> &NewStates) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_MakeStates")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; MakeStates(NewStates,Transition);
}

void UStateMachineComponent::MULTI_MakeStatesEnumerated_Implementation(const UUserDefinedEnum* Enumeration) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_MakeStatesEnumerated")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; MakeStatesEnumerated(Enumeration,Transition);
}

void UStateMachineComponent::MULTI_AddState_Implementation(const uint8 ID, const FName Name) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_AddState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; AddState(ID,Name,Transition);
}

void UStateMachineComponent::MULTI_SetState_Implementation(const FName Name) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_SetState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; SetState(Name,Transition);
}

void UStateMachineComponent::MULTI_SetStateID_Implementation(const uint8 ID) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_SetStateID")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; SetStateID(ID,Transition);
}

void UStateMachineComponent::MULTI_SetStateValue_Implementation(const FSM_State &Value) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	if (!IsActive()) {return;}
	//
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_SetStateValue")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; SetStateValue(Value,Transition);
}

void UStateMachineComponent::MULTI_RemoveState_Implementation(const FName Name) {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_RemoveState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; RemoveState(Name,Transition);
}

void UStateMachineComponent::MULTI_FinishState_Implementation() {
	const auto &Settings = GetMutableDefault<UFSMSettings>();
	//
	if (!Settings->SuppressRPCWarnings) {
		if ((GetOwner()->GetRemoteRole()!=ROLE_AutonomousProxy)&&(GetOwner()->Role==ROLE_Authority)) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("MULTI_FinishState")),FString("Caller is [Local-Authority]!"));}
	} FSM_Transition Transition; FinishState(Transition);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UStateMachineComponent::OnREP_StateID_Implementation(uint8 &ID) {
	ID = StateID;
}

void UStateMachineComponent::OnREP_PreviousStateID_Implementation(uint8 &PreviousID) {
	PreviousID = PreviousStateID;
}

void UStateMachineComponent::OnREP_StateTime_Implementation(float &Time) {
	Time = StateTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FSM_State &UStateMachineComponent::GetActiveState() {
	if (!STATE.Contains(StateID)) {return STATE_Null;}
	return STATE.FindChecked(StateID);
}

FSM_Transition UStateMachineComponent::Pure_AddState(const uint8 ID, const FName Name) {
	if (STATE.Contains(ID)) {
		LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("Pure_AddState")),FString("Can't add State, ID already exists."));
	return FSM_Transition::Aborted;}
	//
	if (ID>255) {LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("Pure_AddState")),FString("Warning: Max uint8 size exceeded!"));}
	//
	for (const auto &State : STATE) {
		if (State.Key != ID) {continue;}
		if (State.Value.Name.Compare(Name)==0) {
			LOG_PIE(Debug,ESeverity::Warning,GetOwner(),FFIND(TEXT("Pure_AddState")),FString("Can't add State, ID already exists."));
		return FSM_Transition::Aborted;
	}}
	//
	STATE.Add(ID,FSM_State(*&Name));
	STATE[ID].Owner = this;
	STATES.Add(Name);
	return FSM_Transition::Succeeded;
	//
	LinkStates();
}

FSM_Transition UStateMachineComponent::Pure_SetState(const FName Name) {
	if (!IsActive()) {return FSM_Transition::Aborted;}
	//
	const auto &ID = FindStateID(Name);
	FSM_Transition Transition = FSM_Transition::Succeeded;
	if (ID == INDEX_NONE) {return FSM_Transition::Aborted;}
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateID = (uint8)ID;
	 StateTime = 0.0f;
	Begin(StateID,InBegin);
	//
	return Transition;
}

FSM_Transition UStateMachineComponent::Pure_SetStateByID(const uint8 ID) {
	if (!IsActive()) {return FSM_Transition::Aborted;}
	//
	FSM_Transition Transition = FSM_Transition::Succeeded;
	if (!STATE.Contains(ID)) {return FSM_Transition::Aborted;}
	//
	Exit(StateID,ID,Transition,InExit);
	 PreviousStateID = StateID;
	 StateTime = 0.0f;
	 StateID = ID;
	Begin(StateID,InBegin);
	//
	return Transition;
}

FSM_Transition UStateMachineComponent::Pure_RemoveState(const FName Name) {
	const auto &ID = FindStateID(Name);
	if (ID == INDEX_NONE) {return FSM_Transition::Aborted;}
	//
	if (STATE.Contains(ID) && STATE[ID].Name.Compare(Name)==0) {
		STATE[ID].OnBeginState.Clear();
		STATE[ID].OnUpdateState.Clear();
		STATE[ID].OnExitState.Clear();
		STATE.Remove(ID);
		STATES.Remove(Name);
		LINKS.Remove(Name);
	return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UStateMachineComponent::Pure_FinishState() {
	FSM_Transition Result = FSM_Transition::Unchecked;
	const auto State = GetActiveState().Name;
	auto Next = LINKS.Find(State);
	//
	if (State != NAME_None && Next != NULL) {
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::CreateNext)) {
			AddState(GetLastStateID()+1,Next->NextState,Result);
		Next = LINKS.Find(State);}
		//
		if (Next) {SetState(Next->NextState,Result);} else {Result = FSM_Transition::Aborted;}
		//
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::ReturnAborted) && Result == FSM_Transition::Aborted) {SetState(State);}
		if (Result == FSM_Transition::Aborted) {LOG_FSM(Debug,true,3.f,FColor::Yellow,GetName()+FString(": ")+State.ToString()+FString(": LINK Transition Aborted!")); return Result;}
		if (BITMASK_CHECK(Next->Rule,FSM_Rule::RemovesState) && GetActiveState().Name != State) {RemoveState(State,Result);}
		//
		UFunction* Function = FindFunction(Next->Function);
		if (Function) {ProcessEvent(Function,NULL);}
		//
		if (!Next) {return Result;}
		LOG_FSM(Debug,true,3.f,FColor::White,GetName()+FString(": Executing LINK Transition:: [")+State.ToString()+FString("]---------->(")+Next->NextState.ToString()+FString(")"));
	} else {LOG_FSM(Debug,true,3.f,FColor::Red,GetName()+FString(" [")+FString("LINK")+FString("]: Failed to execute Default Transition for Current State!")); return Result;} return Result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if WITH_EDITOR
//
void UStateMachineComponent::PostEditChangeProperty(struct FPropertyChangedEvent &PECP) {
	FName Property = (PECP.Property != nullptr) ? PECP.Property->GetFName() : NAME_None;
	//
	if (Property==GET_MEMBER_NAME_CHECKED(UStateMachineComponent,Enumerator) && Enumerator->IsValidLowLevel()) {
		TMap<FName,FSM_Link> COPY = LINKS;
		LINKS.Empty(); STATES.Empty();
		TArray<FText> Enums; uint32 Index = 1;
		Enumerator->DisplayNameMap.GenerateValueArray(Enums);
		for (const auto &EN : Enums) {
			STATES.Add(FName(*EN.ToString())); FSM_Link Link;
			Link.NextState = (Enums.IsValidIndex(Index)) ? FName(*Enums[Index].ToString()) : NAME_None;
			Link.Function = FName(*FString(FString("OnFinish")+EN.ToString().Replace(TEXT(" "),TEXT(""))));
			LINKS.Add(FName(*EN.ToString()),Link);
		Index++;}
		//
		for (auto &LK : COPY) {
			if (COPY.Contains(LK.Key) && LINKS.Contains(LK.Key)) {
				LINKS.Emplace(LK.Key,COPY.FindChecked(LK.Key));
	}} COPY.Empty();}
	//
	if (Property==GET_MEMBER_NAME_CHECKED(UStateMachineComponent,STATES)) {LinkStates();}
	if (Property==GET_MEMBER_NAME_CHECKED(UStateMachineComponent,ReplicateStateID)) {SetIsReplicated((ReplicateStateID||ReplicatePreviousStateID||ReplicateStateTime));}
	if (Property==GET_MEMBER_NAME_CHECKED(UStateMachineComponent,ReplicatePreviousStateID)) {SetIsReplicated((ReplicateStateID||ReplicatePreviousStateID||ReplicateStateTime));}
	if (Property==GET_MEMBER_NAME_CHECKED(UStateMachineComponent,ReplicateStateTime)) {SetIsReplicated((ReplicateStateID||ReplicatePreviousStateID||ReplicateStateTime));}
	//
	Super::PostEditChangeProperty(PECP);
}
//
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////