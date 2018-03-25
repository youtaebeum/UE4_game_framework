// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"

#include "GameFramework/PawnMovementComponent.h"

#include "unit_movement_component.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class GAMECORE_API U_unit_movement_component : public UPawnMovementComponent
{
	GENERATED_BODY()

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	UPROPERTY(Category = UNxnExt_MovementComponent, EditAnywhere, BlueprintReadOnly)
	float m_f_move_acceleration = 900.0f;

	UPROPERTY(Category = UNxnExt_MovementComponent, EditAnywhere, BlueprintReadOnly)
	float m_f_move_break = 900.0f;

	UPROPERTY(Category = "unit_movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float m_f_max_move_speed = 400.0f;

	UPROPERTY(Category = "unit_movement", EditAnywhere, BlueprintReadOnly)
	bool m_b_orient_rotation_to_movement = true;

	UPROPERTY(Category = "unit_movement", EditAnywhere, BlueprintReadOnly)
	FRotator m_r_rotation_rate = FRotator(0, 720.0f, 0);

protected:
	UPROPERTY()	FVector m_v_acceleration; // moved delta

public:
	FVector get_acceleration();

private:
	FRotator compute_orient_to_movemenet_rotation(const FRotator& _r_rotation, float _f_delta_time, const FVector& _v_delta_vector);

	FVector  compute_input_delta(float _f_delta_time);
	FVector  compute_velocity(const FVector& _v_input_delta, float _f_delta_time);
	FVector  new_fall_velocity(const FVector& _v_velocity, const FVector& _v_gravity, float _f_delta_time) const;
};
