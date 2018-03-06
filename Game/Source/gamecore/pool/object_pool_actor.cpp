// Fill out your copyright notice in the Description page of Project Settings.

#include "object_pool_actor.h"
#include "object_pool_manager.h"

// Sets default values
A_object_pool_actor::A_object_pool_actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void A_object_pool_actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void A_object_pool_actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if WITH_EDITOR
	if (U_object_pool_manager::_get_instance())
	{
		iObject_CreateCount = iObject_StackCount = iObject_GivenCount = 0;
		iUObject_CreateCount = iUObject_StackCount = iUObject_GivenCount = 0;
		iActor_CreateCount = iActor_StackCount = iActor_GivenCount = 0;

		U_object_pool_manager::_get_instance()->get_object_count(iObject_CreateCount, iObject_StackCount, iObject_GivenCount);
		U_object_pool_manager::_get_instance()->get_uobject_count(iUObject_CreateCount, iUObject_StackCount, iUObject_GivenCount);
		U_object_pool_manager::_get_instance()->get_actor_count(iActor_CreateCount, iActor_StackCount, iActor_GivenCount);
	}
#endif
}

