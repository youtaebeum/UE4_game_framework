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

	add_mesh_componenet(0, m_p_mesh_head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("head")), false);
	add_mesh_componenet(1, m_p_mesh_body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("body")), false);
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

	change_mesh(0, TEXT("SkeletalMesh'/Game/Meliodas/mesh/hero_meliodas_body_0014.hero_meliodas_body_0014'"));
	change_mesh(1, TEXT("SkeletalMesh'/Game/Meliodas/mesh/hero_meliodas_head_0000.hero_meliodas_head_0000'"));

	set_anim_instance(TEXT("AnimBlueprint'/Game/Meliodas/anim_bp_meliodas.anim_bp_meliodas_C'"));
}

void A_player_unit::_reset()
{
	Super::_reset();
}