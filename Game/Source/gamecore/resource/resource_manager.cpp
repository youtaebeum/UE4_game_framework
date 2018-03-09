// Fill out your copyright notice in the Description page of Project Settings.

#include "resource_manager.h"
#include "gamecore_include.h"

void U_resource_manager::_initialize()
{

}

void U_resource_manager::_reset()
{
	for (int i = 0; i < m_load_list.Num(); ++i)	{
		m_load_list[i]->clear();
		gGameCore->return_object<F_resource_loader>(m_load_list[i]);
	}
	m_load_list.Empty();

	for (auto& Elem : m_map_wait_list)
	{
		TArray<F_resource_loader*>* pLoader = Elem.Value;
		for (int i = 0; i < pLoader->Num(); ++i)
		{
			(*pLoader)[i]->clear();
			gGameCore->return_object<F_resource_loader>((*pLoader)[i]);
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
		e_resource_load_state m_e_load_state = m_load_list[i]->get_load_state();
		if (m_e_load_state == e_resource_load_state::complete ||
			m_e_load_state == e_resource_load_state::fail)
		{
			gGameCore->return_object<F_resource_loader>(m_load_list[i]);
			m_load_list.RemoveAt(i++);
		}
	}

	//AddLoadList
	if (m_load_list.Num() < m_i_resource_load_capacity)
	{
		for (auto Elem : m_map_wait_list)
		{
			TArray<F_resource_loader*>* pLoadList = Elem.Value;
			for (int i = 0; i < pLoadList->Num(); ++i)
			{
				(*pLoadList)[i]->load_start();

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
	delegate_resource_load_fail _delegate_load_fail)
{
	int32 _i_property = (int32)_desc._e_property;
	TArray<F_resource_loader*>* pLoadList = GC_UTILTY::safe_map_value(m_map_wait_list.Find(_i_property));
	if (pLoadList == nullptr)
	{
		m_map_wait_list.Add(_i_property, GC_New(TArray<F_resource_loader*>));
		m_map_wait_list.KeySort([](int32 A, int32 B) {
			return A > B;
		});
	}

	pLoadList = GC_UTILTY::safe_map_value(m_map_wait_list.Find(_i_property));

	F_resource_loader* pLoader = gGameCore->get_object<F_resource_loader>();
	pLoader->clear();
	pLoader->set_load_info(_desc, _delegate_load_complete, _delegate_load_fail);
	pLoadList->Add(pLoader);
}