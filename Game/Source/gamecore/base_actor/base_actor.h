// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"
#include "GameFramework/Actor.h"
#include "base_actor.generated.h"

UCLASS()
class GAMECORE_API A_base_actor : public AActor
{
	GENERATED_BODY()
	
public:	
	A_base_actor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(Category = Unit, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	uint32 m_ui_uniq_index = 0;

public:
	virtual void _initialize(uint32 _ui_uniq_index);
	virtual void _reset();

	FORCEINLINE uint32 get_uniq_index() { return m_ui_uniq_index; }

	//~ Begin UObject Interface
	// virtual void PostInitProperties() override;	인스턴스별 디자이너 설정 프로퍼티를 지원하기 위해, 주어진 오브젝트에 대한 인스턴스 데이터에서 값을 로드하기도 합니다
	// virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override; 에디터에서 프로퍼티 변경시 호출
};
