// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "base_scene.h"
#include "utility/fsm_actor.h"
#include "scene_manager.generated.h"

UCLASS()
class GAMECORE_API U_scene_manager : public UObject, public F_singleton<U_scene_manager>
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

private:
	A_fsm_actor*			   m_p_state_machine_actor = nullptr;
	TMap<uint8, U_base_scene*> m_scene_contaner;

public:
	template<typename T> void regist_scene_state(uint8 _i_index, const FName& _name);
	void change_scene(uint8 _ui_index);
	void change_scene(const FName& _name);

	int8 get_current_scene_id(); 
	FName get_current_scene_name();
};

template<typename T> 
void U_scene_manager::regist_scene_state(uint8 _i_index, const FName& _name)
{
	GC_CHECK(GC_UTILTY::safe_map_value(m_scene_contaner.Find(_i_index)) == nullptr)

	T* p_scene = GC_NewObject<T>(this);
	GC_CHECK(p_scene->IsA(U_base_scene::StaticClass()) == true)
	
	p_scene->AddToRoot();
	p_scene->init(_i_index, _name);
	m_p_state_machine_actor->get_state_machine()->AddState(_i_index, _name);
	m_p_state_machine_actor->get_state_machine()->GetState(_i_index)->OnBeginState.AddDynamic(p_scene, &T::on_begin_state);
	m_p_state_machine_actor->get_state_machine()->GetState(_i_index)->OnUpdateState.AddDynamic(p_scene, &T::on_update_state);
	m_p_state_machine_actor->get_state_machine()->GetState(_i_index)->OnExitState.AddDynamic(p_scene, &T::on_exit_state);
	m_scene_contaner.Add(_i_index, p_scene);
}