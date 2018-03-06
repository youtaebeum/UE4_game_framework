// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"

#include "GameFramework/Pawn.h"
#include "base_unit.generated.h"

REGIIST_OBJECTPOOLTYPE(A_base_unit, OBJECT_POOL_DEFAULT_SIZE)

UCLASS()
class GAMECORE_API A_base_unit : public APawn
{
	GENERATED_BODY()
	
public:	
	A_base_unit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(Category = base_unit, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	uint32 m_ui_uniq_index = 0;

	UPROPERTY(Category = base_unit, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;

public:
	virtual void _initialize(uint32 _ui_uniq_index);
	virtual void _reset();

	//////////////////////////////////////////////////////////////////////////
	//tempcode
	int CharacterSKIndex = 0;
	TArray<FString> _Assets;
	void Loaded();

	e_rsource_loading_type eResourceLoadType;

	void LoadComplite(const FStringAssetReference& _AssetRef, UClass* _p_class);
	void LoadFail(const FStringAssetReference& _AssetRef, UClass* _p_class);
	//////////////////////////////////////////////////////////////////////////

	//~ Begin UObject Interface
	// virtual void PostInitProperties() override;	인스턴스별 디자이너 설정 프로퍼티를 지원하기 위해, 주어진 오브젝트에 대한 인스턴스 데이터에서 값을 로드하기도 합니다
	// virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override; 에디터에서 프로퍼티 변경시 호출
};
