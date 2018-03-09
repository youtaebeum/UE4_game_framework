// Fill out your copyright notice in the Description page of Project Settings.

#include "resource_loader.h"// Fill out your copyright notice in the Description page of Project Settings.

F_resource_loader::F_resource_loader()
{
	clear();
}

F_resource_loader::~F_resource_loader()
{
	clear();
}

void F_resource_loader::set_load_info(const F_load_resource_desc& _desc,
	delegate_resource_load_complete _delegate_load_complete,
	delegate_resource_load_fail _delegate_load_fail)
{
	GC_CHECK(m_e_load_state == e_resource_load_state::none)
	GC_CHECK(_desc._str_path.IsEmpty() == false)
	GC_CHECK(_desc._p_class != nullptr)

	m_e_load_state = e_resource_load_state::ready;
	m_p_class = _desc._p_class;
	m_e_loading_type = _desc._e_loading_type;
	m_str_asset_path = _desc._str_path;
	m_asset_ref = _desc._str_path;
	m_delegate_load_complete = _delegate_load_complete;
	m_delegate_load_fail = _delegate_load_fail;
	m_e_property = _desc._e_property;
	m_i_custom_index = _desc._i_custom_index;
}

void F_resource_loader::clear()
{
	auto& assetLoader = UAssetManager::GetStreamableManager();
	assetLoader.Unload(m_asset_ref);

	m_e_load_state = e_resource_load_state::none;
	m_p_class = nullptr;
	m_e_loading_type = e_rsource_loading_type::instantly;
	m_str_asset_path.Empty();
	m_asset_ref.Reset();
	m_delegate_load_complete.Unbind();
	m_delegate_load_fail.Unbind();
	m_i_custom_index = -1;
}

void F_resource_loader::load_start()
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
		TSharedPtr<FStreamableHandle> NewHandle = assetLoader.RequestAsyncLoad(m_asset_ref, FStreamableDelegate::CreateRaw(this, &F_resource_loader::load_deferred));

		if (NewHandle.IsValid() == false)
		{
			GC_WARNING(TEXT("[F_resource_loader::LoadASync] LoadFail(%s)"), *m_str_asset_path);
			m_delegate_load_fail.Execute(m_asset_ref, m_p_class, m_i_custom_index);
			m_e_load_state = e_resource_load_state::fail;
		}
	}break;
	};
}

void F_resource_loader::load_deferred()
{
	GC_LOG(TEXT("[F_resource_loader::load_deferred] LoadComplete(%s)"), *m_str_asset_path);
	m_delegate_load_complete.Execute(m_asset_ref, m_p_class, m_i_custom_index);
	m_e_load_state = e_resource_load_state::complete;
};

e_resource_load_state F_resource_loader::get_load_state()
{
	return m_e_load_state;
}
