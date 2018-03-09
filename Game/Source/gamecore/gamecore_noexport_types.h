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

UENUM()
enum E_resource_load_property 
{
	front = -128,		

	animinstance, 		

	mesh_group_0,		
	mesh_group_1,		
	mesh_group_2,		

	back = 127
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
	E_resource_load_property _e_property = E_resource_load_property::back;
	int32					 _i_custom_index = 0;

	static struct F_spawn_unit_desc & get()
	{
		static F_spawn_unit_desc Desc;
		return Desc;
	}
};

