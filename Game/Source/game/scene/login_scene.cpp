// Fill out your copyright notice in the Description page of Project Settings.

#include "login_scene.h"
#include "gamecore_include.h"

void U_login_scene::enter()
{
	gGameCore->bind_input_action_event(E_action_event_type::test_event_0, delegate_action_event::FDelegate::CreateUObject(this, &U_login_scene::input_event_test));
}

void U_login_scene::update(float _f_delta_time)
{

}

void U_login_scene::exit()
{
	gGameCore->clear_input_event();
}

void U_login_scene::input_event_test()
{
	gGameCore->change_scene(TEXT("battle"));
}