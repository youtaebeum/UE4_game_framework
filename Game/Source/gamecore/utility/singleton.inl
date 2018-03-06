
#include "GameCore_Utility.h"

template<typename TClass>
void F_singleton<TClass>::_make_instance()
{
	GC_CHECK(_pInstance == nullptr);
	_pInstance = GC_NewObject<TClass>();
	_pInstance->AddToRoot();
};

template<typename TClass>
void F_singleton<TClass>::_remove_instance()
{
	GC_CHECK(_pInstance != nullptr);

	_pInstance->_DepawnMangerActor();
	_pInstance->_reset();
	_pInstance->RemoveFromRoot();

	GC_DeleteObject(_pInstance);
	_pInstance = nullptr;
};

template<typename TClass>
TClass* F_singleton<TClass>::_get_instance()
{
	GC_CHECK(_pInstance != nullptr);
	return _pInstance;
};

template<typename TClass>
bool F_singleton<TClass>::_has_Instance()
{
	return ((_pInstance != nullptr) ? true : false);
};

template<typename TClass>
AActor* F_singleton<TClass>::_get_manager_actor()
{
	return pManagerActor;
}

template<typename TClass>
template<typename FT>
void F_singleton<TClass>::_spawn_manager_actor(class UWorld* _pWorld)
{
#ifdef WITH_EDITOR
	FString strName = FString::Printf(TEXT("%d_%s"), 1, *(TClass::StaticClass()->GetName()));
	pManagerActor = GC_UTILTY::SpawnActor(FT::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, strName, true);
#endif
}

template<typename TClass>
void F_singleton<TClass>::_DepawnMangerActor()
{
#ifdef WITH_EDITOR
	if (pManagerActor) {
		GC_UTILTY::DespawnActor(pManagerActor);
	}
#endif
}

template<typename TClass>
void F_singleton<TClass>::_AttachToManagerActor(AActor* pChild)
{
#ifdef WITH_EDITOR
	if (pManagerActor) {
		pChild->GetRootComponent()->AttachToComponent(pManagerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
#endif
}

template<typename TClass>
void F_singleton<TClass>::_DetachToManagerActor(AActor* pChild)
{
#ifdef WITH_EDITOR
	pChild->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
#endif
}
