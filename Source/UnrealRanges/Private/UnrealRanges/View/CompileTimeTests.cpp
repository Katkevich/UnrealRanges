#include "UnrealRanges/View/All.h"
#include "Containers/Array.h"
#include "Containers/Set.h"
#include "Containers/Map.h"
#include "Containers/List.h"
#include "Containers/StaticArray.h"
#include "Containers/BitArray.h"
#include "Containers/ChunkedArray.h"
#include "Containers/Deque.h"
#include "Containers/IndirectArray.h"
#include "Containers/SparseArray.h"
#include "Containers/StridedView.h"
#include "Containers/UnrealString.h"
#include "Containers/ArrayView.h"
#include "Containers/StringView.h"

namespace Ur::View::Test {

    template<typename TContainer, typename TExpectedReference, typename TExpectedConstReference>
    void CheckReferenceTypes()
    {
        TContainer Container;

        Ur::View::TRefView<TContainer> View = Ur::View::Ref(Container);
        const Ur::View::TRefView<TContainer> ConstView = Ur::View::Ref(Container);
        Ur::View::TRefView<const TContainer> DeepConstView = Ur::View::Cref(Container);

        static_assert(std::is_same_v<decltype(*View.begin()), TExpectedReference>, "non const view reference don't match");
        static_assert(std::is_same_v<decltype(*ConstView.begin()), TExpectedConstReference>, "const view reference don't match");
        static_assert(std::is_same_v<decltype(*DeepConstView.begin()), TExpectedConstReference>, "deep const view reference don't match");

        static_assert(std::is_same_v<decltype(*View.cbegin()), TExpectedConstReference>, "non const view explicit const reference don't match");
        static_assert(std::is_same_v<decltype(*ConstView.cbegin()), TExpectedConstReference>, "const view explicit const reference don't match");
        static_assert(std::is_same_v<decltype(*DeepConstView.cbegin()), TExpectedConstReference>, "deep const view explicit const reference don't match");
    }

    static void UnrealEngineContainers()
    {
        CheckReferenceTypes<TArray<FString>, FString&, const FString&>();
        CheckReferenceTypes<TSet<FString>, FString&, const FString&>();
        CheckReferenceTypes<TMap<FString, FString>, TPair<FString, FString>&, const TPair<FString, FString>&>();
        CheckReferenceTypes<TDoubleLinkedList<FString>, FString&, const FString&>();
        CheckReferenceTypes<TStaticArray<FString, 1>, FString&, const FString&>();
        CheckReferenceTypes<TChunkedArray<FString>, FString&, const FString&>();
        CheckReferenceTypes<TDeque<FString>, FString&, const FString&>();
        CheckReferenceTypes<TIndirectArray<FString>, FString&, const FString&>();
        CheckReferenceTypes<TSparseArray<FString>, FString&, const FString&>();
        CheckReferenceTypes<FString, TCHAR&, const TCHAR&>();

        // do NOT use containers with const elements EVER (move semantics is broken for such types)
        // but we still need to make sure that such types at least compile
        CheckReferenceTypes<TArray<const FString>, const FString&, const FString&>();
        CheckReferenceTypes<TDoubleLinkedList<const FString>, const FString&, const FString&>();
        CheckReferenceTypes<TStaticArray<const FString, 1>, const FString&, const FString&>();
        CheckReferenceTypes<const FString, const TCHAR&, const TCHAR&>();

        // TArrayView do not propagate constness (just like std::span)
        // use TArrayView<const T> to propagate constness (just like std::span)
        CheckReferenceTypes<TArrayView<FString>, FString&, FString&>();
        CheckReferenceTypes<TArrayView<const FString>, const FString&, const FString&>();

        // TStridedView do not propagate constness (just like std::span)
        // use TStridedView<const T> to propagate constness (just like std::span)
        CheckReferenceTypes<TStridedView<FString>, FString&, FString&>();
        CheckReferenceTypes<TStridedView<const FString>, const FString&, const FString&>();

        // FStringView is immutable (just like std::string_view)
        CheckReferenceTypes<FStringView, const TCHAR&, const TCHAR&>();
        CheckReferenceTypes<const FStringView, const TCHAR&, const TCHAR&>();
    }

    static void BiDirIterator()
    {
        using namespace Ur::View;

        TArray<FString> Array;
        TReverseView<TRefView<TArray<FString>>> ReverseView = Ur::View::Reverse(Array);
        TReverseView<TRefView<TArray<FString>>> ReverseView2 = Ur::View::Ref(Array).Reverse();
        TReverseView<TRefView<const TArray<FString>>> DeepConstReverseView = Ur::View::Cref(Array).Reverse();
    }

    static void IteratorReference()
    {
        TArray<FString> Array;
        const TArray<FString> ConstArray;

        Ur::View::TRefView<const TArray<FString>> DeepConstView = Ur::View::Ref(ConstArray);
        const Ur::View::TRefView<const TArray<FString>> ConstDeepConstView = Ur::View::Ref(ConstArray);

        Ur::View::TRefView<TArray<FString>> View = Ur::View::Ref(Array);
        const Ur::View::TRefView<TArray<FString>> ConstView = Ur::View::Ref(Array);

        Ur::View::TRefView<const TArray<FString>> CrefDeepConstView = Ur::View::Cref(Array);

        static_assert(std::is_same_v<decltype(*ConstView.begin()), const FString&>, "should be const ref");
        static_assert(std::is_same_v<decltype(*View.begin()), FString&>, "should be non-const ref");

        static_assert(std::is_same_v<decltype(*DeepConstView.begin()), const FString&>, "should be const ref");
        static_assert(std::is_same_v<decltype(*ConstDeepConstView.begin()), const FString&>, "should be const ref");

        static_assert(std::is_same_v<decltype(*CrefDeepConstView.begin()), const FString&>, "should be const ref");

        for (FString& Str : View)
        {
            (void)Str;
        }

        for (const FString& Str : ConstView)
        {
            (void)Str;
        }

        for (const FString& Str : DeepConstView)
        {
            (void)Str;
        }
    }
}