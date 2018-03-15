// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "scene/base_scene.h"
#include "login_scene.generated.h"

UCLASS()
class U_login_scene : public U_base_scene
{
	GENERATED_BODY()

private:
	virtual void enter() override;
	virtual void update(float _f_delta_time) override;
	virtual void exit() override;

private:
	void input_event_test();
};
