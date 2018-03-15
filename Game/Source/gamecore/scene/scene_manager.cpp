#include "scene_manager.h"

#include "gamecore_include.h"
#include "gamecore_manager.h"

#include "utility/fsm_actor.h"
#include "base_scene.h"

void U_scene_manager::_initialize()
{
	m_p_state_machine_actor = Cast<A_fsm_actor>(GC_UTILTY::SpawnActor(A_fsm_actor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, "scene_state_actor", true));

	GC_UTILTY::DetachAllChildActors(m_p_state_machine_actor);
	GC_UTILTY::DetachParentActor(m_p_state_machine_actor);
	m_p_state_machine_actor->RegisterAllComponents();
	m_p_state_machine_actor->AddToRoot();
	m_p_state_machine_actor->bHidden = false;

	m_p_state_machine_actor->get_state_machine()->InitializeFSM();
};

void U_scene_manager::_reset()
{
	if (m_p_state_machine_actor) 
	{
		m_p_state_machine_actor->RemoveFromRoot();
		GC_UTILTY::DetachParentActor(m_p_state_machine_actor);
	}
	m_p_state_machine_actor = nullptr;

	for (auto& elem : m_scene_contaner) {
		GC_DeleteObject(elem.Value);
	}

	m_scene_contaner.Empty();
};

void U_scene_manager::_tick(float _f_delta_seconds)
{
};

void U_scene_manager::change_scene(uint8 _ui_index) {
	m_p_state_machine_actor->get_state_machine()->SetState(_ui_index);
};

void U_scene_manager::change_scene(const FName& _name) {
	m_p_state_machine_actor->get_state_machine()->SetState(_name);
};

int8 U_scene_manager::get_current_scene_id() {
	return m_p_state_machine_actor->get_state_machine()->GetCurrentStateID();
};

FName U_scene_manager::get_current_scene_name() {
	return m_p_state_machine_actor->get_state_machine()->GetCurrentStateName();
};