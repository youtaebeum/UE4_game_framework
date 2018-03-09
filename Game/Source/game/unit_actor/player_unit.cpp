// Fill out your copyright notice in the Description page of Project Settings.

#include "player_unit.h"
#include "gamecore_include.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Engine/SkeletalMesh.h"
#include "Engine/AssetManager.h"

// Sets default values.
A_player_unit::A_player_unit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void A_player_unit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void A_player_unit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void A_player_unit::_initialize(uint32 _uiUniqIndex)
{
	Super::_initialize(_uiUniqIndex);
}

void A_player_unit::_reset()
{
	Super::_reset();
}