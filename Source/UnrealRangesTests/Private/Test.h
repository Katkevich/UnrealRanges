#pragma once
#include "Misc/AutomationTest.h"
#include <initializer_list>

#define UR_TEST(Plugin, TestGroup, Test) \
    IMPLEMENT_SIMPLE_AUTOMATION_TEST(F##Plugin##_##TestGroup##Test, #Plugin "." #TestGroup "." #Test, EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter) \
    bool F##Plugin##_##TestGroup##Test::RunTest(const FString& Parameters)
