// Fill out your copyright notice in the Description page of Project Settings.

#include "input_manager.h"

#include "gamecore_include.h"
#include "gamecore_manager.h"

#include "controller_pawn.h"

#include "GameFramework/PlayerController.h"

void U_input_manager::_initialize()
{
	_pInstance->m_p_controller_pawn = Cast<A_controller_pawn>(GC_UTILTY::SpawnActor(A_controller_pawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, TEXT("ControllerPawn"), true));

#if WITH_EDITOR
	U_input_manager::_get_instance()->_spawn_manager_actor<AActor>(gGameCore->get_game_instance()->GetWorld());
	if (_pInstance->_get_manager_actor()) {
		_pInstance->_AttachToManagerActor(_pInstance->m_p_controller_pawn);
	}
	GC_CHECK(_pInstance->_get_manager_actor() != nullptr);
#endif

	_pInstance->m_ui_controll_unit_index   = GC_INDEX_NONE;
	_pInstance->m_p_controll_unit_instance = nullptr;
}

void U_input_manager::_reset()
{
	reset();
}

void U_input_manager::_tick(float _f_delta_seconds)
{
	if (_pInstance == nullptr)
		return;

	if (m_ui_controll_unit_index != GC_INDEX_NONE) 
	{
		A_base_unit* p_unit_instance = gGameCore->get_unit<A_base_unit>(m_ui_controll_unit_index);
		if (p_unit_instance == nullptr) {
			_pInstance->m_ui_controll_unit_index = GC_INDEX_NONE;
			_pInstance->m_p_controll_unit_instance = nullptr;
		}
	}
}

void   U_input_manager::reset()
{
	_pInstance->m_ui_controll_unit_index = GC_INDEX_NONE;
	_pInstance->m_p_controll_unit_instance = nullptr;

	if (_pInstance->m_p_controller_pawn != nullptr) {
		GC_UTILTY::DespawnActor(_pInstance->m_p_controller_pawn);
	}

	_pInstance->m_p_controller_pawn = nullptr;
}

void   U_input_manager::set_controll_unit(int32 _ui_unit_Index)
{
	A_base_unit* p_unit_instance = gGameCore->get_unit<A_base_unit>(_ui_unit_Index);
	if (p_unit_instance != nullptr) 
	{
		m_ui_controll_unit_index = _ui_unit_Index;
		m_p_controll_unit_instance = p_unit_instance;
	}
}

uint32 U_input_manager::get_controll_unit_index()
{
	return m_ui_controll_unit_index;
}

A_controller_pawn* U_input_manager::get_controller_pawn()
{
	return m_p_controller_pawn;
}

class APlayerController* get_player_controller();

APlayerController* U_input_manager::get_player_controller()
{
	if (m_p_controller_pawn == nullptr)
		return nullptr;

	return Cast<APlayerController>(m_p_controller_pawn->GetController());
}

FVector	U_input_manager::get_move_axis(float _f_sampling_time)
{
	if (m_p_controller_pawn == nullptr)
		return FVector::ZeroVector;

	return m_p_controller_pawn->get_move_axis(_f_sampling_time);
}

void U_input_manager::bind_axis_event(const FName& _event_name, delegate_axis_event::FDelegate _delegator)
{
	GC_CHECK(m_p_controller_pawn != nullptr);
	m_p_controller_pawn->bind_axis_event(_event_name, _delegator);
}

void U_input_manager::bind_action_event(E_action_event_type _e_type, delegate_action_event::FDelegate _delegator)
{
	GC_CHECK(m_p_controller_pawn != nullptr);
	m_p_controller_pawn->bind_action_event(_e_type, _delegator);
}

void U_input_manager::clear_event()
{
	GC_CHECK(m_p_controller_pawn != nullptr);
	m_p_controller_pawn->clear_event();
}

void U_input_manager::set_virtual_joystick_visiblity(bool _b_visible)
{
	APlayerController* p_controller = get_player_controller();
	GC_CHECK(p_controller != nullptr);
	p_controller->SetVirtualJoystickVisibility(_b_visible);
}

void U_input_manager::enable_input(bool _b_enable)
{
	APlayerController* p_controller = get_player_controller();
	GC_CHECK(p_controller != nullptr);
	GC_CHECK(m_p_controller_pawn != nullptr);

	if (_b_enable) {
		m_p_controller_pawn->EnableInput(p_controller);
	}
	else {
		m_p_controller_pawn->DisableInput(p_controller);
	}
}
