// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "game_minimal.h"

#include "Engine/GameInstance.h"
#include "Ticker.h"

#include "game_instance.generated.h"

UCLASS()
class GAME_API Ugame_instance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
	virtual void Shutdown() override;
	
private:
	FDelegateHandle  m_tick_delegate_handle;

private:
	bool tick(float _f_delta_seconds);
};
