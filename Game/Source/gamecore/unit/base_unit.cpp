// Fill out your copyright notice in the Description page of Project Settings.

#include "base_unit.h"
#include "gamecore_include.h"
#include "gamecore_manager.h"

#include "components/unit_movement_component.h"
#include "animation/unit_anim_instance.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Engine/SkeletalMesh.h"
#include "Engine/AssetManager.h"
#include "Engine/CollisionProfile.h"


// Sets default values.
A_base_unit::A_base_unit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	//////////////////////////////////////////////////////////////////////////
	static FName scene_component_name(TEXT("root_scene"));
	m_p_root_scene_componenet = CreateDefaultSubobject<USceneComponent>(scene_component_name);
	m_p_root_scene_componenet->Mobility = EComponentMobility::Movable;
	RootComponent = m_p_root_scene_componenet;

	//////////////////////////////////////////////////////////////////////////
	static FName capsule_component_name(TEXT("root_collision"));
	m_p_capsule_componenet = CreateDefaultSubobject<UCapsuleComponent>(capsule_component_name);
	m_p_capsule_componenet->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	m_p_capsule_componenet->CanCharacterStepUpOn = ECB_No;
	m_p_capsule_componenet->bShouldUpdatePhysicsVolume = true;
	m_p_capsule_componenet->bCheckAsyncSceneOnMove = false;
	m_p_capsule_componenet->SetCanEverAffectNavigation(false);
	m_p_capsule_componenet->SetupAttachment(RootComponent);
	m_p_capsule_componenet->bDynamicObstacle = true;

	m_p_capsule_componenet->SetHiddenInGame(false);

	//////////////////////////////////////////////////////////////////////////
	static FName mesh_componenet_name(TEXT("unit_mesh"));
	static FName mesh_collision_profile_name(TEXT("unit_mesh"));
	m_p_root_mesh_componenet = CreateDefaultSubobject<USkeletalMeshComponent>(mesh_componenet_name);
	m_p_root_mesh_componenet->AlwaysLoadOnClient = true;
	m_p_root_mesh_componenet->AlwaysLoadOnServer = true;
	m_p_root_mesh_componenet->bOwnerNoSee = false;
	m_p_root_mesh_componenet->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
	m_p_root_mesh_componenet->bCastDynamicShadow = true;
	m_p_root_mesh_componenet->bAffectDynamicIndirectLighting = true;
	m_p_root_mesh_componenet->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_p_root_mesh_componenet->SetupAttachment(RootComponent);
	m_p_root_mesh_componenet->SetCollisionProfileName(mesh_collision_profile_name);
	m_p_root_mesh_componenet->bGenerateOverlapEvents = false;
	m_p_root_mesh_componenet->SetCanEverAffectNavigation(false);
	m_p_root_mesh_componenet->SetRelativeRotation(FRotator(0, -90.0f, 0));

	//////////////////////////////////////////////////////////////////////////
	static FName movement_component_name(TEXT("movement"));
	m_p_movement_component = CreateDefaultSubobject<U_unit_movement_component>(movement_component_name);
	m_p_movement_component->SetUpdatedComponent(RootComponent);

	//////////////////////////////////////////////////////////////////////////
	m_p_anim_instance = Cast<U_unit_anim_instance>(m_p_root_mesh_componenet->GetAnimInstance());
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

	if (m_p_root_mesh_componenet) {
		if (m_p_anim_instance != m_p_root_mesh_componenet->GetAnimInstance()) {
			m_p_anim_instance = Cast<U_unit_anim_instance>(m_p_root_mesh_componenet->GetAnimInstance());
		}

		if (m_p_anim_instance) {
			m_p_anim_instance->_tick(DeltaTime);
			if (m_p_movement_component) {
				m_p_anim_instance->set_velocity(m_p_movement_component->Velocity);
			}
		}
	}
}

void A_base_unit::_initialize(uint32 _uiUniqIndex)
{
	PrimaryActorTick.SetTickFunctionEnable(true);

	m_ui_uniq_index = _uiUniqIndex;

	m_p_root_scene_componenet->Activate();
	m_p_capsule_componenet->Activate();
	m_p_root_mesh_componenet->Activate();
	m_p_movement_component->Activate();

	if (m_p_anim_instance) {
		m_p_anim_instance->_initialize();
	}

	GC_CHECK(m_map_child_mesh.Num() == 0)
	m_map_child_mesh.Empty();
}

