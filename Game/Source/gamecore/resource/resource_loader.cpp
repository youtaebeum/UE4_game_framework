// Fill out your copyright notice in the Description page of Project Settings.

#include "resource_loader.h"// Fill out your copyright notice in the Description page of Project Settings.
#include "gamecore_manager.h"

void U_resource_loader::set_load_info(const F_load_resource_desc& _desc,
	delegate_resource_load_complete _delegate_load_complete,
	delegate_resource_load_fail _delegate_load_fail)
{
	GC_CHECK(m_e_load_state == e_resource_load_state::none)
	GC_CHECK(_desc._str_path.IsEmpty() == false)
	GC_CHECK(_desc._p_class != nullptr)

	m_load_info = _desc;

	m_e_load_state = e_resource_load_state::ready;
	m_asset_ref = _desc._str_path;
	m_delegate_load_complete = _delegate_load_complete;
	m_delegate_load_fail = _delegate_load_fail;
}

void U_resource_loader::clear()
{
	m_load_info.clear();

	m_e_load_state = e_resource_load_state::none;
	m_asset_ref.Reset();
	m_delegate_load_complete.Unbind();
	m_delegate_load_fail.Unbind();

	if (m_streamable_handle.IsValid() == true) {
		m_streamable_handle->CancelHandle();
	}
}

void U_resource_loader::load_start()
{
	m_e_load_state = e_resource_load_state::loading;

	switch (m_load_info._e_loading_type)
	{
	case e_rsource_loading_type::instantly:
	{
		UObject* pLoadedObject = StaticLoadObject(m_load_info._p_class, NULL, *m_load_info._str_path);
		if (pLoadedObject == nullptr)
		{
			GC_WARNING(TEXT("[F_resource_loader::LoadInstantly] LoadFail(%s)"), *m_load_info._str_path);
			m_delegate_load_fail.Execute(m_asset_ref, m_load_info._p_class, m_load_info._i_custom_index);
			m_e_load_state = e_resource_load_state::fail;
		}
		else
		{
			GC_LOG(TEXT("[F_resource_loader::LoadInstantly] LoadComplete(%s)"), *m_load_info._str_path);
			m_delegate_load_complete.Execute(m_asset_ref, m_load_info._p_class, m_load_info._i_custom_index);
			m_e_load_state = e_resource_load_state::complete;
		}
	}break;
	case e_rsource_loading_type::async:
	{
		auto& assetLoader = gGameCore->get_streamable_manager();
		m_streamable_handle = assetLoader.RequestAsyncLoad(m_asset_ref, FStreamableDelegate::CreateUObject(this, &U_resource_loader::load_deferred), m_load_info._i_priority);
		if (m_streamable_handle.IsValid() == false)
		{
			GC_WARNING(TEXT("[F_resource_loader::LoadASync] LoadFail(%s)"), *m_load_info._str_path);
			m_delegate_load_fail.Execute(m_asset_ref, m_load_info._p_class, m_load_info._i_custom_index);
			m_e_load_state = e_resource_load_state::fail;
		}
	}break;
	};
}

void U_resource_loader::load_deferred()
{
	m_e_load_state = e_resource_load_state::complete;
	if (m_delegate_load_complete.GetUObject() == nullptr) return;

	bool is_pooled = false;
	if (m_delegate_load_complete.GetUObject()->IsA(AActor::StaticClass())) {
		is_pooled = gGameCore->is_pooled_actor(Cast<AActor>(m_delegate_load_complete.GetUObject()));
	}

	if (is_pooled == false)
	{
		GC_LOG(TEXT("[F_resource_loader::load_deferred] LoadComplete(%s)"), *m_load_info._str_path);
		m_delegate_load_complete.Execute(m_asset_ref, m_load_info._p_class, m_load_info._i_custom_index);
	}

	m_streamable_handle = nullptr;
}

e_resource_load_state U_resource_loader::get_load_state()
{
	return m_e_load_state;
}

const F_load_resource_desc& U_resource_loader::get_load_info()
{
	return m_load_info;
}