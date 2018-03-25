// Fill out your copyright notice in the Description page of Project Settings.

#include "battle_scene.h"
#include "gamecore_include.h"
#include "gamecore_manager.h"

#include "unit_actor/player_unit.h"
#include "unit_actor/player_prediction_unit.h"

#include "GameFramework/PlayerController.h"

void U_battle_scene::enter()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// test code
	loaded_unit_list.Empty();
	m_ui_self_unit = 0;

	gGameCore->bind_input_axis_event(DEFAULT_AXIS_EVENT_MOVEFORWARDBACK, delegate_axis_event::FDelegate::CreateUObject(this, &U_battle_scene::input_event_move));
	gGameCore->bind_input_axis_event(DEFAULT_AXIS_EVENT_TOUCH, delegate_axis_event::FDelegate::CreateUObject(this, &U_battle_scene::input_event_touch));
	gGameCore->bind_input_axis_event(DEFAULT_AXIS_EVENT_PINCH, delegate_axis_event::FDelegate::CreateUObject(this, &U_battle_scene::input_event_pinch));

	gGameCore->bind_input_action_event(E_action_event_type::test_event_0, delegate_action_event::FDelegate::CreateUObject(this, &U_battle_scene::input_event_test));

	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Barbarous.SK_CharM_Barbarous'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/sk_CharM_Base.sk_CharM_Base'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Bladed.SK_CharM_Bladed'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Forge.SK_CharM_Forge'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_FrostGiant.SK_CharM_FrostGiant'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Natural.SK_CharM_Natural'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Pit.SK_CharM_Pit'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ragged0.SK_CharM_Ragged0'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_RaggedElite.SK_CharM_RaggedElite'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ram.SK_CharM_Ram'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Robo.SK_CharM_Robo'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Shell.SK_CharM_Shell'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_solid.SK_CharM_solid'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Standard.SK_CharM_Standard'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Tusk.SK_CharM_Tusk'");
	_Assets.Add("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior'");

	F_spawn_unit_desc desc;
	desc._e_load_type = e_rsource_loading_type::async;
	m_ui_self_unit = gGameCore->spawn_unit<A_player_prediction_unit>(desc);
	gGameCore->set_controll_unit(m_ui_self_unit);

//	int iIndex = gGameCore->spawn_unit<A_player_unit>(desc);
//	A_player_unit* p_unit = gGameCore->get_unit<A_player_unit>(iIndex);
//	gGameCore->set_controll_unit(iIndex);
//	loaded_unit_list.Add(iIndex);

	/*for (int i = 0; i < 500; ++i)
	{
		desc._v_location.X = FMath::RandRange(-5000.0f, 5000.0f);
		desc._v_location.Y = FMath::RandRange(-5000.0f, 5000.0f);
		int iIndex = gGameCore->spawn_unit<A_player_prediction_unit>(desc);
		loaded_unit_list.Add(iIndex);
	}*/

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void U_battle_scene::update(float _f_delta_time)
{
}

void U_battle_scene::exit()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// test code
	gGameCore->despawn_unit(m_ui_self_unit);

	for (int32 i = 0; i < loaded_unit_list.Num(); ++i) {
		gGameCore->despawn_unit(loaded_unit_list[i]);
	}
	loaded_unit_list.Empty();

	gGameCore->clear_input_event();
	gGameCore->clear_load_list();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// test code
void U_battle_scene::input_event_move(float _f_axis)
{
	FVector v_input_axis = gGameCore->get_intput_move_axis();
	if (v_input_axis.IsNearlyZero() == false)
	{
		A_base_unit* p_controll_unit = gGameCore->get_unit(gGameCore->get_controll_unit_index());
		APlayerController* p_player_controller = gGameCore->get_player_controller();
		if (p_controll_unit && p_player_controller)
		{
			APlayerCameraManager* pPlayerCameraManager = p_player_controller->PlayerCameraManager;
			v_input_axis = v_input_axis.RotateAngleAxis(pPlayerCameraManager->GetCameraRotation().Yaw, FVector(0, 0, 1));

			if (v_input_axis.IsNearlyZero() == false) {

				p_controll_unit->AddMovementInput(v_input_axis);
				//p_controll_unit->AddMovementInput(v_input_axis * 100.0f);
				//FVector v_location = p_controll_unit->GetActorLocation() + v_input_axis * gGameCore->get_world_delta_time() * 400.0f;
				//	p_controll_unit->SetActorLocation(p_controll_pawn);
			}
		}
	}
}

void U_battle_scene::input_event_touch(float _f_axis)
{
}

void U_battle_scene::input_event_pinch(float _f_axis)
{

}

void U_battle_scene::input_event_test()
{
	gGameCore->change_scene(TEXT("login"));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////