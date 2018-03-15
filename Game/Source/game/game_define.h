// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "game_minimal.h"

UENUM(BlueprintType)
enum class E_game_scene_type : uint8
{
	login = 0	UMETA(DisplayName = "login"),
	battle		UMETA(DisplayName = "battle"),
};
