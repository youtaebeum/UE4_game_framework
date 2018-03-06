// Fill out your copyright notice in the Description page of Project Settings.

#include "object_pool_manager.h"

void U_object_pool_manager::_initialize()
{
}

void U_object_pool_manager::_reset() 
{
	for (auto& Elem : m_object_pool)
		Elem.Value->clear();

	for (auto& Elem : m_uobject_pool)
		Elem.Value->clear();

	for (auto& Elem : m_actor_pool)
		Elem.Value->clear();

	m_object_pool.Empty();
	m_uobject_pool.Empty();
	m_actor_pool.Empty();
};

void U_object_pool_manager::_tick(float _f_delta_seconds)
{ 

};

//////////////////////////////////////////////////////////////////////////
F_object_pool<UObject>*	U_object_pool_manager::get_uobject_pool(UClass* p_uclass)
{
	GC_CHECK(p_uclass != nullptr);
	F_object_pool<UObject>* p_pool = GC_UTILTY::safe_map_value< F_object_pool<UObject> >(_get_instance()->m_uobject_pool.Find(p_uclass));
	return p_pool;
}

void U_object_pool_manager::return_uobject(UObject* _p_uobject)
{
	GC_CHECK(_p_uobject != nullptr);
	F_object_pool<UObject>* p_pool = _get_instance()->get_uobject_pool(_p_uobject->GetClass());
	GC_CHECK(p_pool != nullptr);
	p_pool->return_object<UObject>(_p_uobject);
}

bool U_object_pool_manager::is_pooled_uobject(UObject* _p_uobject)
{
	GC_CHECK(_p_uobject != nullptr);
	F_object_pool<UObject>* p_pool = _get_instance()->get_uobject_pool(_p_uobject->GetClass());
	GC_CHECK(p_pool != nullptr);
	return p_pool->IsPooled(_p_uobject);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
F_object_pool<A_base_actor>* U_object_pool_manager::get_actor_pool(UClass* p_uclass)
{
	GC_CHECK(p_uclass != nullptr);
	F_object_pool<A_base_actor>* p_pool = GC_UTILTY::safe_map_value< F_object_pool<A_base_actor> >(_get_instance()->m_actor_pool.Find(p_uclass));
	return p_pool;
}

void U_object_pool_manager::return_actor(A_base_actor* _p_actor)
{
	GC_CHECK(_p_actor != nullptr);
	F_object_pool<A_base_actor>* p_pool = _get_instance()->get_actor_pool(_p_actor->GetClass());
	GC_CHECK(p_pool != nullptr);
	p_pool->return_object<A_base_actor>(_p_actor);
}

bool U_object_pool_manager::is_pooled_actor(A_base_actor* _p_actor)
{
	GC_CHECK(_p_actor != nullptr);
	F_object_pool<A_base_actor>* p_pool = _get_instance()->get_actor_pool(_p_actor->GetClass());
	GC_CHECK(p_pool != nullptr);
	return p_pool->IsPooled(_p_actor);
}
//////////////////////////////////////////////////////////////////////////

void U_object_pool_manager::get_object_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count)
{
	for (auto& Elem : m_object_pool) {
		Elem.Value->get_count(_i_out_create_count, _i_out_stack_count, _i_out_given_count);
	}
}

void U_object_pool_manager::get_uobject_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count)
{
	for (auto& Elem : m_uobject_pool) {
		Elem.Value->get_count(_i_out_create_count, _i_out_stack_count, _i_out_given_count);
	}
}

void U_object_pool_manager::get_actor_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count)
{
	for (auto& Elem : m_actor_pool) {
		Elem.Value->get_count(_i_out_create_count, _i_out_stack_count, _i_out_given_count);
	}
}