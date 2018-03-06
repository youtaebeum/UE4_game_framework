// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "GameFramework/Actor.h"
#include "object_pool_actor.generated.h"

UCLASS()
class GAMECORE_API A_object_pool_actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	A_object_pool_actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iObject_CreateCount = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iObject_StackCount = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iObject_GivenCount = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iUObject_StackCount = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iUObject_CreateCount = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iUObject_GivenCount = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iActor_CreateCount = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iActor_StackCount = 0;
	UPROPERTY(VisibleInstanceOnly, Category = "PoolInfo")	uint16 iActor_GivenCount = 0;
};
