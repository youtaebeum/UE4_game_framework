// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "Engine/AssetManager.h"

REGIIST_OBJECTPOOLTYPE(F_resource_loader, OBJECT_POOL_DEFAULT_SIZE)

class F_resource_loader
{
public:
	F_resource_loader();
	~F_resource_loader();

private:
	e_resource_load_state    m_e_load_state = e_resource_load_state::none;
	UClass*					 m_p_class = nullptr;
	e_rsource_loading_type   m_e_loading_type = e_rsource_loading_type::instantly;
	E_resource_load_property m_e_property = E_resource_load_property::back;

	FString				   m_str_asset_path;
	FStringAssetReference  m_asset_ref;

	int32				   m_i_custom_index = 0;

	delegate_resource_load_complete  m_delegate_load_complete;
	delegate_resource_load_fail		 m_delegate_load_fail;

public:
	void set_load_info(const F_load_resource_desc& _desc, 
					   delegate_resource_load_complete _delegate_load_complete, 
		               delegate_resource_load_fail _delegate_load_fail);

	void clear();
	void load_start();

	e_resource_load_state get_load_state(); 

private:
	void load_deferred();

};
