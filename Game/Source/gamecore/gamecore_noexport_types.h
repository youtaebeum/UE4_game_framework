// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"

////////////////////////////////////////////////////////////////////////////

UENUM()
enum e_rsource_loading_type
{
	instantly,
	async,
};

UENUM()
enum e_resource_load_state
{
	none = 0,
	ready,
	loading,
	complete,
	fail
};

////////////////////////////////////////////////////////////////////////////
USTRUCT(noexport)
struct F_spawn_unit_desc
{
	//GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	FVector _v_location = FVector::ZeroVector;

	UPROPERTY()
	FRotator _r_rotator = FRotator::ZeroRotator;

	e_rsource_loading_type _e_load_type = e_rsource_loading_type::instantly;

	static struct F_spawn_unit_desc & get()
	{
		static F_spawn_unit_desc Desc;
		return Desc;
	}
};
	
////////////////////////////////////////////////////////////////////////////
USTRUCT(noexport)
struct F_load_resource_desc
{
public:
	UClass*					 _p_class = nullptr;
	FString					 _str_path = "";
	e_rsource_loading_type   _e_loading_type = e_rsource_loading_type::instantly;
	int32					 _i_priority = 0;
	int32					 _i_custom_index = 0;
	FVector					 _v_loaded_location = FVector::ZeroVector;

	static struct F_spawn_unit_desc & get()
	{
		static F_spawn_unit_desc Desc;
		return Desc;
	}

	void clear()
	{
		_p_class = nullptr;
		_str_path = "";
		_e_loading_type = e_rsource_loading_type::instantly;
		_i_priority = 0;
		_i_custom_index = 0;
		_v_loaded_location = FVector::ZeroVector;
	}
};

