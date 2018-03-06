// Fill out your copyright notice in the Description page of Project Settings.

#include "gamecore_manager.h"
#include "gamecore_include.h"

#include "Engine/World.h"

void U_gamecore_manager::_initialize()
{
	//////////////////////////////////////////////////////////////////////////
	U_object_pool_manager::_make_instance();
	U_resource_manager::_make_instance();
	U_unit_manager::_make_instance();

	//////////////////////////////////////////////////////////////////////////

	U_object_pool_manager::_get_instance()->_initialize();	U_object_pool_manager::_get_instance()->_spawn_manager_actor<A_object_pool_actor>(m_p_game_instance->GetWorld());
	U_resource_manager::_get_instance()->_initialize();
	U_unit_manager::_get_instance()->_initialize();		U_unit_manager::_get_instance()->_spawn_manager_actor<AActor>(m_p_game_instance->GetWorld());
}

void U_gamecore_manager::_reset()
{
	U_unit_manager::_remove_instance();
	U_resource_manager::_remove_instance();
	U_object_pool_manager::_remove_instance();

	m_p_game_instance = nullptr;
}

void U_gamecore_manager::_tick(float _f_delta_seconds)
{
	U_object_pool_manager::_get_instance()->_tick(_f_delta_seconds);
	U_resource_manager::_get_instance()->_tick(_f_delta_seconds);
	U_unit_manager::_get_instance()->_tick(_f_delta_seconds);
}

void U_gamecore_manager::load_resource(UClass* _p_class, e_rsource_loading_type _e_type, const FString& _str_path, delegate_resource_load_complete _delegate_load_complete, delegate_resource_load_fail _delegate_load_fail)
{
	U_resource_manager::_get_instance()->load_resource(_p_class, _e_type, _str_path, _delegate_load_complete, _delegate_load_fail);
}

void  U_gamecore_manager::despawn_unit(uint32 _i_uniq_index) 
{
	U_unit_manager::_get_instance()->despawn_unit(_i_uniq_index);
}

void U_gamecore_manager::return_actor(A_base_actor* _p_actor) {
	U_object_pool_manager::_get_instance()->return_actor(_p_actor);
}

void U_gamecore_manager::return_uobject(UObject* _p_uobject) {
	U_object_pool_manager::_get_instance()->return_uobject(_p_uobject);
}

bool U_gamecore_manager::is_pooled_actor(A_base_actor* _p_actor) {
	return U_object_pool_manager::_get_instance()->is_pooled_actor(_p_actor);
}

bool U_gamecore_manager::is_pooled_uobject(UObject* _p_uobject) {
	return U_object_pool_manager::_get_instance()->is_pooled_uobject(_p_uobject);
}



