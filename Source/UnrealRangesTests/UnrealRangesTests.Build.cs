// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealRangesTests : ModuleRules
{
    public UnrealRangesTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "UnrealRanges"
            }
            );
    }
}