void A_base_unit::_reset()
{
	PrimaryActorTick.SetTickFunctionEnable(false);

	m_ui_uniq_index = GC_INDEX_NONE;

	for (auto iter : m_map_child_mesh) {
		if (m_p_root_mesh_componenet != iter.Value)	{
			GC_DeleteObject(iter.Value);
		}
	}

	m_map_child_mesh.Empty();

	m_p_root_scene_componenet->Deactivate();
	m_p_capsule_componenet->Deactivate();
	m_p_root_mesh_componenet->Deactivate();
	m_p_movement_component->Deactivate();

	m_p_root_mesh_componenet->SetAnimInstanceClass(nullptr);
	m_p_root_mesh_componenet->SetSkeletalMesh(nullptr);

	if (m_p_anim_instance) {
		m_p_anim_instance->_reset();
	}

	m_p_anim_instance = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------
void A_base_unit::set_anim_instance(const FString& _str_path)
{
	F_load_resource_desc load_desc;

	load_desc._p_class = UClass::StaticClass();
	load_desc._e_loading_type = e_rsource_loading_type::async;
	load_desc._str_path = _str_path;
	load_desc._i_property = 100;

	gGameCore->load_resource(load_desc, 
		delegate_resource_load_complete::CreateUObject(this, &A_base_unit::load_complite_anim_instance),
		delegate_resource_load_fail::CreateUObject(this, &A_base_unit::load_fail_anim_instance));
}

void A_base_unit::load_complite_anim_instance(const FStringAssetReference& _AssetRef, UClass* _p_class, int32 _i_custom_index)
{
	if (m_ui_uniq_index == GC_INDEX_NONE) return;

	TAssetPtr<UClass> ptr_instance(_AssetRef);
	if (ptr_instance) {
		m_p_root_mesh_componenet->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		m_p_root_mesh_componenet->SetAnimInstanceClass(ptr_instance.Get());
	} else {
		GC_WARNING(TEXT("[A_base_unit::load_complite_anim_instance] %s"), *_AssetRef.GetAssetPathString());
	}
}

void A_base_unit::load_fail_anim_instance(const FStringAssetReference& _AssetRef, UClass* _p_class, int32 _i_custom_index)
{
	GC_WARNING(TEXT("[A_base_unit::load_fail_anim_instance] %s"), *_AssetRef.GetAssetPathString());
}

//--------------------------------------------------------------------------------------------------------------------------------

void A_base_unit::change_mesh(int32 _ui_index, const FString& _str_path, int32 _i_property)
{
	F_load_resource_desc load_desc;

	load_desc._p_class = USkeletalMesh::StaticClass();
	load_desc._e_loading_type = e_rsource_loading_type::async;
	load_desc._str_path = _str_path;
	load_desc._i_property = _i_property;
	load_desc._i_custom_index = _ui_index;
	load_desc._v_loaded_location = GetActorLocation();

	gGameCore->load_resource(load_desc,
		delegate_resource_load_complete::CreateUObject(this, &A_base_unit::load_complite_mesh),
		delegate_resource_load_fail::CreateUObject(this, &A_base_unit::load_fail_mesh), true);
}

void A_base_unit::load_complite_mesh(const FStringAssetReference& _AssetRef, UClass* _p_class, int32 _i_custom_index)
{
	if (m_ui_uniq_index == GC_INDEX_NONE) return;

	TAssetPtr<USkeletalMesh> ptr_instance(_AssetRef);
	if (ptr_instance)
	{
		USkeletalMeshComponent* p_child_mesh = GC_UTILTY::safe_map_value(m_map_child_mesh.Find(_i_custom_index));
		if (p_child_mesh == nullptr)
		{
			if (m_map_child_mesh.Num() == 0) {
				p_child_mesh = m_p_root_mesh_componenet;
			}
			else {
				p_child_mesh = GC_NewObject<USkeletalMeshComponent>(this);
			}
			m_map_child_mesh.Add(_i_custom_index, p_child_mesh);
		}

		if (p_child_mesh != m_p_root_mesh_componenet)
		{
			p_child_mesh->SetupAttachment(m_p_root_mesh_componenet);
			p_child_mesh->SetMasterPoseComponent(m_p_root_mesh_componenet);
			p_child_mesh->RegisterComponent();
		}
		
		p_child_mesh->SetSkeletalMesh(ptr_instance.Get());
	}
	else {
		GC_WARNING(TEXT("[A_base_unit::load_complite_mesh] %s"), *_AssetRef.GetAssetPathString());
	}
}

void A_base_unit::load_fail_mesh(const FStringAssetReference& _AssetRef, UClass* _p_class, int32 _i_custom_index)
{
	GC_WARNING(TEXT("[A_base_unit::load_fail_mesh] %s"), *_AssetRef.GetAssetPathString());
}
