// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WarHorizon_AllBigGunServerTarget : TargetRules
{
	public WarHorizon_AllBigGunServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.Add("WarHorizon_AllBigGun");
	}
}
