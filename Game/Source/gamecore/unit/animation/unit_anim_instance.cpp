// Fill out your copyright notice in the Description page of Project Settings.

#include "unit_anim_instance.h"
#include "unit/base_unit.h"
#include "unit/components/unit_movement_component.h"

void U_unit_anim_instance::_initialize()
{
	m_v_velocity = FVector::ZeroVector;
}

void U_unit_anim_instance::_reset()
{
	m_v_velocity = FVector::ZeroVector;
}

void U_unit_anim_instance::_tick(float _f_delta_time)
{
	if (GetOwningActor()->IsA(A_base_unit::StaticClass()))
	{
		U_unit_movement_component* p_movement = Cast<U_unit_movement_component>(GetOwningActor()->GetComponentByClass(U_unit_movement_component::StaticClass()));
		if (p_movement) {
			m_v_velocity = p_movement->Velocity;
		}
	}
}

void U_unit_anim_instance::set_velocity(FVector _velocity)
{
	m_v_velocity = _velocity;
}
