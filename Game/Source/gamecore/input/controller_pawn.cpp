// Fill out your copyright notice in the Description page of Project Settings.

#include "controller_pawn.h"
#include "gamecore_manager.h"

#include "Engine/InputDelegateBinding.h"
#include "Components/InputComponent.h"

// Sets default values
A_controller_pawn::A_controller_pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//------------------------------------------------------------------------//
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AIControllerClass = APlayerController::StaticClass();
}

// Called when the game starts or when spawned
void A_controller_pawn::BeginPlay()
{
	Super::BeginPlay();

	m_move_axis_history.Reset(CONTROLLER_PAWN_AXIS_HISTORY_SIZE);
}

// Called every frame
void A_controller_pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	///test
	if (gGameCore->get_controll_unit_index() != GC_INDEX_NONE)
	{
		FRotator r_camera_rotation = FRotator(-25, 0, 0);
		FVector v_camera_base_position = FVector(-200, 0.0f, 0.0f);
		FVector v_camera_relative_location = FRotationMatrix(FRotator(r_camera_rotation)).TransformPosition(v_camera_base_position);
		r_camera_rotation.Normalize();

		A_base_unit* p_unit = gGameCore->get_unit(gGameCore->get_controll_unit_index());
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
	////
}

// Called to bind functionality to input
void A_controller_pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	m_p_input_componenet = PlayerInputComponent;
	if(m_p_input_componenet != nullptr)
	{
		{ // axis
			m_p_input_componenet->BindAxis(DEFAULT_AXIS_EVENT_MOVEFORWARDBACK, this, &A_controller_pawn::axis_move_envet);
			m_p_input_componenet->BindAxis(DEFAULT_AXIS_EVENT_MOVERIGHTLEFT, this, &A_controller_pawn::axis_move_dummy_event);
			m_p_input_componenet->BindAxis(DEFAULT_AXIS_EVENT_TOUCH, this, &A_controller_pawn::axis_touch_event);
			m_p_input_componenet->BindAxis(DEFAULT_AXIS_EVENT_PINCH, this, &A_controller_pawn::axis_pinch_event);
		}

		{ // action
			for (int32 i_cnt = 0; i_cnt < (int32)E_action_event_type::Max; ++i_cnt)
			{
				E_action_event_type eType = (E_action_event_type)i_cnt;
				FString str_enum = GC_UTILTY::get_enum_to_string<E_action_event_type>(TEXT("E_action_event_type"), eType);

				FInputActionBinding oBinding(FName(*str_enum), IE_Pressed);
				oBinding.ActionDelegate.GetDelegateForManualSet().BindLambda(
				[this, eType]() 
				{ 
					apply_action_event(eType); 
				});

				m_p_input_componenet->AddActionBinding(oBinding);
			}
		}
	}
}

void A_controller_pawn::init()
{
	m_p_input_componenet->Activate();
}

void A_controller_pawn::reset()
{
	m_p_input_componenet->Deactivate();
	clear_event();
}

void A_controller_pawn::apply_action_event(E_action_event_type _e_type)
{
	broadcast_action_event(_e_type);
}

void A_controller_pawn::bind_axis_event(FName _axis_name, delegate_axis_event::FDelegate _delegate)
{
	delegate_axis_event& ref_delegator = m_event_axis.FindOrAdd(_axis_name);
	ref_delegator.Add(_delegate);
}

void A_controller_pawn::bind_action_event(E_action_event_type _e_type, delegate_action_event::FDelegate _delegate)
{
	delegate_action_event& ref_delegator = m_event_action.FindOrAdd(_e_type);
	ref_delegator.Add(_delegate);
}

void A_controller_pawn::clear_event()
{
	m_event_axis.Empty();
	m_event_action.Empty();
}

FVector	A_controller_pawn::get_move_axis(float _f_sampling_time)
{
	float time = 0.0f;
	for (auto It = m_move_axis_history.CreateConstIterator(); It; ++It) {
		time += It->Key;

		if (It->Value.IsZero() == false) {
			return It->Value;
		}

		if (time >= _f_sampling_time) {
			break;
		}
	}

	return FVector(0, 0, 0);
}

void A_controller_pawn::add_move_axis_history(float _f_delta_time, const FVector& _v_axis)
{
	m_move_axis_history.Insert(TTuple<float, FVector>(_f_delta_time, _v_axis), 0);

	if (m_move_axis_history.Num() > CONTROLLER_PAWN_AXIS_HISTORY_SIZE)
		m_move_axis_history.Pop();
}

void A_controller_pawn::broadcast_axis_event(FName _axis_name, float _f_axis)
{
	delegate_axis_event* p_delegator = m_event_axis.Find(_axis_name);
	if (p_delegator) {
		p_delegator->Broadcast(_f_axis);
	}
}

void A_controller_pawn::broadcast_action_event(E_action_event_type eType)
{
	delegate_action_event* p_delegator = m_event_action.Find(eType);
	if (p_delegator) {
		p_delegator->Broadcast();
	}
}

void A_controller_pawn::axis_move_envet(float _f_value)
{
	float f_delta_time = gGameCore->get_world_delta_time();
	float f_axis_forward_back = m_p_input_componenet->GetAxisValue(DEFAULT_AXIS_EVENT_MOVEFORWARDBACK);
	float f_axis_right_left = m_p_input_componenet->GetAxisValue(DEFAULT_AXIS_EVENT_MOVERIGHTLEFT);

	FVector v_axis = FVector(f_axis_forward_back, f_axis_right_left, 0);
	
	add_move_axis_history(f_delta_time, v_axis);
	broadcast_axis_event(DEFAULT_AXIS_EVENT_MOVEFORWARDBACK, f_axis_forward_back);
}

void A_controller_pawn::axis_move_dummy_event(float _f_value)
{
	float f_axis_right_left = m_p_input_componenet->GetAxisValue(DEFAULT_AXIS_EVENT_MOVERIGHTLEFT);
	broadcast_axis_event(DEFAULT_AXIS_EVENT_MOVERIGHTLEFT, f_axis_right_left);
}

void A_controller_pawn::axis_touch_event(float _f_value)
{
	broadcast_axis_event(DEFAULT_AXIS_EVENT_TOUCH, _f_value);
}

void A_controller_pawn::axis_pinch_event(float _f_value)
{
	broadcast_axis_event(DEFAULT_AXIS_EVENT_PINCH, _f_value);
}