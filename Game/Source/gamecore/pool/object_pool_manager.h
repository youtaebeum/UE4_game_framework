// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "object_pool.h"
#include "object_pool_actor.h"
#include "base_actor/base_actor.h"

#include "object_pool_manager.generated.h"

/////////////////////////////////////////////////////////////////
// example : 클래스 상단 
// param1 : 클래스 이름
// param2 : 한번에 생성될 크기(개수)
// REGIIST_OBJECTPOOLTYPE(A_base_actor, 10)
/////////////////////////////////////////////////////////////////

UCLASS()
class GAMECORE_API U_object_pool_manager : public UObject, public F_singleton<U_object_pool_manager>
{
	GENERATED_BODY()

	friend class A_object_pool_actor;

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

private:
	TMap<FName, F_object_pool<void>*>			 m_object_pool;
	TMap<UClass*, F_object_pool<UObject>*>		 m_uobject_pool;
	TMap<UClass*, F_object_pool<A_base_actor>*>	 m_actor_pool;

	template<typename T> F_object_pool<void>*		    get_object_pool();
	template<typename T> F_object_pool<UObject>*	    get_uobject_pool();
	template<typename T> F_object_pool<A_base_actor>*   get_actor_pool();

	F_object_pool<UObject>*	       get_uobject_pool(UClass* pClass);
	F_object_pool<A_base_actor>*   get_actor_pool(UClass* pClass);

public:
	template<typename T> T*   get_object();
	template<typename T> void return_object(T* _p_ptr);
	template<typename T> bool is_pooled_object(T* _p_ptr);

	template<typename T> T* get_actor();
	void return_actor(A_base_actor* _p_actor);
	bool is_pooled_actor(A_base_actor* _p_actor);

	template<typename T> T* get_uobject();
	void return_uobject(UObject* _p_uobject);
	bool is_pooled_uobject(UObject* _p_uobject);

	void get_object_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count);
	void get_uobject_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count);
	void get_actor_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count);
};

//////////////////////////////////////////////////////////////////////////
template<typename T> F_object_pool<void>*   U_object_pool_manager::get_object_pool()
{
	static_assert(F_object_pool_type<T>::Value == 1, "You cannot use m_object_pool with not regist classes.");

	FName str_object_name = F_object_pool_type<T>::_get_object_name();
	GC_CHECK(str_object_name.IsNone() == false);

	F_object_pool<void>* p_pool = GC_UTILTY::safe_map_value< F_object_pool<void> >(_get_instance()->m_object_pool.Find(str_object_name));
	if (p_pool == nullptr) {
		p_pool = GC_New F_object_pool<void>();
		_get_instance()->m_object_pool.Add(str_object_name, p_pool);
	}
	return p_pool;
}

template<typename T>
F_object_pool<UObject>* U_object_pool_manager::get_uobject_pool()
{
	static_assert(TPointerIsConvertibleFromTo<T, const UObject>::Value == 1, "You cannot use m_uobject_pool with non UObject classes.");
	static_assert(F_object_pool_type<T>::Value == 1, "not regist classes.");

	UClass* p_uclass = T::StaticClass();
	GC_CHECK(p_uclass != nullptr);

	F_object_pool<UObject>* p_pool = GC_UTILTY::safe_map_value< F_object_pool<UObject> >(_get_instance()->m_uobject_pool.Find(p_uclass));
	if (p_pool == nullptr)
	{
		p_pool = GC_New F_object_pool<UObject>();
		_get_instance()->m_uobject_pool.Add(p_uclass, p_pool);
	}

	return p_pool;
}

template<typename T> 
F_object_pool<A_base_actor>*  U_object_pool_manager::get_actor_pool()
{
	static_assert(TPointerIsConvertibleFromTo<T, const A_base_actor>::Value == 1, "You cannot use m_actor_pool with non A_base_actor classes.");
	static_assert(F_object_pool_type<T>::Value == 1, "not regist classes.");

	UClass* p_uclass = T::StaticClass();
	GC_CHECK(p_uclass != nullptr);

	F_object_pool<A_base_actor>* p_pool = GC_UTILTY::safe_map_value< F_object_pool<A_base_actor> >(_get_instance()->m_actor_pool.Find(p_uclass));
	if (p_pool == nullptr)
	{
		p_pool = GC_New F_object_pool<A_base_actor>();
		_get_instance()->m_actor_pool.Add(p_uclass, p_pool);
	}

	return p_pool;
}

