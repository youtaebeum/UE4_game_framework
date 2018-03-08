// Fill out your copyright notice in the Description page of Project Settings.

#include "unit_anim_instance.h"

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
	
}

void U_unit_anim_instance::set_velocity(FVector _velocity)
{
	m_v_velocity = _velocity;
}
