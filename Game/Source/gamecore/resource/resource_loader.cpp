// Fill out your copyright notice in the Description page of Project Settings.

#include "resource_loader.h"// Fill out your copyright notice in the Description page of Project Settings.

void U_resource_loader::set_load_info(const F_load_resource_desc& _desc,
	delegate_resource_load_complete _delegate_load_complete,
	delegate_resource_load_fail _delegate_load_fail)
{
	GC_CHECK(m_e_load_state == e_resource_load_state::none)
	GC_CHECK(_desc._str_path.IsEmpty() == false)
	GC_CHECK(_desc._p_class != nullptr)

	m_p_class = _desc._p_class;
	m_e_loading_type = _desc._e_loading_type;
	m_str_asset_path = _desc._str_path;
	m_i_property = _desc._i_property;
	m_i_custom_index = _desc._i_custom_index;

	m_e_load_state = e_resource_load_state::ready;
	m_asset_ref = _desc._str_path;
	m_delegate_load_complete = _delegate_load_complete;
	m_delegate_load_fail = _delegate_load_fail;
}

void U_resource_loader::clear()
{
	m_p_class = nullptr;
	m_e_loading_type = e_rsource_loading_type::instantly;
	m_str_asset_path.Empty();
	m_i_property = 0;
	m_i_custom_index = -1;

	m_e_load_state = e_resource_load_state::none;
	m_asset_ref.Reset();
	m_delegate_load_complete.Unbind();
	m_delegate_load_fail.Unbind();

	if (m_streamable_handle.IsValid() == true)
	{
		m_streamable_handle->CancelHandle();
	}
}

void U_resource_loader::load_start()
{
	m_e_load_state = e_resource_load_state::loading;

	switch (m_e_loading_type)
	{
	case e_rsource_loading_type::instantly:
	{
		UObject* pLoadedObject = StaticLoadObject(m_p_class, NULL, *m_str_asset_path);
		if (pLoadedObject == nullptr)
		{
			GC_WARNING(TEXT("[F_resource_loader::LoadInstantly] LoadFail(%s)"), *m_str_asset_path);
			m_delegate_load_fail.Execute(m_asset_ref, m_p_class, m_i_custom_index);
			m_e_load_state = e_resource_load_state::fail;
		}
		else
		{
			GC_LOG(TEXT("[F_resource_loader::LoadInstantly] LoadComplete(%s)"), *m_str_asset_path);
			m_delegate_load_complete.Execute(m_asset_ref, m_p_class, m_i_custom_index);
			m_e_load_state = e_resource_load_state::complete;
		}
	}break;
	case e_rsource_loading_type::async:
	{
		auto& assetLoader = UAssetManager::GetStreamableManager();
		m_streamable_handle = assetLoader.RequestAsyncLoad(m_asset_ref, FStreamableDelegate::CreateUObject(this, &U_resource_loader::load_deferred), m_i_property);
		if (m_streamable_handle.IsValid() == false)
		{
			GC_WARNING(TEXT("[F_resource_loader::LoadASync] LoadFail(%s)"), *m_str_asset_path);
			m_delegate_load_fail.Execute(m_asset_ref, m_p_class, m_i_custom_index);
			m_e_load_state = e_resource_load_state::fail;
		}
	}break;
	};
}

void U_resource_loader::load_deferred()
{
	if (m_p_class == nullptr) return;

	GC_LOG(TEXT("[F_resource_loader::load_deferred] LoadComplete(%s)"), *m_str_asset_path);
	m_delegate_load_complete.Execute(m_asset_ref, m_p_class, m_i_custom_index);
	m_e_load_state = e_resource_load_state::complete;
}

e_resource_load_state U_resource_loader::get_load_state()
{
	return m_e_load_state;
}
