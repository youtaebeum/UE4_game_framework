// Fill out your copyright notice in the Description page of Project Settings.

#include "base_actor.h"

// Sets default values.
A_base_actor::A_base_actor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	m_ui_uniq_index = 0;
}

// Called when the game starts or when spawned
void A_base_actor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void A_base_actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void A_base_actor::_initialize(uint32 _uiUniqIndex)
{
	m_ui_uniq_index = _uiUniqIndex;
}

void A_base_actor::_reset()
{
	m_ui_uniq_index = GC_INDEX_NONE;
}