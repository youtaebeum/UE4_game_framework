/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreUObject.h"
#include "UFSMStateMachineComponent.h"
#include "Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"

#include "UFSMFunctionLibrary.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class UFSM_API UFSM_Library : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	/** Evaluates a FSM Transition result from a Transition Comparison. */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Bool Compare", CompactNodeTitle = "Evaluate", Keywords = "FSM Evaluate Bool"))
	static FSM_Transition FSM_EvaluateTransitions(UPARAM(ref)FSM_Transition &Equals, UPARAM(ref)FSM_Transition &To);
	//
	//
	/** Evaluates a FSM Transition result from Boolean Value. */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Bool", CompactNodeTitle = "Evaluate", Keywords = "FSM Evaluate Bool"))
	static FSM_Transition FSM_EvaluateBool(UPARAM(ref)bool &Bool);
	//
	/** Evaluates a FSM Transition result from two Boolean Values. */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Bools", CompactNodeTitle = "Evaluate", Keywords = "FSM Evaluate Bool"))
	static FSM_Transition FSM_EvaluateTwoBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB);
	//
	/** Evaluates a FSM Transition result from three Boolean Values. */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Bools", CompactNodeTitle = "Evaluate", Keywords = "FSM Evaluate Bool"))
	static FSM_Transition FSM_EvaluateThreeBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB, UPARAM(ref)bool &AndC);
	//
	/** Evaluates a FSM Transition result from four Boolean Values. */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Bools", CompactNodeTitle = "Evaluate", Keywords = "FSM Evaluate Bool"))
	static FSM_Transition FSM_EvaluateFourBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB, UPARAM(ref)bool &AndC, UPARAM(ref)bool &AndD);
	//
	/** Evaluates a FSM Transition result from five Boolean Values. */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Bools", CompactNodeTitle = "Evaluate", Keywords = "FSM Evaluate Bool"))
	static FSM_Transition FSM_EvaluateFiveBools(UPARAM(ref)bool &BoolA, UPARAM(ref)bool &AndB, UPARAM(ref)bool &AndC, UPARAM(ref)bool &AndD, UPARAM(ref)bool &AndE);
	//
	/** Evaluates a FSM Transition result from a Boolean Comparison (==). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Bool Compare", CompactNodeTitle = "Eval (==)", Keywords = "FSM Evaluate Bool"))
	static FSM_Transition FSM_EvaluateBoolEquals(UPARAM(ref)bool &Equals, UPARAM(ref)bool &To);
	//
	//
	/** Evaluates a FSM Transition result from a Byte Comparison (==). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Byte Compare", CompactNodeTitle = "Eval (==)", Keywords = "FSM Evaluate Byte"))
	static FSM_Transition FSM_EvaluateByteEquals(UPARAM(ref)uint8 &Equals, UPARAM(ref)uint8 &To);
	//
	/** Evaluates a FSM Transition result from a Byte Comparison (>=). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Byte Compare", CompactNodeTitle = "Eval (>=)", Keywords = "FSM Evaluate Byte"))
	static FSM_Transition FSM_EvaluateByteGreaterOrEqual(UPARAM(ref)uint8 &IsGreaterOrEquals, UPARAM(ref)uint8 &To);
	//
	/** Evaluates a FSM Transition result from a Byte Comparison (<=). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Byte Compare", CompactNodeTitle = "Eval (<=)", Keywords = "FSM Evaluate Byte"))
	static FSM_Transition FSM_EvaluateByteSmallerOrEqual(UPARAM(ref)uint8 &IsSmallerOrEquals, UPARAM(ref)uint8 &To);
	//
	/** Evaluates a FSM Transition result from a Byte Comparison (>). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Byte Compare", CompactNodeTitle = "Eval (>)", Keywords = "FSM Evaluate Byte"))
	static FSM_Transition FSM_EvaluateByteGreater(UPARAM(ref)uint8 &IsGreater, UPARAM(ref)uint8 &Than);
	//
	/** Evaluates a FSM Transition result from a Byte Comparison (<). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Byte Compare", CompactNodeTitle = "Eval (<)", Keywords = "FSM Evaluate Byte"))
	static FSM_Transition FSM_EvaluateByteSmaller(UPARAM(ref)uint8 &IsSmaller, UPARAM(ref)uint8 &Than);
	//
	//
	/** Evaluates a FSM Transition result from a Int Comparison (==). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Int Compare", CompactNodeTitle = "Eval (==)", Keywords = "FSM Evaluate Int"))
	static FSM_Transition FSM_EvaluateIntEquals(UPARAM(ref)int32 &Equals, UPARAM(ref)int32 &To);
	//
	/** Evaluates a FSM Transition result from a Int Comparison (>=). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Int Compare", CompactNodeTitle = "Eval (>=)", Keywords = "FSM Evaluate Int"))
	static FSM_Transition FSM_EvaluateIntGreaterOrEqual(UPARAM(ref)int32 &IsGreaterOrEquals, UPARAM(ref)int32 &To);
	//
	/** Evaluates a FSM Transition result from a Int Comparison (<=). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Int Compare", CompactNodeTitle = "Eval (<=)", Keywords = "FSM Evaluate Int"))
	static FSM_Transition FSM_EvaluateIntSmallerOrEqual(UPARAM(ref)int32 &IsSmallerOrEquals, UPARAM(ref)int32 &To);
	//
	/** Evaluates a FSM Transition result from a Int Comparison (>). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Int Compare", CompactNodeTitle = "Eval (>)", Keywords = "FSM Evaluate Int"))
	static FSM_Transition FSM_EvaluateIntGreater(UPARAM(ref)int32 &IsGreater, UPARAM(ref)int32 &Than);
	//
	/** Evaluates a FSM Transition result from a Int Comparison (<). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Int Compare", CompactNodeTitle = "Eval (<)", Keywords = "FSM Evaluate Int"))
	static FSM_Transition FSM_EvaluateIntSmaller(UPARAM(ref)int32 &IsSmaller, UPARAM(ref)int32 &Than);
	//
	//
	/** Evaluates a FSM Transition result from a Float Comparison (==). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Float Compare", CompactNodeTitle = "Eval (==)", Keywords = "FSM Evaluate Float"))
	static FSM_Transition FSM_EvaluateFloatEquals(UPARAM(ref)float &Equals, UPARAM(ref)float &To);
	//
	/** Evaluates a FSM Transition result from a Float Comparison (>=). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Float Compare", CompactNodeTitle = "Eval (>=)", Keywords = "FSM Evaluate Float"))
	static FSM_Transition FSM_EvaluateFloatGreaterOrEqual(UPARAM(ref)float &IsGreaterOrEquals, UPARAM(ref)float &To);
	//
	/** Evaluates a FSM Transition result from a Float Comparison (<=). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Float Compare", CompactNodeTitle = "Eval (<=)", Keywords = "FSM Evaluate Float"))
	static FSM_Transition FSM_EvaluateFloatSmallerOrEqual(UPARAM(ref)float &IsSmallerOrEquals, UPARAM(ref)float &To);
	//
	/** Evaluates a FSM Transition result from a Float Comparison (>). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Float Compare", CompactNodeTitle = "Eval (>)", Keywords = "FSM Evaluate Float"))
	static FSM_Transition FSM_EvaluateFloatGreater(UPARAM(ref)float &IsGreater, UPARAM(ref)float &Than);
	//
	/** Evaluates a FSM Transition result from a Float Comparison (<). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Float Compare", CompactNodeTitle = "Eval (<)", Keywords = "FSM Evaluate Float"))
	static FSM_Transition FSM_EvaluateFloatSmaller(UPARAM(ref)float &IsSmaller, UPARAM(ref)float &Than);
	//
	//
	/** Evaluates a FSM Transition result from a Name Comparison (==). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Name Compare", CompactNodeTitle = "Eval (==)", Keywords = "FSM Evaluate Name"))
	static FSM_Transition FSM_EvaluateNameEquals(UPARAM(ref)FName &Equals, UPARAM(ref)FName &To);
	//
	//
	/** Evaluates a FSM Transition result from a String Comparison (==). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate String Compare", CompactNodeTitle = "Eval (==)", Keywords = "FSM Evaluate String"))
	static FSM_Transition FSM_EvaluateStringEquals(UPARAM(ref)FString &Equals, UPARAM(ref)FString &To);
	//
	//
	/** Evaluates a FSM Transition result from a Text Comparison (==). */
	UFUNCTION(Category = "FSM|Utility", BlueprintPure, meta = (DisplayName = "FSM: Evaluate Text Compare", CompactNodeTitle = "Eval (==)", Keywords = "FSM Evaluate Text"))
	static FSM_Transition FSM_EvaluateTextEquals(UPARAM(ref)FText &Equals, UPARAM(ref)FText &To);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////