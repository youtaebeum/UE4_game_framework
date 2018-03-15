// Fill out your copyright notice in the Description page of Project Settings.

#include "base_scene.h"
#include "gamecore_include.h"
#include "UFSM.h"

void U_base_scene::init(uint8 _i_index, const FName& _name)
{
	m_i_state_index = _i_index;
	m_state_name = _name;
}

void U_base_scene::on_begin_state() {
	enter();
}

void U_base_scene::on_update_state() {
	update(gGameCore->get_world_delta_time());
}

void U_base_scene::on_exit_state() {
	exit();
}