// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "base_scene.generated.h"

UCLASS()
class GAMECORE_API U_base_scene : public UObject
{
	GENERATED_BODY()

public:
	void init(uint8 _i_index, const FName& _name);

private:
	UPROPERTY()	int32 m_i_state_index = -1;
	UPROPERTY()	FName m_state_name;

public:
	UFUNCTION() void on_begin_state();
	UFUNCTION() void on_update_state();
	UFUNCTION() void on_exit_state();

private:
	virtual void enter() {}
	virtual void update(float _f_delta_time) {}
	virtual void exit() {}

	FORCEINLINE uint8 get_state_index()	{
		return m_i_state_index;
	}

	FORCEINLINE FName get_state_name() {
		return m_state_name;
	}
};
