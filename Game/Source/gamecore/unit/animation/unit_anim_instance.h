// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"

#include "Animation/AnimInstance.h"

#include "unit_anim_instance.generated.h"

UCLASS()
class GAMECORE_API U_unit_anim_instance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void _initialize();
	virtual void _reset();
	virtual void _tick(float _f_delta_time);

private:
	UPROPERTY(Category = base_unit, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector m_v_velocity = FVector::ZeroVector;

private:
	class A_base_unit* m_p_owner_unit;

public:
	void set_velocity(FVector _velocity);
};
