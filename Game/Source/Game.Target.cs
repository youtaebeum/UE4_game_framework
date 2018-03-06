// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameTarget : TargetRules
{
	public GameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "game" } );
        ExtraModuleNames.AddRange(new string[] { "gamecore" });
    } 
}
