// Fill out your copyright notice in the Description page of Project Settings.

#include "unit_movement_component.h"

#include "GameFramework/PhysicsVolume.h"

void U_unit_movement_component::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime) == true)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PawnOwner || !UpdatedComponent)
		return;

	FVector v_input_delta = compute_input_delta(DeltaTime);
	ConsumeInputVector();

	if (v_input_delta.IsNearlyZero() == false)
	{
		const FVector v_add = v_input_delta.GetSafeNormal() * m_f_move_acceleration * DeltaTime;
		m_v_acceleration += v_add;
		m_v_acceleration = (m_v_acceleration.Size() > m_f_max_move_speed) ? m_v_acceleration.GetSafeNormal() * m_f_max_move_speed : m_v_acceleration;
	}
	else
	{
		const FVector v_remove = m_v_acceleration.GetSafeNormal() * (-m_f_move_break) * DeltaTime;
		m_v_acceleration = (m_v_acceleration.Size() > v_remove.Size()) ? (m_v_acceleration + v_remove) : (FVector::ZeroVector);
	}

	FVector v_velocity = m_v_acceleration * DeltaTime;
	const FVector _v_gravity(0.f, 0.f, GetGravityZ());
	v_velocity = new_fall_velocity(v_velocity, _v_gravity, DeltaTime);

	//바닥체크 (air상태인지 체크 하기 위함)

	//Additive Move (Velocity에 영향이 없는 이동 값에 대한 처리 (스킬 이동이나 밀림 등, RootMotion은 어떻게 처리 방법 고민 해야함)
	FVector v_additive_delta = FVector::ZeroVector;
	{

	}
	
	FVector v_total_delta = v_velocity + v_additive_delta;
	if (v_total_delta.IsNearlyZero() == false) {
		FHitResult     hit(1.f);
		const FRotator r_old_rotation = UpdatedComponent->GetComponentRotation();
		FRotator       r_new_rotation = r_old_rotation;

		if (m_b_orient_rotation_to_movement && m_v_acceleration.IsNearlyZero() == false) {
			r_new_rotation = compute_orient_to_movemenet_rotation(r_old_rotation, DeltaTime, m_v_acceleration);
		}

		SafeMoveUpdatedComponent(v_total_delta, r_new_rotation, true, hit);

		if (hit.IsValidBlockingHit()) {
			HandleImpact(hit, DeltaTime, v_total_delta);
			SlideAlongSurface(v_total_delta, 1.f - hit.Time, hit.Normal, hit, true);	// Try to slide the remaining distance along the surface.
		}

		// Update velocity
		//FVector vComponentVelocity = (UpdatedComponent->GetComponentLocation()-vOldLocation) / DeltaTime;	//SafeMoveUpdatedComponent()에 의해서 의도하지 않은 위치이동 처리가 Velocity에 영향을 줄 수 있다.
	}

	Velocity = v_velocity;
	UpdateComponentVelocity();
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

FVector U_unit_movement_component::compute_input_delta(float _f_delta_time)
{
	FVector v_input_delta = FVector::ZeroVector;
	const FVector v_input_vector = GetPendingInputVector();
	if (v_input_vector.IsNearlyZero() == false) {
		v_input_delta = v_input_vector.GetSafeNormal() * m_f_max_move_speed * _f_delta_time;
	}

	return v_input_delta;
}

FVector U_unit_movement_component::compute_acceleration(const FVector& _v_input_delta, float _f_delta_time)
{
	FVector v_result = Velocity;
	if (_v_input_delta.IsNearlyZero() == false)
	{
		const FVector v_add = _v_input_delta.GetSafeNormal() * m_f_move_acceleration * _f_delta_time;
		v_result += v_add;
		v_result = (Velocity.Size() > m_f_max_move_speed) ? v_result.GetSafeNormal() * m_f_max_move_speed : v_result;
	}
	else
	{
		const FVector v_remove = v_result.GetSafeNormal() * (-m_f_move_break) * _f_delta_time;
		v_result = (v_result.Size() > v_remove.Size()) ? (v_result + v_remove) : (FVector::ZeroVector);
	}

	return v_result;
}

FVector U_unit_movement_component::new_fall_velocity(const FVector& _v_velocity, const FVector& _v_gravity, float _f_delta_time) const
{
	FVector _v_result = _v_velocity;

	if (!_v_gravity.IsZero())
	{
		// Apply gravity.
		_v_result += _v_gravity * _f_delta_time;

		const FVector _v_gravity_dir = _v_gravity.GetSafeNormal();
		const float _terminal_limit = FMath::Abs(GetPhysicsVolume()->TerminalVelocity);

		// Don't exceed terminal velocity.
		if ((_v_result | _v_gravity_dir) > _terminal_limit)
		{
			_v_result = FVector::PointPlaneProject(_v_result, FVector::ZeroVector, _v_gravity_dir) + _v_gravity_dir * _terminal_limit;
		}
	}

	return _v_result;
}