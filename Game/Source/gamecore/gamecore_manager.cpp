// Fill out your copyright notice in the Description page of Project Settings.

#include "gamecore_manager.h"
#include "gamecore_include.h"

#include "pool/object_pool_manager.h"
#include "resource/resource_manager.h"
#include "unit/unit_manager.h"
#include "input/input_manager.h"

#include "Engine/World.h"

GAMECORE_API U_gamecore_manager*	gGameCore = NULL;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void U_gamecore_manager::_initialize()
{
	gGameCore = _get_instance();

	{
		U_object_pool_manager::_make_instance();
		U_resource_manager::_make_instance();
		U_unit_manager::_make_instance();
		U_input_manager::_make_instance();
	}
	
	{
		U_object_pool_manager::_get_instance()->_initialize();	
		U_resource_manager::_get_instance()->_initialize();
		U_unit_manager::_get_instance()->_initialize();			
		U_input_manager::_get_instance()->_initialize();		
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void U_gamecore_manager::_reset()
{
	U_input_manager::_remove_instance();
	U_unit_manager::_remove_instance();
	U_resource_manager::_remove_instance();
	U_object_pool_manager::_remove_instance();

	m_p_game_instance = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void U_gamecore_manager::_tick(float _f_delta_seconds)
{
	U_object_pool_manager::_get_instance()->_tick(_f_delta_seconds);
	U_resource_manager::_get_instance()->_tick(_f_delta_seconds);
	U_unit_manager::_get_instance()->_tick(_f_delta_seconds);
	U_input_manager::_get_instance()->_tick(_f_delta_seconds);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void U_gamecore_manager::set_game_instance(class UGameInstance* _p_game_instance) 
{ 
	m_p_game_instance = _p_game_instance; 
}

UGameInstance* U_gamecore_manager::get_game_instance() 
{ 
	return m_p_game_instance; 
}

float U_gamecore_manager::get_world_delta_time()
{
	return m_p_game_instance->GetWorld()->GetDeltaSeconds();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void U_gamecore_manager::load_resource(UClass* _p_class, e_rsource_loading_type _e_type, const FString& _str_path, delegate_resource_load_complete _delegate_load_complete, delegate_resource_load_fail _delegate_load_fail)
{
	U_resource_manager::_get_instance()->load_resource(_p_class, _e_type, _str_path, _delegate_load_complete, _delegate_load_fail);
}

void  U_gamecore_manager::despawn_unit(uint32 _i_uniq_index) 
{
	U_unit_manager::_get_instance()->despawn_unit(_i_uniq_index);
}

class A_base_unit* U_gamecore_manager::get_unit(uint32 _i_uniq_index)
{
	return U_unit_manager::_get_instance()->get_unit(_i_uniq_index);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void U_gamecore_manager::return_actor(AActor* _p_actor) {
	U_object_pool_manager::_get_instance()->return_actor(_p_actor);
}

void U_gamecore_manager::return_uobject(UObject* _p_uobject) {
	U_object_pool_manager::_get_instance()->return_uobject(_p_uobject);
}

bool U_gamecore_manager::is_pooled_actor(AActor* _p_actor) {
	return U_object_pool_manager::_get_instance()->is_pooled_actor(_p_actor);
}

bool U_gamecore_manager::is_pooled_uobject(UObject* _p_uobject) {
	return U_object_pool_manager::_get_instance()->is_pooled_uobject(_p_uobject);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void   U_gamecore_manager::set_controll_unit(int32 _ui_unit_Index) {
	U_input_manager::_get_instance()->set_controll_unit(_ui_unit_Index);
}

uint32 U_gamecore_manager::get_controll_unit_index() {
	return U_input_manager::_get_instance()->get_controll_unit_index();
}

void   U_gamecore_manager::reset_controll_unit() {
	U_input_manager::_get_instance()->reset();
}

A_controller_pawn* U_gamecore_manager::get_controller_pawn() {
	return U_input_manager::_get_instance()->get_controller_pawn();
}

APlayerController* U_gamecore_manager::get_player_controller() {
	return U_input_manager::_get_instance()->get_player_controller();
}

FVector	U_gamecore_manager::get_intput_move_axis(float _f_sampling_time)
{
	return U_input_manager::_get_instance()->get_move_axis(_f_sampling_time);
}

void    U_gamecore_manager::bind_input_axis_event(const FName& _event_name, delegate_axis_event::FDelegate _delegate)
{
	U_input_manager::_get_instance()->bind_axis_event(_event_name, _delegate);
}

void    U_gamecore_manager::bind_input_action_event(E_action_event_type _e_type, delegate_action_event::FDelegate _delegate)
{
	U_input_manager::_get_instance()->bind_action_event(_e_type, _delegate);
}

void    U_gamecore_manager::clear_input_event()
{
	U_input_manager::_get_instance()->clear_event();
}

void	U_gamecore_manager::set_virtual_joystick_visiblity(bool _b_visible)
{
	U_input_manager::_get_instance()->set_virtual_joystick_visiblity(_b_visible);
}

void	U_gamecore_manager::enable_controller_input(bool _b_enable)
{
	U_input_manager::_get_instance()->enable_input(_b_enable);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------