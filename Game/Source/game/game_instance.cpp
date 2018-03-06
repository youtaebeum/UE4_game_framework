// Fill out your copyright notice in the Description page of Project Settings.

#include "game_instance.h"
#include "gamecore_include.h"

void Ugame_instance::Init()
{
	Super::Init();

	// Register delegate for ticker callback
	m_tick_delegate_handle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &Ugame_instance::tick));

	//----------------------------------------------------------------------------------------------------------------------//
	if (U_gamecore_manager::_has_Instance() == false) U_gamecore_manager::_make_instance();

	U_gamecore_manager::_get_instance()->set_game_instance(this);
	U_gamecore_manager::_get_instance()->_initialize();	

	loaded_unit_list.Empty();
}

void Ugame_instance::Shutdown()
{
	//----------------------------------------------------------------------------------------------------------------------//

	loaded_unit_list.Empty();

	U_gamecore_manager::_remove_instance();

	FTicker::GetCoreTicker().RemoveTicker(m_tick_delegate_handle);

	Super::Shutdown();
}

bool Ugame_instance::tick(float _f_delta_seconds)
{
	//----------------------------------------------------------------------------------------------------------------------//
	if (U_gamecore_manager::_get_instance() != nullptr)
		U_gamecore_manager::_get_instance()->_tick(_f_delta_seconds);

	static bool b = false;
	uint32 iIndex = 0;
	if (b == true)
	{
		int i = 0;
		F_spawn_unit_desc desc;
		desc._e_load_type = e_rsource_loading_type::instantly;
		for (uint32 ui = 0; ui < 500; ++ui)
		{
			desc._v_location.X = FMath::RandRange(-5000.0f, 5000.0f);
			desc._v_location.Y = FMath::RandRange(-5000.0f, 5000.0f);
			iIndex = get_gamecore()->spawn_unit<A_base_unit>(desc);
			loaded_unit_list.Add(iIndex);
		}

		desc._v_location.Set(0.0f, 0.0f, 1000);
		desc._e_load_type = e_rsource_loading_type::async;
		for (uint32 ui = 0; ui < 500; ++ui)
		{
			desc._v_location.X = FMath::RandRange(-5000.0f, 5000.0f);
			desc._v_location.Y = FMath::RandRange(-5000.0f, 5000.0f);
			iIndex = get_gamecore()->spawn_unit<A_base_unit>(desc);
			loaded_unit_list.Add(iIndex);
		}
		b = false;
	}

	if (false)
	{
		for (int32  ui = 0; ui < loaded_unit_list.Num(); ++ui)
		{
			get_gamecore()->despawn_unit(loaded_unit_list[ui]);
		}		
		loaded_unit_list.Empty();
	}
	
	return true;
}