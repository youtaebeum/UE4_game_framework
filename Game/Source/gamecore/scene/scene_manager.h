// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "scene_manager.generated.h"

UCLASS()
class GAMECORE_API U_scene_manager : public UObject, public F_singleton<U_scene_manager>
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_seconds);

private:
	//void change_scene();

public:

private:
};
