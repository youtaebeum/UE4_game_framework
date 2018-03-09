// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"

#include "pool/object_pool_manager.h"
#include "unit/unit_manager.h"

#include "GameCore_Manager.generated.h"

UCLASS()
class GAMECORE_API U_gamecore_manager : public UObject, public F_singleton<U_gamecore_manager>
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// GameInstance
private: 
	class UGameInstance* m_p_game_instance = nullptr;
public: 
	void set_game_instance(class UGameInstance* _p_game_instance);
	class UGameInstance* get_game_instance(); 
	float get_world_delta_time();
#pragma endregion

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Object Pool
public:
	template<typename T> T* get_object();
	template<typename T> T* get_actor();
	template<typename T> T* get_uobject();

	template<typename T> void return_object(T* _p_ptr);
	void return_actor(class AActor* _p_actor);
	void return_uobject(UObject* _p_uobject);

	template<typename T> bool is_pooled_object(T* _p_ptr);
	bool is_pooled_actor(AActor* _p_actor);
	bool is_pooled_uobject(UObject* _p_uobject);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Resource Manager
	void load_resource(const F_load_resource_desc& _desc,
		delegate_resource_load_complete _delegate_load_complete, 
		delegate_resource_load_fail _delegate_load_fail);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Scene Manager

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unit Manager
	template<typename T> uint32 spawn_unit(const F_spawn_unit_desc& _desc = F_spawn_unit_desc::get());
	void					    despawn_unit(uint32 _i_uniq_index);
	template<typename T> T*	    get_unit(uint32 _i_uniq_index);
	class A_base_unit*		    get_unit(uint32 _i_uniq_index);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// InputManager
	void   set_controll_unit(int32 _ui_unit_Index);
	uint32 get_controll_unit_index();
	void   reset_controll_unit();

	class A_controller_pawn* get_controller_pawn();
	class APlayerController* get_player_controller();

	FVector	get_intput_move_axis(float _f_sampling_time = CONTROLLER_PAWN_AXIS_SAMPLING_INTERVAL);
	void    bind_input_axis_event(const FName& _event_name, delegate_axis_event::FDelegate _delegate);
	void    bind_input_action_event(E_action_event_type _e_type, delegate_action_event::FDelegate _delegator);
	void    clear_input_event();

	void	set_virtual_joystick_visiblity(bool _b_visible);
	void	enable_controller_input(bool _b_enable);
};

extern GAMECORE_API class U_gamecore_manager*	gGameCore;

#pragma region templte function
template<typename T>
uint32 U_gamecore_manager::spawn_unit(const F_spawn_unit_desc& _desc) {
	return U_unit_manager::_get_instance()->spawn_unit<T>(_desc);
}

template<typename T>
T* U_gamecore_manager::get_unit(uint32 _i_uniq_index) {
	return Cast<T>(U_unit_manager::_get_instance()->get_unit(_i_uniq_index));
}

template<typename T> T* U_gamecore_manager::get_object() {
	return U_object_pool_manager::_get_instance()->get_object<T>();
}

template<typename T> T* U_gamecore_manager::get_actor() {
	return U_object_pool_manager::_get_instance()->get_actor<T>();
}

template<typename T> T* U_gamecore_manager::get_uobject() {
	return U_object_pool_manager::_get_instance()->get_uobject<T>();
}

template<typename T> void U_gamecore_manager::return_object(T* _p_ptr) {
	U_object_pool_manager::_get_instance()->return_object<T>(_p_ptr);
}

template<typename T> bool U_gamecore_manager::is_pooled_object(T* _p_ptr) {
	return U_object_pool_manager::_get_instance()->is_pooled_object<T>(_p_ptr);
}
#pragma endregion