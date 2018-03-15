/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2017 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UFSM.h"

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "AnimGraphDefinitions.h"
#include "UObject/ObjectMacros.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "UFSMAnimGraphNodes.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS(ClassGroup = Synaptech, Category = "FSM", meta = (DisplayName = "FSM: Blend State Machines", ShortTooltip = "FSM: Easy blend of two Animation State Machines or Cached Poses."))
class UFSMDEVELOPER_API UFSM_BlendStateMachines_AnimGraphNode : public UAnimGraphNode_Base {
	GENERATED_UCLASS_BODY()
	UPROPERTY(Category = "FSM", EditAnywhere)
	FSM_BlendStateMachines Node;
protected:
	virtual FText GetControllerDescription() const;
public:
	virtual FText GetTooltipText() const override;
	virtual FName GetCornerIcon() const override;
	virtual FString GetNodeCategory() const override;
	virtual bool ShowPaletteIconOnNode() const override;
	virtual FString GetDocumentationLink() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS(ClassGroup = Synaptech, Category = "FSM", meta = (DisplayName = "FSM: Set State", ShortTooltip = "FSM: Set State of Named FSM Component on Actor Owner of this Animation Graph Blueprint."))
class UFSMDEVELOPER_API UFSM_SetMachineState_AnimGraphNode : public UAnimGraphNode_Base {
	GENERATED_UCLASS_BODY()
	UPROPERTY(Category = "FSM", EditAnywhere)
	FSM_SetMachineState Node;
protected:
	virtual FText GetControllerDescription() const;
public:
	virtual FText GetTooltipText() const override;
	virtual FName GetCornerIcon() const override;
	virtual FString GetNodeCategory() const override;
	virtual bool ShowPaletteIconOnNode() const override;
	virtual FString GetDocumentationLink() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*UCLASS(ClassGroup = Synaptech, Category = "FSM", meta = (DisplayName = "FSM: Get State", ShortTooltip = "FSM: Get State of Named FSM Component on Actor Owner of this Animation Graph Blueprint."))
class UFSMDEVELOPER_API UFSM_GetMachineState_AnimGraphNode : public UAnimGraphNode_Base {
	GENERATED_UCLASS_BODY()
	UPROPERTY(Category = "FSM", EditAnywhere)
	FSM_GetMachineState Node;
protected:
	virtual FText GetControllerDescription() const;
public:
	virtual FText GetTooltipText() const override;
	virtual FName GetCornerIcon() const override;
	virtual FString GetNodeCategory() const override;
	virtual bool ShowPaletteIconOnNode() const override;
	virtual FString GetDocumentationLink() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////