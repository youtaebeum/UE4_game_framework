// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "UFSM.h"
#include "GameFramework/Actor.h"
#include "fsm_actor.generated.h"

UCLASS()
class GAMECORE_API A_fsm_actor : public AActor
{
	GENERATED_BODY()

	A_fsm_actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
	UStateMachineComponent* m_state_machine = nullptr;

public:
	UStateMachineComponent* get_state_machine() { return m_state_machine; }
};
