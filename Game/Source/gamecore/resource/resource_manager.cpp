// Fill out your copyright notice in the Description page of Project Settings.

#include "resource_manager.h"
#include "gamecore_include.h"

void U_resource_manager::_initialize()
{

}

void U_resource_manager::_reset()
{
	for (int i = 0; i < m_load_list.Num(); ++i)	{
		m_load_list[i].get()->clear();
		gGameCore->return_uobject(m_load_list[i].get());
	}
	m_load_list.Empty();

	for (auto& Elem : m_map_wait_list)
	{
		TArray<F_resource_loader>* pLoader = Elem.Value;
		for (int i = 0; i < pLoader->Num(); ++i)
		{
			(*pLoader)[i].get()->clear();
			gGameCore->return_uobject((*pLoader)[i].get());
		}
		Elem.Value->Empty();
		GC_Delete(Elem.Value);
	}

	m_map_wait_list.Empty();
}

void U_resource_manager::_tick(float _f_delta_seconds)
{
	//consume 
	for (int i = 0; i < m_load_list.Num(); ++i)
	{
		e_resource_load_state m_e_load_state = m_load_list[i].get()->get_load_state();
		if (m_e_load_state == e_resource_load_state::complete ||
			m_e_load_state == e_resource_load_state::fail)
		{
			m_load_list[i].get()->clear();
			gGameCore->return_uobject(m_load_list[i].get());
			m_load_list.RemoveAt(i++);
		}
	}

	//AddLoadList
	if (m_load_list.Num() < m_i_resource_load_capacity)
	{
		for (auto Elem : m_map_wait_list)
		{
			TArray<F_resource_loader>* pLoadList = Elem.Value;
			for (int i = 0; i < pLoadList->Num(); ++i)
			{
				(*pLoadList)[i].get()->load_start();

				m_load_list.Add((*pLoadList)[i]);
				pLoadList->RemoveAt(i++);
				
				if (m_load_list.Num() >= m_i_resource_load_capacity) {
					break;
				}
			}

			if (m_load_list.Num() >= m_i_resource_load_capacity) {
				break;
			}
		}
	}
}

void U_resource_manager::load_resource(const F_load_resource_desc& _desc,
	delegate_resource_load_complete _delegate_load_complete, 
	delegate_resource_load_fail _delegate_load_fail, bool _b_sort)
{
	int32 _i_property = _desc._i_property;
	TArray<F_resource_loader>* p_load_list = GC_UTILTY::safe_map_value(m_map_wait_list.Find(_i_property));
	if (p_load_list == nullptr)
	{
		m_map_wait_list.Add(_i_property, GC_New(TArray<F_resource_loader>));
		m_map_wait_list.KeySort([](int32 A, int32 B) {
			return A > B;
		});
	}

	p_load_list = GC_UTILTY::safe_map_value(m_map_wait_list.Find(_i_property));

	U_resource_loader* p_loader = gGameCore->get_uobject<U_resource_loader>();
	p_loader->clear();
	p_loader->set_load_info(_desc, _delegate_load_complete, _delegate_load_fail);
	p_load_list->Add(F_resource_loader(p_loader));

	if (_b_sort)
	{
		FVector v_camera_location = gGameCore->get_camera_location();
		p_load_list->Sort([v_camera_location]( const F_resource_loader& A, const F_resource_loader& B)
		{
			FVector v_camera_location = gGameCore->get_camera_location();
			float dist_a = FVector::DistSquared(v_camera_location, A.p_loader->get_load_info()._v_loaded_location);
			float dist_b = FVector::DistSquared(v_camera_location, B.p_loader->get_load_info()._v_loaded_location);
			return dist_a < dist_b;
		});
	}
}