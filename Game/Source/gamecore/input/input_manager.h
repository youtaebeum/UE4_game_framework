// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "input_manager.generated.h"

UCLASS()
class GAMECORE_API U_input_manager : public UObject, public F_singleton<U_input_manager>
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

private:
	class  A_controller_pawn* m_p_controller_pawn        = nullptr;
	uint32					  m_ui_controll_unit_index   = GC_INDEX_NONE;
	class  A_base_unit*		  m_p_controll_unit_instance = nullptr;

public:
	void   reset();
	void   set_controll_unit(int32 _ui_unit_Index);
	uint32 get_controll_unit_index();

	class A_controller_pawn* get_controller_pawn();
	APlayerController*       get_player_controller();

	FVector	get_move_axis(float _f_sampling_time = CONTROLLER_PAWN_AXIS_SAMPLING_INTERVAL);
	void bind_axis_event(const FName& _event_name, delegate_axis_event::FDelegate _delegator);
	void bind_action_event(E_action_event_type _e_type, delegate_action_event::FDelegate _delegator);
	void clear_event();

	void set_virtual_joystick_visiblity(bool _b_visible);
	void enable_input(bool _b_enable);
};
