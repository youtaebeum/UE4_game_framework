
#include "game_instance.h"
#include "gamecore_include.h"

#include "GameFramework/PlayerController.h"

void Ugame_instance::Init()
{
	Super::Init();

	// Register delegate for ticker callback
	m_tick_delegate_handle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &Ugame_instance::tick));

	//----------------------------------------------------------------------------------------------------------------------//
	if (U_gamecore_manager::_has_Instance() == false) U_gamecore_manager::_make_instance();

	U_gamecore_manager::_get_instance()->set_game_instance(this);
	U_gamecore_manager::_get_instance()->_initialize();	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// test code
	loaded_unit_list.Empty();
	m_ui_self_unit = 0;

	gGameCore->bind_input_axis_event(DEFAULT_AXIS_EVENT_MOVEFORWARDBACK, delegate_axis_event::FDelegate::CreateUObject(this, &Ugame_instance::input_event_move));
	gGameCore->bind_input_axis_event(DEFAULT_AXIS_EVENT_TOUCH, delegate_axis_event::FDelegate::CreateUObject(this, &Ugame_instance::input_event_touch));
	gGameCore->bind_input_axis_event(DEFAULT_AXIS_EVENT_PINCH, delegate_axis_event::FDelegate::CreateUObject(this, &Ugame_instance::input_event_pinch));
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Ugame_instance::Shutdown()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// test code
	gGameCore->despawn_unit(m_ui_self_unit);

	for (int32 i = 0; i < loaded_unit_list.Num(); ++i)
	{
		gGameCore->despawn_unit(loaded_unit_list[i]);
	}
	loaded_unit_list.Empty();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	U_gamecore_manager::_remove_instance();

	FTicker::GetCoreTicker().RemoveTicker(m_tick_delegate_handle);

	Super::Shutdown();
}

bool Ugame_instance::tick(float _f_delta_seconds)
{
	if (U_gamecore_manager::_get_instance() != nullptr)
		U_gamecore_manager::_get_instance()->_tick(_f_delta_seconds);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// test code
	if (m_ui_self_unit != GC_INDEX_NONE)
	{
		FVector v_camera_base_position = FVector(-500, 0.0f, 0.0f);
		FVector v_camera_relative_location = FRotationMatrix(FRotator(r_camera_rotation)).TransformPosition(v_camera_base_position);
		r_camera_rotation.Normalize();

		A_base_unit* p_unit = gGameCore->get_unit(m_ui_self_unit);
		A_controller_pawn* p_controll_pawn = gGameCore->get_controller_pawn();
		if (p_unit && p_controll_pawn)
		{
			v_camera_relative_location += p_unit->GetActorLocation();

			p_controll_pawn->SetActorLocation(v_camera_relative_location);
			p_controll_pawn->SetActorRotation(r_camera_rotation);

			p_unit->BaseEyeHeight = 400.0f;

			APlayerController* p_player_controller = gGameCore->get_player_controller();
			GC_CHECK(p_player_controller != nullptr);

			p_player_controller->SetControlRotation(r_camera_rotation);	//PlayerCamera의 Rotation을 적용하려면 Controller의 Rotation을 적용해줘야 한다.
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// test code
void Ugame_instance::input_event_move(float _f_axis)
{
	FVector v_input_axis = gGameCore->get_intput_move_axis();
	if (v_input_axis.IsNearlyZero() == false)
	{
		A_base_unit* p_controll_unit = gGameCore->get_unit(m_ui_self_unit); // <- 이런 코드는 캐싱해서 쓰는게 좋을듯
		APlayerController* p_player_controller = gGameCore->get_player_controller();
		if (p_controll_unit && p_player_controller)
		{
			APlayerCameraManager* pPlayerCameraManager = p_player_controller->PlayerCameraManager;
			v_input_axis = v_input_axis.RotateAngleAxis(pPlayerCameraManager->GetCameraRotation().Yaw, FVector(0, 0, 1));

			if (v_input_axis.IsNearlyZero() == false) {

				FVector v_location = p_controll_unit->GetActorLocation() + v_input_axis * gGameCore->get_world_delta_time() * 100.0f;
				p_controll_unit->SetActorLocation(v_location);
			}
		}
	}
}

void Ugame_instance::input_event_touch(float _f_axis)
{
	if (_f_axis >= 0.001f)
	{
		if (m_ui_self_unit == 0)
		{
			F_spawn_unit_desc desc;
			desc._e_load_type = e_rsource_loading_type::instantly;
			m_ui_self_unit = gGameCore->spawn_unit<A_base_unit>(desc);
			gGameCore->set_controll_unit(m_ui_self_unit);
		}

		uint32 iIndex = 0;
		F_spawn_unit_desc desc;
		desc._e_load_type = e_rsource_loading_type::async;
		for (uint32 ui = 0; ui < 10; ++ui)
		{
			desc._v_location.X = FMath::RandRange(-5000.0f, 5000.0f);
			desc._v_location.Y = FMath::RandRange(-5000.0f, 5000.0f);
			iIndex = gGameCore->spawn_unit<A_base_unit>(desc);
			loaded_unit_list.Add(iIndex);
		}
	}
}

void Ugame_instance::input_event_pinch(float _f_axis)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////