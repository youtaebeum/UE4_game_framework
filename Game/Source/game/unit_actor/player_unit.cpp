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

/*
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Barbarous.SK_CharM_Barbarous'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/sk_CharM_Base.sk_CharM_Base'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Bladed.SK_CharM_Bladed'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Forge.SK_CharM_Forge'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_FrostGiant.SK_CharM_FrostGiant'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Natural.SK_CharM_Natural'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Pit.SK_CharM_Pit'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ragged0.SK_CharM_Ragged0'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_RaggedElite.SK_CharM_RaggedElite'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ram.SK_CharM_Ram'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Robo.SK_CharM_Robo'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Shell.SK_CharM_Shell'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_solid.SK_CharM_solid'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Standard.SK_CharM_Standard'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Tusk.SK_CharM_Tusk'");
_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior'");
*/