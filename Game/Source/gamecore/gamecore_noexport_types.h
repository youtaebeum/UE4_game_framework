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

	e_rsource_loading_type _e_load_type;

	static struct F_spawn_unit_desc & get()
	{
		static F_spawn_unit_desc Desc;
		return Desc;
	}
};
////////////////////////////////////////////////////////////////////////////


