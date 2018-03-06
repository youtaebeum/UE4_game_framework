// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "GameCore_Manager.generated.h"

UCLASS()
class GAMECORE_API U_gamecore_manager : public UObject, public F_singleton<U_gamecore_manager>
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

#pragma region GameInstance
private: 
	class UGameInstance* m_p_game_instance = nullptr;
public: 
	void set_game_instance(class UGameInstance* _p_game_instance) { m_p_game_instance = _p_game_instance; }
	class UGameInstance* get_game_instance() { return m_p_game_instance; }
#pragma endregion

#pragma region ObjectPool
public:
	template<typename T> T* get_object();
	template<typename T> T* get_actor();
	template<typename T> T* get_uobject();

	template<typename T> void return_object(T* _p_ptr);
	void return_actor(class A_base_actor* _p_actor);
	void return_uobject(UObject* _p_uobject);

	template<typename T> bool is_pooled_object(T* _p_ptr);
	bool is_pooled_actor(A_base_actor* _p_actor);
	bool is_pooled_uobject(UObject* _p_uobject);

#pragma endregion

#pragma region ResourceManager
	void load_resource(UClass* _p_class, e_rsource_loading_type _e_type, const FString& _str_path, delegate_resource_load_complete _delegate_load_complete, delegate_resource_load_fail _delegate_load_fail);
#pragma endregion

#pragma region UnitManager
	template<typename T> uint32 spawn_unit(const F_spawn_unit_desc& _desc = F_spawn_unit_desc::get());
	void					    despawn_unit(uint32 _i_uniq_index);
	template<typename T> T*	    get_unit(uint32 _i_uniq_index);
#pragma endregion

};

static U_gamecore_manager* get_gamecore() { return U_gamecore_manager::_get_instance(); }

//------------------------------------------------------------------------------------------------

template<typename T>
uint32 U_gamecore_manager::spawn_unit(const F_spawn_unit_desc& _desc) {
	return U_unit_manager::_get_instance()->spawn_unit<T>(_desc);
}

template<typename T>
T* U_gamecore_manager::get_unit(uint32 _i_uniq_index) {
	return Cast<T>(U_unit_manager::_get_instance()->get_unit(_i_uniq_index));
}

//------------------------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------------------------