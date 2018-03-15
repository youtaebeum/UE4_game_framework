
#include "game_instance.h"
#include "gamecore_include.h"

#include "scene/login_scene.h"
#include "scene/battle_scene.h"

void Ugame_instance::Init()
{
	Super::Init();

	// Register delegate for ticker callback
	m_tick_delegate_handle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &Ugame_instance::tick));

	//----------------------------------------------------------------------------------------------------------------------//
	if (U_gamecore_manager::_has_Instance() == false) U_gamecore_manager::_make_instance();

	U_gamecore_manager::_get_instance()->set_game_instance(this);
	U_gamecore_manager::_get_instance()->_initialize();	

	gGameCore->regist_scene_state<U_login_scene>((uint8)E_game_scene_type::login, TEXT("login"));
	gGameCore->regist_scene_state<U_battle_scene>((uint8)E_game_scene_type::battle, TEXT("battle"));
	
	//gGameCore->change_scene(E_game_scene_type::login);
	gGameCore->change_scene("login");
}

void Ugame_instance::Shutdown()
{
	U_gamecore_manager::_remove_instance();

	FTicker::GetCoreTicker().RemoveTicker(m_tick_delegate_handle);

	Super::Shutdown();
}

bool Ugame_instance::tick(float _f_delta_seconds)
{
	if (U_gamecore_manager::_get_instance() != nullptr)
		U_gamecore_manager::_get_instance()->_tick(_f_delta_seconds);

	return true;
}
