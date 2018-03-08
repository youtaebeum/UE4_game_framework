// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

namespace GC_UTILTY
{
	GAMECORE_API AActor* SpawnActor(UClass* pClass, FVector vLocation, FRotator rRotation, FString sLabelName = FString(), bool bNeedRootComponent = false);
	GAMECORE_API void    DespawnActor(AActor* _p_actor);

	GAMECORE_API void DetachParentActor(AActor* _p_actor);
	GAMECORE_API void DetachAllChildActors(AActor* _p_actor);

	template<typename TEnum>
	FString get_enum_to_string(const FString& _str, TEnum _enum) 
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *_str, true);
		if (!enumPtr) {
			return FString("Invalid");
		}

		return enumPtr->GetNameStringByIndex((int32)_enum);
	}

	template<typename T> T* safe_map_value(T** ppAddress){
		return  (ppAddress != nullptr) ? *ppAddress : nullptr;
	};

	template<typename T> T*const SafeMapConstValue(T*const* ppAddress){
		return  (ppAddress != nullptr) ? *ppAddress : nullptr;
	};

	GAMECORE_API int32 GetTickCount();

	GAMECORE_API float    get_axis_delta_rotation(float _f_rotation_rate, float _f_delta_time);
	GAMECORE_API FRotator get_delta_rotation(FRotator _r_rotation, float _r_delta_time);
}  