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
	//m_p_root_scene_componenet = CreateDefaultSubobject<USceneComponent>(scene_component_name);
	//m_p_root_scene_componenet->Mobility = EComponentMobility::Movable;
	//RootComponent = m_p_root_scene_componenet;

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

	RootComponent = m_p_capsule_componenet;

	//////////////////////////////////////////////////////////////////////////
	static FName mesh_componenet_name(TEXT("master_mesh"));
	static FName mesh_collision_profile_name(TEXT("master_mesh"));
	m_p_master_mesh_componenet = CreateDefaultSubobject<USkeletalMeshComponent>(mesh_componenet_name);
	m_p_master_mesh_componenet->AlwaysLoadOnClient = true;
	m_p_master_mesh_componenet->AlwaysLoadOnServer = true;
	m_p_master_mesh_componenet->bOwnerNoSee = false;
	m_p_master_mesh_componenet->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	m_p_master_mesh_componenet->bCastDynamicShadow = true;
	m_p_master_mesh_componenet->bAffectDynamicIndirectLighting = true;
	m_p_master_mesh_componenet->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	m_p_master_mesh_componenet->SetupAttachment(RootComponent);
	m_p_master_mesh_componenet->SetCollisionProfileName(mesh_collision_profile_name);
	m_p_master_mesh_componenet->bGenerateOverlapEvents = false;
	m_p_master_mesh_componenet->SetCanEverAffectNavigation(false);
	m_p_master_mesh_componenet->SetRelativeRotation(FRotator(0, -90.0f, 0));

	// 확인사항 : AlwaysTickPoseAndRefreshBones <- 일경우 안그려질때의 의미가 단순 visible이 껴져있을때를 말하는건지 확인해야함
	m_p_master_mesh_componenet->SetVisibility(false);   

	//////////////////////////////////////////////////////////////////////////
	static FName movement_component_name(TEXT("movement"));
	m_p_movement_component = CreateDefaultSubobject<U_unit_movement_component>(movement_component_name);
	m_p_movement_component->SetUpdatedComponent(RootComponent);

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

	if (m_p_master_mesh_componenet) {
		U_unit_anim_instance* anim_instance = Cast<U_unit_anim_instance>(m_p_master_mesh_componenet->GetAnimInstance());
		if (anim_instance != nullptr) {
			anim_instance->_tick(DeltaTime);
		}
	}

	for (auto iter : m_map_child_mesh)
	{
		U_unit_anim_instance* anim_instance = Cast<U_unit_anim_instance>(iter.Value->GetAnimInstance());
		if (anim_instance != nullptr) {
			anim_instance->_tick(DeltaTime);
		}
	}
}

void A_base_unit::_initialize(uint32 _uiUniqIndex)
{
	PrimaryActorTick.SetTickFunctionEnable(true);

	m_ui_uniq_index = _uiUniqIndex;

	//m_p_root_scene_componenet->Activate();
	m_p_capsule_componenet->Activate();
	m_p_master_mesh_componenet->Activate();
	m_p_movement_component->Activate();

	for (auto iter : m_map_child_mesh) 
	{
		iter.Value->Activate();

		U_unit_anim_instance* anim_instance = Cast<U_unit_anim_instance>(iter.Value->GetAnimInstance());
		if (anim_instance != nullptr) {
			anim_instance->_initialize();
		}
	}
}

