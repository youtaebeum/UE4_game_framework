// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "gamecore_minimal.h"

//////////////////////////////////////////////////////////////////////////
template<typename TClass>
class F_singleton
{
protected:
	static TClass* _pInstance;

public:  
	static void _make_instance();
	static void _remove_instance();
	static TClass* _get_instance();
	static bool    _has_Instance();

public:
	virtual void _initialize() = 0;
	virtual void _reset() = 0;
	virtual void _tick(float _f_delta_seconds) = 0;

protected:
	AActor* pManagerActor = nullptr;

public:
	FORCEINLINE AActor* _get_manager_actor();

public:
	template<typename FT> void _spawn_manager_actor(class UWorld* _pWorld);
	void _DepawnMangerActor();
	void _AttachToManagerActor(AActor* pChild);
	void _DetachToManagerActor(AActor* pChild);
};

template<typename TClass>
TClass* F_singleton<TClass>::_pInstance = nullptr;

#include "Singleton.inl"