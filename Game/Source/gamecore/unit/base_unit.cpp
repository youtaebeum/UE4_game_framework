// Fill out your copyright notice in the Description page of Project Settings.

#include "base_unit.h"
#include "gamecore_include.h"

#include "Components/SkeletalMeshComponent.h"

#include "Engine/SkeletalMesh.h"
#include "Engine/AssetManager.h"

// Sets default values.
A_base_unit::A_base_unit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	m_ui_uniq_index = 0;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("TestMesh"));
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(GetRootComponent());
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->bGenerateOverlapEvents = false;
		Mesh->SetCanEverAffectNavigation(false);
	}

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
}

// Called when the game starts or when spawned
void A_base_unit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void A_base_unit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void A_base_unit::_initialize(uint32 _uiUniqIndex)
{
	Super::_initialize(_uiUniqIndex);

	CharacterSKIndex = FMath::RandRange(0, _Assets.Num() - 1);
	get_gamecore()->load_resource(USkeletalMesh::StaticClass(),
		eResourceLoadType,
		_Assets[CharacterSKIndex],
		delegate_resource_load_complete::CreateUObject(this, &A_base_unit::LoadComplite),
		delegate_resource_load_fail::CreateUObject(this, &A_base_unit::LoadFail));
}

void A_base_unit::_reset()
{
	m_ui_uniq_index = GC_INDEX_NONE;
}

void A_base_unit::LoadComplite(const FStringAssetReference& _AssetRef, UClass* _p_class)
{
/*	if (get_gamecore()->is_pooled_actor(this))
	{

	}*/

	TAssetPtr<USkeletalMesh> NewCharacter(_AssetRef);
	if (NewCharacter)
	{
		Mesh->SetSkeletalMesh(NewCharacter.Get());
	}
}

void A_base_unit::LoadFail(const FStringAssetReference& _AssetRef, UClass* _p_class)
{

}