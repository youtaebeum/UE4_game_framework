#include "unit_manager.h"

#include "gamecore_include.h"
#include "gamecore_manager.h"

void U_unit_manager::_initialize()
{
	U_unit_manager::_get_instance()->_spawn_manager_actor<AActor>(gGameCore->GetWorld());
}

void U_unit_manager::_reset()
{
	m_ui_create_count = 0;
	for (auto& Elem : m_map_unit) 
	{
		despawn_unit(Elem.Value);
	}

	m_map_unit.Empty();
};

void U_unit_manager::_tick(float _f_delta_seconds)
{

};

A_base_unit* U_unit_manager::get_unit(uint32 _i_uniq_index)
{
	A_base_unit* p_unit = GC_UTILTY::safe_map_value<A_base_unit>(m_map_unit.Find(_i_uniq_index));
	if (p_unit == nullptr)
	{
		GC_WARNING(TEXT("[U_unit_manager::get_unit] not exist Unit(%d)"), _i_uniq_index);
		return nullptr;
	}

	return p_unit;
}

void U_unit_manager::despawn_unit(uint32 _i_uniq_index)
{
	A_base_unit* pUnit = GC_UTILTY::safe_map_value<A_base_unit>(m_map_unit.Find(_i_uniq_index));
	if (pUnit == nullptr) {
		GC_ERROR(TEXT("[U_unit_manager::despawn_unit] not exist Unit(%d)"), _i_uniq_index);
		return;
	}

	despawn_unit(pUnit);
	m_map_unit.Remove(_i_uniq_index);
}

void U_unit_manager::despawn_unit(A_base_unit* _p_unit)
{
	GC_CHECK(_p_unit != nullptr)

	_p_unit->_reset();
#if WITH_EDITOR
	_get_instance()->_DetachToManagerActor(_p_unit);
#endif

	gGameCore->return_actor(_p_unit);
}