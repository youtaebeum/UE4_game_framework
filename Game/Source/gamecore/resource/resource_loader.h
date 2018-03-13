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
	UPROPERTY()
	F_load_resource_desc	m_load_info;

private:
	e_resource_load_state			 m_e_load_state = e_resource_load_state::none;

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
	const F_load_resource_desc& get_load_info();

private:
	void load_deferred();

public:
/*	bool operator()(const U_resource_loader* A, const U_resource_loader* B) const
	{
		FVector v_camera_location = gGameCore->get_camera_location();
		float dist_a = FVector::DistSquared(v_camera_location, A->get_load_info()._v_loaded_location);
		float dist_b = FVector::DistSquared(v_camera_location, B->get_load_info()._v_loaded_location);
		return dist_a > dist_b;
	}*/
};
