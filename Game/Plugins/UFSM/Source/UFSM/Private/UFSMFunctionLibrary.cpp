/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UFSMFunctionLibrary.h"
#include "UFSMShared.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FSM_Transition UFSM_Library::FSM_EvaluateTransitions(UPARAM(ref)FSM_Transition &Equals, UPARAM(ref)FSM_Transition &To) {
	if ((*&Equals)==(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FSM_Transition UFSM_Library::FSM_EvaluateBool(UPARAM(ref)bool &Bool) {
	if ((*&Bool)==true) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateTwoBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB) {
	if (((*&BoolA)==true)&&((*&AndB)==true)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateThreeBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB, UPARAM(ref)bool &AndC) {
	if (((*&BoolA)==true)&&((*&AndB)==true)&&((*&AndC)==true)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateFourBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB, UPARAM(ref)bool &AndC, UPARAM(ref)bool &AndD) {
	if (((*&BoolA)==true)&&((*&AndB)==true)&&((*&AndC)==true)&&((*&AndD)==true)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateFiveBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB, UPARAM(ref)bool &AndC, UPARAM(ref)bool &AndD, UPARAM(ref)bool &AndE) {
	if (((*&BoolA)==true)&&((*&AndB)==true)&&((*&AndC)==true)&&((*&AndD)==true)&&((*&AndE)==true)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateBoolEquals(UPARAM(ref)bool &Equals, UPARAM(ref)bool &To) {
	if ((*&Equals)==(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FSM_Transition UFSM_Library::FSM_EvaluateByteEquals(UPARAM(ref)uint8 &Equals, UPARAM(ref)uint8 &To) {
	if ((*&Equals)==(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateByteGreaterOrEqual(UPARAM(ref)uint8 &IsGreaterOrEquals, UPARAM(ref)uint8 &To) {
	if ((*&IsGreaterOrEquals)>=(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateByteSmallerOrEqual(UPARAM(ref)uint8 &IsSmallerOrEquals, UPARAM(ref)uint8 &To) {
	if ((*&IsSmallerOrEquals)<=(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateByteGreater(UPARAM(ref)uint8 &IsGreater, UPARAM(ref)uint8 &Than) {
	if ((*&IsGreater)>(*&Than)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateByteSmaller(UPARAM(ref)uint8 &IsSmaller, UPARAM(ref)uint8 &Than) {
	if ((*&IsSmaller)<(*&Than)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FSM_Transition UFSM_Library::FSM_EvaluateIntEquals(UPARAM(ref)int32 &Equals, UPARAM(ref)int32 &To) {
	if ((*&Equals)==(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateIntGreaterOrEqual(UPARAM(ref)int32 &IsGreaterOrEquals, UPARAM(ref)int32 &To) {
	if ((*&IsGreaterOrEquals)>=(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateIntSmallerOrEqual(UPARAM(ref)int32 &IsSmallerOrEquals, UPARAM(ref)int32 &To) {
	if ((*&IsSmallerOrEquals)<=(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateIntGreater(UPARAM(ref)int32 &IsGreater, UPARAM(ref)int32 &Than) {
	if ((*&IsGreater)>(*&Than)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateIntSmaller(UPARAM(ref)int32 &IsSmaller, UPARAM(ref)int32 &Than) {
	if ((*&IsSmaller)<(*&Than)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FSM_Transition UFSM_Library::FSM_EvaluateFloatEquals(UPARAM(ref)float &Equals, UPARAM(ref)float &To) {
	if ((*&Equals)==(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateFloatGreaterOrEqual(UPARAM(ref)float &IsGreaterOrEquals, UPARAM(ref)float &To) {
	if ((*&IsGreaterOrEquals)>=(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateFloatSmallerOrEqual(UPARAM(ref)float &IsSmallerOrEquals, UPARAM(ref)float &To) {
	if ((*&IsSmallerOrEquals)<=(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateFloatGreater(UPARAM(ref)float &IsGreater, UPARAM(ref)float &Than) {
	if ((*&IsGreater)>(*&Than)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateFloatSmaller(UPARAM(ref)float &IsSmaller, UPARAM(ref)float &Than) {
	if ((*&IsSmaller)<(*&Than)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FSM_Transition UFSM_Library::FSM_EvaluateNameEquals(UPARAM(ref)FName &Equals, UPARAM(ref)FName &To) {
	if ((*&Equals)==(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateStringEquals(UPARAM(ref)FString &Equals, UPARAM(ref)FString &To) {
	if ((*&Equals)==(*&To)) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

FSM_Transition UFSM_Library::FSM_EvaluateTextEquals(UPARAM(ref)FText &Equals, UPARAM(ref)FText &To) {
	if ((*&Equals).ToString()==(*&To).ToString()) {return FSM_Transition::Succeeded;}
	return FSM_Transition::Aborted;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////