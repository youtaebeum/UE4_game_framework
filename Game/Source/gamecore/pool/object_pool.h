// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "base_actor/base_actor.h"

/////////////////////////////////////////////////////////////////////////////////
template<typename TObj>
class F_object_pool
{
	friend class U_object_pool_manager;

protected:
	int32				m_i_create_count = 0;	
	int32				m_i_give_count = 0;
	TArray<TObj*>		m_stack_list;

#if WITH_EDITOR
	TArray<TObj*>		m_given_list;
#endif

public:
	template<typename FT> FT*  
	get_object(int _i_capacity_size = 1)
	{
		if (m_stack_list.Num() <= 0)
		{
			for (int i = 0; i < _i_capacity_size; ++i)
			{
				FT* p_new_obj = GC_New FT();
				m_stack_list.Add(p_new_obj);
				m_i_create_count++;
			}
		}

		FT* p_object = static_cast<FT*>(m_stack_list[0]);
		m_stack_list.RemoveAt(0);

		GC_CHECK(p_object != nullptr);
		m_i_give_count++;

#if WITH_EDITOR
		m_given_list.Add(p_object);
#endif
		return p_object;
	}

	template<typename FT>  
	void return_object(TObj* _p_object)
	{
#if WITH_EDITOR
		GC_CHECK(m_stack_list.Find(_p_object) == INDEX_NONE)
#endif

		m_i_give_count--;
		m_stack_list.Add(_p_object);

#if WITH_EDITOR
		m_given_list.Remove(_p_object);
#endif
	}
	
	void clear()
	{
#if WITH_EDITOR
		GC_CHECK(m_given_list.Num() == 0)
#endif
		if (m_given_list.Num() > 0) {
			GC_ERROR(TEXT("ObjectPool[Object] : Object has not been return(Count : %d)"), m_given_list.Num());
		}

		m_i_create_count = 0;
		m_i_give_count = 0;

		for (int Index = 0; Index < m_stack_list.Num(); ++Index) {
			GC_Delete(m_stack_list[Index]);
			m_stack_list[Index] = nullptr;
		}
		m_stack_list.Empty();

#if WITH_EDITOR
		m_given_list.Empty();
#endif
	}

	bool is_pooled(TObj* _p_object)
	{
		int32 iReturn = m_stack_list.Find(_p_object);
		if (iReturn != INDEX_NONE)
			return true;
		else
			return false;
	}

	void get_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count)
	{
		_i_out_create_count += m_i_create_count; _i_out_stack_count += m_stack_list.Num(); _i_out_given_count += m_i_give_count;
	}
};

//////////////////////////////////////////////////////////////////////////

template<>
class F_object_pool<UObject>
{
	friend class U_object_pool_manager;

protected:
	int32				m_i_create_count = 0;
	int32				m_i_give_count = 0;
	TArray<UObject*>	m_stack_list;

#if WITH_EDITOR
	TArray<UObject*>		m_given_list;
#endif

public:
	template<typename FT> 
	FT*  get_object(int _i_capacity_size = 1)
	{
		int iStackNum = m_stack_list.Num();
		if (iStackNum <= 0)
		{
			for (int i = 0; i < _i_capacity_size; ++i)
			{
				FT* p_new_object = NewObject<FT>(U_object_pool_manager::_get_instance());
				p_object->AddToRoot();
				m_stack_list.Add(p_new_object);
				m_i_create_count++;
			}
		}

		FT* p_object = static_cast<FT*>(m_stack_list[0]);
		m_stack_list.RemoveAt(0);

		GC_CHECK(p_object != nullptr);
		m_i_give_count++;

#if WITH_EDITOR
		m_given_list.Add(p_object);
#endif

		return p_object;
	}

	template<typename FT>
	void return_object(UObject* _p_object)
	{
#if WITH_EDITOR
		GC_CHECK(m_stack_list.Find(_p_object) == INDEX_NONE)
		GC_CHECK(_p_object->GetOuter() == U_object_pool_manager::_get_instance());
#endif

		m_stack_list.Add(_p_object);
		m_i_give_count--;

#if WITH_EDITOR
		m_given_list.Remove(_p_object);
#endif
	}

