// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "GameFramework/Pawn.h"
#include "input_define.h"
#include "controller_pawn.generated.h"

////////////////////////////////////////////////////////////////////////////////

UCLASS()
class GAMECORE_API A_controller_pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	A_controller_pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//------------------------------------------------------------------------//
private:
	UPROPERTY(Category=controller_pawn, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UInputComponent* m_p_input_componenet = nullptr;

	class A_base_actor*				m_p_controll_actor = nullptr;
	TArray<TTuple<float, FVector>>	m_move_axis_history;

	TMap<FName, delegate_axis_event>				 m_event_axis;;
	TMap<E_action_event_type, delegate_action_event> m_event_action;

public:
	void init();
	void reset();

	void apply_action_event(E_action_event_type _e_type);

	void bind_axis_event(FName _axis_name, delegate_axis_event::FDelegate _delegate);
	void bind_action_event(E_action_event_type _e_type, delegate_action_event::FDelegate _delegate);
	void clear_event();

	FVector	get_move_axis(float _f_sampling_time = CONTROLLER_PAWN_AXIS_SAMPLING_INTERVAL);

private:
	void add_move_axis_history(float _f_delta_time, const FVector& _v_axis);

	void broadcast_axis_event(FName _axis_name, float _f_axis);
	void broadcast_action_event(E_action_event_type eType);

	// event func
	void axis_move_envet(float _f_value);			//ForwardBack과 RightLeft의 두 Axis를 한번에 처리하기 위해 한 곳에서만 처리한다.
	void axis_move_dummy_event(float _f_value);

	void axis_touch_event(float _f_value);
	void axis_pinch_event(float _f_value);
};
