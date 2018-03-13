// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "base_unit.h"
#include "unit_manager.generated.h"

UCLASS()
class GAMECORE_API U_unit_manager : public UObject, public F_singleton<U_unit_manager>
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

private:
	uint32					   m_ui_create_count = 0;
	TMap<uint32, A_base_unit*> m_map_unit;

public:
	template<typename TF> uint32	spawn_unit(const F_spawn_unit_desc& _desc = F_spawn_unit_desc::get());
	void		despawn_unit(uint32 _i_uniq_index);
	A_base_unit* get_unit(uint32 _i_uniq_index);

private:
	void despawn_unit(A_base_unit* _p_unit);
};

//////////////////////////////////////////////////////////////////////////
template<typename TF>
uint32 U_unit_manager::spawn_unit(const F_spawn_unit_desc& _desc)
{
	m_ui_create_count++;

	A_base_unit* pUnit = gGameCore->get_actor<TF>();

	pUnit->SetActorLocation(_desc._v_location);
	pUnit->SetActorRotation(_desc._r_rotator);

	pUnit->_initialize(m_ui_create_count);

#if WITH_EDITOR
	_get_instance()->_AttachToManagerActor(pUnit);
#endif

	m_map_unit.Add(m_ui_create_count, pUnit);

	return m_ui_create_count;
}
