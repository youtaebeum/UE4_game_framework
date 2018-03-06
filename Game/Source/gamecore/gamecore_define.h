// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(GameCoreLog, Log, All);
GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(GameCoreWarning, Warning, All);
GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(GameCoreError, Error, All);

#define GC_LOG(InFormat, ...)     UE_LOG(GameCoreLog, Log, InFormat, ##__VA_ARGS__);
#define	GC_WARNING(InFormat, ...) UE_LOG(GameCoreWarning, Warning, InFormat, ##__VA_ARGS__);
#define	GC_ERROR(InFormat, ...)   UE_LOG(GameCoreError, Warning, InFormat, ##__VA_ARGS__);

#define GC_CHECK(expr) check(expr);

#define	GC_New new
#define	GC_Delete delete

#define	GC_NewObject NewObject
#define	GC_DeleteObject(Object){					    \
			GC_CHECK(Object != nullptr);				\
			(Object->ConditionalBeginDestroy());		\
		}

#define GC_INDEX_NONE 0 

#define OBJECT_POOL_DEFAULT_SIZE 10

template<typename T> struct F_object_pool_type {
	static FName _get_object_name() { GC_CHECK(false); return FName("None"); }
	enum { Value = 0, Capacity = 1 };
};

#define REGIIST_OBJECTPOOLTYPE(_ClassName, _Capacity) class _ClassName; template<> struct F_object_pool_type<_ClassName> { static FName _get_object_name(){ return FName(#_ClassName); }  enum { Value = 1, Capacity = _Capacity }; };


#pragma region DelegateDefine
DECLARE_DELEGATE_TwoParams(delegate_resource_load_complete, const FStringAssetReference&, UClass*);
DECLARE_DELEGATE_TwoParams(delegate_resource_load_fail, const FStringAssetReference&, UClass*);
#pragma endregion

