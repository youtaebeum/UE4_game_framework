// Fill out your copyright notice in the Description page of Project Settings.

#include "fsm_actor.h"

// Sets default values
A_fsm_actor::A_fsm_actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_state_machine = CreateDefaultSubobject<UStateMachineComponent>(TEXT("state_machine"));
}

// Called when the game starts or when spawned
void A_fsm_actor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void A_fsm_actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

