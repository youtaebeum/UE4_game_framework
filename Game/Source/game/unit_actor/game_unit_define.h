// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "gamecore_minimal.h"

UENUM(BlueprintType)
enum class E_unit_parts_type : uint8
{
	hair = 0    UMETA(DisplayName = "hair"),
	head		UMETA(DisplayName = "head"),
	upper		UMETA(DisplayName = "upper"),
	lower		UMETA(DisplayName = "lower"),
	glove		UMETA(DisplayName = "head"),
	shoes		UMETA(DisplayName = "head")
};
