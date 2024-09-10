#include "UnrealRangesTests.h"
#include "Algo/Find.h"
#include "Algo/FindSequence.h"

struct Foo
{
    bool operator==(const Foo& X) const
    {
        return false;
    }
};

void FUnrealRangesTestsModule::StartupModule()
{
    TArray<Foo> Arr;
    //Algo::FindBy(Arr, Foo{});
}

void FUnrealRangesTestsModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FUnrealRangesTestsModule, UnrealRangesTests)