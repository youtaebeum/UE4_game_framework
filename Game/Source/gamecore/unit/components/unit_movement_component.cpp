// Fill out your copyright notice in the Description page of Project Settings.

#include "unit_movement_component.h"

void U_unit_movement_component::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime) == true)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PawnOwner || !UpdatedComponent)
		return;

	FVector v_input_delta = FVector::ZeroVector;
	const FVector v_input_vector = GetPendingInputVector();
	if (v_input_vector.IsNearlyZero() == false) {
		v_input_delta = v_input_vector.GetSafeNormal() * m_f_max_move_speed * DeltaTime;
	}
	ConsumeInputVector();

	//////////////////////////////////////////////////////////////////////////
	//Compute Velocity
	FVector v_moved_delta = v_input_delta;
	if (v_moved_delta.IsNearlyZero() == false) 
	{
		const FVector v_add_velocity = v_moved_delta.GetSafeNormal() * m_f_move_acceleration * DeltaTime;
		Velocity += v_add_velocity;
		Velocity = (Velocity.Size() > m_f_max_move_speed) ? Velocity.GetSafeNormal() * m_f_max_move_speed : Velocity;
	}
	else 
	{
		const FVector vStopVelocity = Velocity.GetSafeNormal() * (-m_f_move_break) * DeltaTime;
		Velocity = (Velocity.Size() > vStopVelocity.Size()) ? (Velocity + vStopVelocity) : (FVector::ZeroVector);
	}

	v_moved_delta = Velocity * DeltaTime;

	//////////////////////////////////////////////////////////////////////////
	//Additive Move (Velocity에 영향이 없는 이동 값에 대한 처리 (스킬 이동이나 밀림 등, RootMotion은 어떻게 처리 방법 고민 해야함)
	FVector v_additive_delta = FVector::ZeroVector;
	{

	}
	
	FVector v_total_delta = v_moved_delta + v_additive_delta;

	if (v_total_delta.IsNearlyZero() == false) {
		FHitResult     hit(1.f);
		const FRotator r_old_rotation = UpdatedComponent->GetComponentRotation();
		FRotator       r_new_rotation = r_old_rotation;

		if (m_b_orient_rotation_to_movement && v_moved_delta.IsNearlyZero() == false) {
			r_new_rotation = compute_orient_to_movemenet_rotation(r_old_rotation, DeltaTime, v_moved_delta);
		}

		SafeMoveUpdatedComponent(v_total_delta, r_new_rotation, true, hit);

		if (hit.IsValidBlockingHit()) {
			HandleImpact(hit, DeltaTime, v_total_delta);
			SlideAlongSurface(v_total_delta, 1.f - hit.Time, hit.Normal, hit, true);	// Try to slide the remaining distance along the surface.
		}

		// Update velocity
		//FVector vComponentVelocity = (UpdatedComponent->GetComponentLocation()-vOldLocation) / DeltaTime;	//SafeMoveUpdatedComponent()에 의해서 의도하지 않은 위치이동 처리가 Velocity에 영향을 줄 수 있다.
		FVector v_component_velocity = v_total_delta / DeltaTime;

		if (UpdatedComponent) {
			UpdatedComponent->ComponentVelocity = v_component_velocity;
		}
	}
}

FRotator U_unit_movement_component::compute_orient_to_movemenet_rotation(const FRotator& _r_rotation, float _f_delta_time, const FVector& _v_delta_vector)
{
	const FRotator r_desired = _v_delta_vector.Rotation();
	FRotator	   r_delta = GC_UTILTY::get_delta_rotation(m_r_rotation_rate, _f_delta_time);
	FRotator	   r_ret   = _r_rotation;

	if (!_r_rotation.Equals(r_desired)) {
		// PITCH
		if (!FMath::IsNearlyEqual(_r_rotation.Pitch, r_desired.Pitch)) {
			r_ret.Pitch = FMath::FixedTurn(_r_rotation.Pitch, r_desired.Pitch, r_delta.Pitch);
		}

		// YAW
		if (!FMath::IsNearlyEqual(_r_rotation.Yaw, r_desired.Yaw)) {
			r_ret.Yaw = FMath::FixedTurn(_r_rotation.Yaw, r_desired.Yaw, r_delta.Yaw);
		}

		// ROLL
		if (!FMath::IsNearlyEqual(_r_rotation.Roll, r_desired.Roll)) {
			r_ret.Roll = FMath::FixedTurn(_r_rotation.Roll, r_desired.Roll, r_delta.Roll);
		}
	}

	return r_ret;
}
