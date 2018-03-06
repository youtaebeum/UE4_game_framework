// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCore_Utility.h"
#include "gamecore_manager.h"

namespace GC_UTILTY
{
	AActor* SpawnActor(UClass* pClass, FVector vLocation, FRotator rRotation, FString sLabelName, bool bNeedRootComponent)
	{
		GC_CHECK(U_gamecore_manager::_get_instance() != nullptr);
		GC_CHECK(U_gamecore_manager::_get_instance()->get_game_instance() != nullptr);

		FActorSpawnParameters SpawnInfo;	
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* pNewActor = U_gamecore_manager::_get_instance()->get_game_instance()->GetWorld()->SpawnActor(pClass, &vLocation, &rRotation, SpawnInfo);
		GC_CHECK(pNewActor != nullptr);

#if WITH_EDITOR
		if (sLabelName.IsEmpty() == false)
			pNewActor->SetActorLabel(sLabelName);
#endif

		if ((bNeedRootComponent == true) && (pNewActor->GetRootComponent() == nullptr)) {
			USceneComponent* pRootComponent = NewObject<USceneComponent>(pNewActor, USceneComponent::GetDefaultSceneRootVariableName(), RF_Transactional);
			pRootComponent->Mobility = EComponentMobility::Movable;
#if WITH_EDITORONLY_DATA
			pRootComponent->bVisualizeComponent = false;
#endif

			pNewActor->SetRootComponent(pRootComponent);
			pNewActor->AddInstanceComponent(pRootComponent);

			pRootComponent->RegisterComponent();
		}

		return pNewActor;
	}
	
	void DespawnActor(AActor* _p_actor)
	{
		GC_CHECK(U_gamecore_manager::_get_instance() != nullptr);
		GC_CHECK(U_gamecore_manager::_get_instance()->get_game_instance() != nullptr);

		gGameCore->get_game_instance()->GetWorld()->DestroyActor(_p_actor);
	}

	void DetachParentActor(AActor* _p_actor)
	{
		GC_CHECK(_p_actor != nullptr);
		_p_actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	void DetachAllChildActors(AActor* _p_actor)
	{
		GC_CHECK(_p_actor != nullptr);
		TArray<class AActor*> Children;
		_p_actor->GetAttachedActors(Children);
		for (int32 Index = 0; Index < Children.Num(); ++Index) {
			if (Children[Index] != nullptr)
				Children[Index]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}

	int32 GetTickCount()
	{
		double d = (FPlatformTime::Seconds() * 1000.0);
		uint64 ui64 = (uint64)d;
		int32 i32 = *(int32*)&ui64;
		return i32;
	}
}