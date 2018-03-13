// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "Engine/AssetManager.h"

#include "resource_loader.generated.h"

REGIIST_OBJECTPOOLTYPE(U_resource_loader, OBJECT_POOL_DEFAULT_SIZE)

UCLASS()
class U_resource_loader : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY() UClass*					 m_p_class = nullptr;
	UPROPERTY() int32					 m_i_property = 0;
	UPROPERTY() FString					 m_str_asset_path;
	UPROPERTY() int32					 m_i_custom_index = 0;

	e_rsource_loading_type   m_e_loading_type = e_rsource_loading_type::instantly;

private:
	e_resource_load_state    m_e_load_state = e_resource_load_state::none;

	FStringAssetReference			 m_asset_ref;
	TSharedPtr<FStreamableHandle>	 m_streamable_handle = nullptr;

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