void A_base_unit::_reset()
{
	PrimaryActorTick.SetTickFunctionEnable(false);

	m_ui_uniq_index = GC_INDEX_NONE;
	
	//m_p_root_scene_componenet->Deactivate();
	m_p_capsule_componenet->Deactivate();
	m_p_master_mesh_componenet->Deactivate();
	m_p_movement_component->Deactivate();

	m_p_master_mesh_componenet->SetAnimInstanceClass(nullptr);
	m_p_master_mesh_componenet->SetSkeletalMesh(nullptr);

	for (auto iter : m_map_child_mesh) {
		iter.Value->Deactivate();

		iter.Value->SetAnimInstanceClass(nullptr);
		iter.Value->SetSkeletalMesh(nullptr);
		iter.Value->SetMasterPoseComponent(nullptr);
	}

	for (auto iter : m_map_child_mesh)
	{
		U_unit_anim_instance* anim_instance = Cast<U_unit_anim_instance>(iter.Value->GetAnimInstance());
		if (anim_instance != nullptr) {
			anim_instance->_reset();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------
void A_base_unit::set_anim_instance(const FString& _str_path)
{
	F_load_resource_desc load_desc;

	load_desc._p_class = UClass::StaticClass();
	load_desc._e_loading_type = e_rsource_loading_type::async;
	load_desc._str_path = _str_path;
	load_desc._i_priority = 100;

	gGameCore->load_resource(load_desc, 
		delegate_resource_load_complete::CreateUObject(this, &A_base_unit::load_complite_anim_instance),
		delegate_resource_load_fail::CreateUObject(this, &A_base_unit::load_fail_anim_instance));
}

void A_base_unit::load_complite_anim_instance(const FStringAssetReference& _AssetRef, UClass* _p_class, int32 _i_custom_index)
{
	if (m_ui_uniq_index == GC_INDEX_NONE) return;

	TAssetPtr<UClass> ptr_instance(_AssetRef);
	if (ptr_instance) {
		TArray<UActorComponent*> arr_mesh = GetComponentsByClass(USkeletalMeshComponent::StaticClass());
		for (int i = 0; i < arr_mesh.Num(); ++i)
		{
			USkeletalMeshComponent* p_mesh = Cast<USkeletalMeshComponent>(arr_mesh[i]);
			if (p_mesh->GetAttachParent() != m_p_master_mesh_componenet)
			{
				p_mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				p_mesh->SetAnimInstanceClass(ptr_instance.Get());
			}
		}
	}
	else {
		GC_WARNING(TEXT("[A_base_unit::load_complite_anim_instance] %s"), *_AssetRef.GetAssetPathString());
	}
}

void A_base_unit::load_fail_anim_instance(const FStringAssetReference& _AssetRef, UClass* _p_class, int32 _i_custom_index)
{
	GC_WARNING(TEXT("[A_base_unit::load_fail_anim_instance] %s"), *_AssetRef.GetAssetPathString());
}

//--------------------------------------------------------------------------------------------------------------------------------

void A_base_unit::add_mesh_componenet(int32 _ui_index, USkeletalMeshComponent* _p_mesh_componenet, bool _b_attach_master)
{
	USkeletalMeshComponent* p_child_mesh = GC_UTILTY::safe_map_value(m_map_child_mesh.Find(_ui_index));
	if (p_child_mesh == nullptr) {
		m_map_child_mesh.Add(_ui_index, _p_mesh_componenet);
	}

	p_child_mesh = GC_UTILTY::safe_map_value(m_map_child_mesh.Find(_ui_index));

	p_child_mesh->AlwaysLoadOnClient = true;
	p_child_mesh->AlwaysLoadOnServer = true;
	p_child_mesh->bOwnerNoSee = false;
	p_child_mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
	p_child_mesh->bCastDynamicShadow = true;
	p_child_mesh->bAffectDynamicIndirectLighting = true;
	p_child_mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	static FName mesh_collision_profile_name(TEXT("unit_mesh"));
	p_child_mesh->SetCollisionProfileName(mesh_collision_profile_name);
	p_child_mesh->bGenerateOverlapEvents = false;
	p_child_mesh->SetCanEverAffectNavigation(false);

	if (_b_attach_master == true) {
		p_child_mesh->SetupAttachment(m_p_master_mesh_componenet);
	}
	else
	{
		p_child_mesh->SetupAttachment(RootComponent);
		p_child_mesh->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}
}

void A_base_unit::change_mesh(int32 _ui_index, const FString& _str_path, int32 _i_priority)
{
	USkeletalMeshComponent* p_child_mesh = GC_UTILTY::safe_map_value(m_map_child_mesh.Find(_ui_index));
	if (p_child_mesh == nullptr ) {
		GC_WARNING(TEXT("[A_base_unit::change_mesh] invalid mesh componenet (type:%d ,path:%s"), _ui_index, *_str_path);
	}

	F_load_resource_desc load_desc;

	load_desc._p_class = USkeletalMesh::StaticClass();
	load_desc._e_loading_type = e_rsource_loading_type::async;
	load_desc._str_path = _str_path;
	load_desc._i_priority = _i_priority;
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
		if (p_child_mesh) 
		{
			p_child_mesh->SetSkeletalMesh(ptr_instance.Get());

			// set master pos
			if (p_child_mesh->GetAttachParent() == m_p_master_mesh_componenet) 
			{
				if (m_p_master_mesh_componenet->SkeletalMesh == nullptr) {
					m_p_master_mesh_componenet->SetSkeletalMesh(ptr_instance.Get());
				}
				
				p_child_mesh->SetMasterPoseComponent(m_p_master_mesh_componenet);
			}
		}		
	}
	else {
		GC_WARNING(TEXT("[A_base_unit::load_complite_mesh] %s"), *_AssetRef.GetAssetPathString());
	}
}

void A_base_unit::load_fail_mesh(const FStringAssetReference& _AssetRef, UClass* _p_class, int32 _i_custom_index)
{
	GC_WARNING(TEXT("[A_base_unit::load_fail_mesh] %s"), *_AssetRef.GetAssetPathString());
}
