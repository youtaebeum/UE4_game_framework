// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "game_minimal.h"

#include "Engine/GameInstance.h"
#include "Ticker.h"

#include "game_instance.generated.h"

UCLASS()
class GAME_API Ugame_instance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
	virtual void Shutdown() override;
	
private:
	FDelegateHandle  m_tick_delegate_handle;

/////////////////////////////////////////////////////////
// TestCode
	uint32 m_ui_self_unit = 0;

	TArray<int32> loaded_unit_list;
	FRotator r_camera_rotation = FRotator::ZeroRotator;

	TArray<FString> _Assets;

	UFUNCTION() void input_event_move(float _f_axis);
	UFUNCTION() void input_event_touch(float _f_axis);
	UFUNCTION() void input_event_pinch(float _f_axis);
/////////////////////////////////////////////////////////

private:
	bool tick(float _f_delta_seconds);
};