template<typename T>
T* U_object_pool_manager::get_object()
{
	static_assert(F_object_pool_type<T>::Value == 1, "You cannot use m_object_pool with not regist classes.");

	F_object_pool<void>* p_pool = _get_instance()->get_object_pool<T>();
	GC_CHECK(p_pool != nullptr);

	return p_pool->get_object<T>(F_object_pool_type<T>::Capacity);
}

template<typename T> 
T* U_object_pool_manager::get_actor()
{
	static_assert(TPointerIsConvertibleFromTo<T, const A_base_actor>::Value == 1, "You cannot use m_actor_pool with non A_base_actor classes.");
	static_assert(F_object_pool_type<T>::Value == 1, "not regist classes.");

	F_object_pool<A_base_actor>* p_pool = _get_instance()->get_actor_pool<T>();
	GC_CHECK(p_pool != nullptr);

	return p_pool->get_object<T>(F_object_pool_type<T>::Capacity);
}

template<typename T> 
T* U_object_pool_manager::get_uobject()
{
	static_assert(TPointerIsConvertibleFromTo<T, const UObject>::Value == 1, "You cannot use m_uobject_pool with non UObject classes.");
	static_assert(F_object_pool_type<T>::Value == 1, "not regist classes.");

	F_object_pool<UObject>* p_pool = _get_instance()->get_uobject_pool<T>();
	GC_CHECK(p_pool != nullptr);

	T* pObject = p_pool->get_object<T>(F_object_pool_type<T>::Capacity);

	return pObject;
}

template<typename T> 
void U_object_pool_manager::return_object(T* _p_ptr)
{
	static_assert(F_object_pool_type<T>::Value == 1, "You cannot use m_object_pool with not regist classes.");
	GC_CHECK(_p_ptr != nullptr);

	F_object_pool<void>* p_pool = _get_instance()->get_object_pool<T>();
	GC_CHECK(p_pool != nullptr);

	p_pool->return_object<T>(_p_ptr);
}

template<typename T> 
bool U_object_pool_manager::is_pooled_object(T* _p_ptr)
{
	static_assert(F_object_pool_type<T>::Value == 1, "You cannot use m_object_pool with not regist classes.");
	GC_CHECK(_p_ptr != nullptr);

	F_object_pool<void>* p_pool = _get_instance()->get_object_pool<T>();
	GC_CHECK(p_pool != nullptr);

	return p_pool->IsPooled(_p_ptr);
}

/*
template<typename T>
bool U_object_pool_manager::is_pooled_actor(T* _p_actor)
{
	static_assert(TPointerIsConvertibleFromTo<T, const A_base_actor>::Value == 1, "You cannot use m_actor_pool with non A_base_actor classes.");
	static_assert(F_object_pool_type<T>::Value == 1, "not regist classes.");

	GC_CHECK(_p_actor != nullptr);

	F_object_pool<A_base_actor>* p_pool = _get_instance()->get_actor_pool<T>();
	GC_CHECK(p_pool != nullptr);

	return p_pool->IsPooled(_p_actor);
}

template<typename T> 
bool U_object_pool_manager::is_pooled_uobject(T* pObject)
{
	static_assert(TPointerIsConvertibleFromTo<T, const UObject>::Value == 1, "You cannot use m_uobject_pool with non UObject classes.");
	static_assert(F_object_pool_type<T>::Value == 1, "not regist classes.");
	GC_CHECK(pObject != nullptr);

	F_object_pool<UObject>* p_pool = _get_instance()->get_uobject_pool<T>();
	GC_CHECK(p_pool != nullptr);

	return p_pool->IsPooled(pObject);
}*/