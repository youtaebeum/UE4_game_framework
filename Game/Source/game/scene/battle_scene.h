// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "scene/base_scene.h"
#include "battle_scene.generated.h"

UCLASS()
class U_battle_scene : public U_base_scene
{
	GENERATED_BODY()

private:
	virtual void enter() override;
	virtual void update(float _f_delta_time) override;
	virtual void exit() override;

private:
	/////////////////////////////////////////////////////////
	// TestCode
	uint32 m_ui_self_unit = 0;
	bool m_b_create_test_actor = false;

	TArray<int32> loaded_unit_list;
	FRotator r_camera_rotation = FRotator::ZeroRotator;

	TArray<FString> _Assets;

	UFUNCTION() void input_event_move(float _f_axis);
	UFUNCTION() void input_event_touch(float _f_axis);
	UFUNCTION() void input_event_pinch(float _f_axis);

	UFUNCTION() void input_event_test();
	/////////////////////////////////////////////////////////
};
