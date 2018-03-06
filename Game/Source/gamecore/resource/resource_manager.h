// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "resource_loader.h"
#include "resource_manager.generated.h"

//------------------------------------------------------------------------------------------------------
// 앞으로 해야할 것 들
// 1. Actor의 경우 거리 소팅 기능 
// 2. 비동기, 동기 그룹 나누기
// 3. 우선순위 property 작업
//------------------------------------------------------------------------------------------------------

UCLASS()
class GAMECORE_API U_resource_manager : public UObject, public F_singleton<U_resource_manager>
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

private:
	int32					   m_i_resource_load_capacity = 10;	
	TArray<F_resource_loader*> m_load_list;
	TMap<UClass*, TArray<F_resource_loader*>*> m_map_wait_list;
	
public:
	void set_resource_load_capacity(int32 _iLoadListSize) { m_i_resource_load_capacity = _iLoadListSize; }

	void load_resource(UClass* _p_class, 
		e_rsource_loading_type _e_type,
		const FString& _str_path, 
		delegate_resource_load_complete _delegate_load_complete,
		delegate_resource_load_fail _delegate_load_fail);

};