	void clear()
	{
#if WITH_EDITOR
		GC_CHECK(m_given_list.Num() == 0)
#endif

		if (m_given_list.Num() > 0) {
			GC_ERROR(TEXT("ObjectPool[UObject] : Object has not been return(Count : %d)"), m_given_list.Num());
		}

		m_i_create_count = 0;
		m_i_give_count = 0;

		for (int Index = 0; Index < m_stack_list.Num(); ++Index) {
			GC_DeleteObject(m_stack_list[Index]);
			m_stack_list[Index] = nullptr;
		}
		m_stack_list.Empty();

#if WITH_EDITOR
		m_given_list.Empty();
#endif
	}

	bool IsPooled(UObject* _p_object)
	{
		int32 iReturn = m_stack_list.Find(_p_object);
		if (iReturn != INDEX_NONE)
			return true;
		else
			return false;
	}

	void get_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count)
	{
		_i_out_create_count += m_i_create_count; _i_out_stack_count += m_stack_list.Num(); _i_out_given_count += m_i_give_count;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class F_object_pool<A_base_actor>
{
	friend class U_object_pool_manager;

protected:
	int32				m_i_create_count = 0;
	int32				m_i_give_count = 0;
	TArray<A_base_actor*>		m_stack_list;

#if WITH_EDITOR
	TArray<A_base_actor*>		m_given_list;
#endif

public:
	template<typename FT> 
	FT* get_object(int _i_capacity_size = 1)
	{
		if (m_stack_list.Num() <= 0)
		{
			for (int i = 0; i < _i_capacity_size; ++i)
			{
				FT* p_new_object = Cast<FT>(GC_UTILTY::SpawnActor(FT::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, "", true));
				m_stack_list.Add(p_new_object);
				m_i_create_count++;

				p_new_object->AddToRoot();
				p_new_object->bHidden = true;
#if WITH_EDITOR
				if (U_object_pool_manager::_get_instance()->_get_manager_actor()) {
					p_new_object->GetRootComponent()->AttachToComponent(U_object_pool_manager::_get_instance()->_get_manager_actor()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				}
#endif
			}
		}

		FT* p_object = static_cast<FT*>(m_stack_list[0]);
		m_stack_list.RemoveAt(0);

		GC_CHECK(p_object != nullptr);
		m_i_give_count++;

		GC_UTILTY::DetachAllChildActors(p_object);
		GC_UTILTY::DetachParentActor(p_object);
		p_object->RegisterAllComponents();
		p_object->AddToRoot();
		p_object->bHidden = false;

#if WITH_EDITOR
		m_given_list.Add(p_object);
#endif

		return p_object;
	}

	template<typename FT>
	void return_object(A_base_actor* _p_object)
	{
#if WITH_EDITOR
		GC_CHECK(m_stack_list.Find(_p_object) == INDEX_NONE)
#endif

		GC_UTILTY::DetachAllChildActors(_p_object);
		GC_UTILTY::DetachParentActor(_p_object);
		_p_object->UnregisterAllComponents();
		_p_object->AddToRoot();
		_p_object->bHidden = true;

		m_stack_list.Add(_p_object);
		m_i_give_count--;

#if WITH_EDITOR
		m_given_list.Remove(_p_object);
		if (U_object_pool_manager::_get_instance()->_get_manager_actor()) 
		{
			_p_object->GetRootComponent()->AttachToComponent(U_object_pool_manager::_get_instance()->_get_manager_actor()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
#endif
	}

	void clear()
	{
#if WITH_EDITOR
		GC_CHECK(m_given_list.Num() == 0)
#endif
		if (m_given_list.Num() > 0) {
			GC_ERROR(TEXT("ObjectPool[UObject] : Object has not been return(Count : %d)"), m_given_list.Num());
		}

		m_i_create_count = 0;
		m_i_give_count = 0;

		for (int Index = 0; Index < m_stack_list.Num(); ++Index) {
			m_stack_list[Index]->RemoveFromRoot();
			GC_UTILTY::DetachParentActor(m_stack_list[Index]);
			m_stack_list[Index] = nullptr;
		}
		m_stack_list.Empty();

#if WITH_EDITOR
		m_given_list.Empty();
#endif
	}

	bool IsPooled(A_base_actor* p_object)
	{
		int32 iReturn = m_stack_list.Find(p_object);
		if (iReturn != INDEX_NONE)
			return true;
		else
			return false;
	}

	void get_count(uint16& _i_out_create_count, uint16& _i_out_stack_count, uint16& _i_out_given_count)
	{
		_i_out_create_count += m_i_create_count; _i_out_stack_count += m_stack_list.Num(); _i_out_given_count += m_i_give_count;
	}
};