[![Standard](https://img.shields.io/badge/standard-C%2B%2B20-blue.svg?logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://mit-license.org/)

# UnrealRanges #

UnrealRanges is a C++20 ranges library (Unreal Engine 5 plugin) designed with Unreal Engine containers in mind, which allow to create and manipulate *range views*, lightweight objects that indirectly represent iterable sequences (ranges). It is similar in spirit to C++20 flux library, C++20 standard ranges, C# Linq, Rust iterators and others.

Unreal Engine containers are well known for their unique iteration model, which was designed years ago, before C++11 was introduced (`CreateIterator` and family). C++11 range-based for loop support was added as a half-baked solution, and its development has since stalled. Unreal Engine containers and iterators do not meet even basic standard iterator and container requirements. As a result, they cannot be used with standard algorithms or the ranges library. Instead, Unreal relies on its own implementations, which are also outdated in terms of design.

The goal of UnrealRanges library is to provide a modern, composable, performant, and easy-to-use interface for Unreal Engine containers, out of the box.

UnrealRanges offers:

* A large selection of algorithms and views adaptors for creating powerful and efficient data pipelines
* Unreal Engine containers support (TArray, TSet, TMap, etc.)
* **Discoverability** – Each view adaptor and algorithm for a particular view is essentially a **C++ method** of that view, meaning auto-complete in most IDEs will work out of the box. In my opinion, this is the most appealing, important, and powerful feature of this library, one that is hard to overestimate. You simply type a *dot* on your keyboard, and your IDE’s auto-complete instantly shows the available adaptors and algorithms. Just like in C# or Rust! No more abusing `operator|` overload set nonsence.
* Improved safety compared to standard library iterators and ranges (due to iteration model which is based on *cursors* instead of *iterators*)
* Near zero overhead and overall improved run-time efficiency for some common operations (due to usage of so-called *internal iteration* for most common algorithms)
* Consistency and const-correctness (C++20 ranges are known for not providing const-correctness for some of its views of for having caching *begin()* which is a SUPER bad decision IMHO and it will hurt C++ ranges reputation badly)

## Documentation ##

Work-in-progress...

## A Quick Example ##

```cpp
// 'Sum' algorithm will use 'internal iteration' under the hood
const auto Ints = TArray<int32>{ 0,1,2,3,4,5,6,7,8,9 };

int32 Sum = Ur::View::Ref(Ints)                     // 0,1,2,3,...
    .Filter([](int32 Num){ return Num % 2 == 0; })  // 0,2,4,6,...
    .Transform([](int32 Num) { return Num * 2; })   // 0,4,8,12,...
    .Take(3)                                        // 0,4,8
    .Sum();                                         // 12

assert(Sum == 12);


// 'To' algorithm will use 'internal iteration' under the hood
const auto Strs = TArray<FString>{ TEXT("0"),TEXT("1") };

TMap<int32, FString> Map = Ur::View::Ref(Strs)      // "0","1"
    .ConcatWith(Ur::View::Single(TEXT("2")))        // "0","1","2"
    .ZipWith(Ur::View::Ints(0))                     // TTuple("0", 0), TTuple("1", 1), TTuple("2", 2)
    .Swizzle<1, 0>()                                // TTuple(0, "0"), TTuple(1, "1"), TTuple(2, "2")
    .To<TMap>();                                    // TMap{ TPair(0, "0"), TPair(1, "1"), TPair(2, "2") }

const auto Expected = std::array{ MakeTuple(0, TEXT("0"_s)), MakeTuple(1, TEXT("1"_s)), MakeTuple(2, TEXT("2"_s)) };
assert(Ur::View::Ref(Map).EqualTo(Expected));


// iterators and range-based for loop are supported as well
for (const auto [ItemReference, Index] : Ur::View::Ref(Strs).Reverse().Enumerate(13))
{
    // ["1", 13]
    // ["0", 14]
}
```

## Fab (Epic Games marketplace) ##

To be filled up...

## Compiler support ##

UnrealRanges requires a recent compiler with good support for the C++20 standard. It is tested with:

* AppleClang ??? (not tested yet)
* MSVC 2022

## The UnrealRanges difference ##

UnrealRanges provides a broadly equivalent feature set to C++20 Ranges, but uses a slightly different iteration model based around *cursors* rather than STD *iterators* or UE *iterators*. UnrealRanges cursors are a generalisation of array *indices*, whereas STL iterators are a generalisation of array *pointers*.

A UnrealRanges `view` provides five basis operations:

* `Ur::Cursor::Begin(View)` returns an object called a *cursor*, which represents a *begin* position in a view.
* `Ur::Cursor::End(View)` returns an object called a *cursor*, which represents a *past-the-end* position in a view.
* `Ur::Cursor::Eq(View, CursorLhs, CursorRhs)` returns a boolean value indicating whether cursors are equal
* `Ur::Cursor::Inc(View, Cursor)` increments the given cursor, so that it points to the next element in the view
* `Ur::Cursor::Deref(View, Cursor)` returns the view element at the given cursor position

For a view with N elements there are N+1 possible cursor positions, including the past-the-end (terminal) position.

These basis operations are equivalent to the basis operations on STL iterators. The crucial difference is that in the UnrealRanges model, you need to **provide both the view and the cursor** to each function call, whereas in the STL model the iterator must know how to increment and dereference itself.

> STL iterators are "smart", but UnrealRanges cursors are not!

This seemingly small change has some far-reaching consequences. In particular:

* Because we need the view object in order to do anything useful with a cursor, **dangling cursors are not possible by design**: if the view object is no longer around, the cursor can't be used
* Because a cursor only represents a position in a view (like an integer index for an array), cursor **invalidation is much less likely** when modifying the underlying view (by modifying underlying container).

## STD ranges quirks and how UnrealRanges differs ##

There are several YouTube videos on the design pitfalls of C++20 ranges by Nicolai Josuttis.\
[C++20 – My Favourite Code Examples - Nicolai Josuttis - ACCU 2022](https://www.youtube.com/watch?v=UCIKbUvEKfI)\
[Belle Views on C++ Ranges, their Details and the Devil - Nico Josuttis - Keynote Meeting C++ 2022](https://www.youtube.com/watch?v=O8HndvYNvQ4)

Highly recommend!

To summarize:
* STD views lack const propagation
* STD `drop_while_view`, `filter_view`, `split_view` will *always* cache theirs `begin` iterator (`drop_view`, `reverse` *may* cache it). This is a **BIG** one and it has a lot of bad consequences:
    * `begin` methods aren't `const`
    * `begin` methods aren't thread-safe
    * inconsistencies while self-modifying during iteration

UnrealRanges deliberately chooses not to cache begin iterators (following the principle of least surprise) and ensures const-propagation for its views. I daresay that const-propagation is exactly what everyone expects when iterating over const views—period.\
As for not caching begin iterators: yes, it violates the STD principle of constant-time complexity for begin iterators, but: 1) STD begin isn’t truly constant-time coz the first invocation will take linear time anyway, and 2) views are typically used ad-hoc, where begin is usually invoked only once. In my opinion, not caching the begin iterator is what most programmers would assume (principle of least astonishment).

```cpp
std::vector<int32> Array = { 1,2,3 };

// generic read-only function
auto Print = [](const auto& Rng) {
    for (auto& Val : Rng)
    {
    }
};

{
    const auto StdFilteredConst = Array | std::views::filter(Ur::Fn::Even);
    const auto StdTakenConst = Array | std::views::take(5);
    auto StdTakenNonConst = Array | std::views::take(5);

    // StdFilteredConst.begin();                                                        // BAD - wont't compile (begin is non-const)
    // StdFilteredConst.cbegin();                                                       // BAD - wont't compile (cbegin member is missing)
    static_assert(std::is_same_v<decltype(*StdTakenConst.begin()), int32&>);            // BAD - no const-propagation
    static_assert(std::is_same_v<decltype(*std::cbegin(StdTakenConst)), int32&>);       // BAD - std::cbegin is broken
    static_assert(std::is_same_v<decltype(*std::cbegin(StdTakenNonConst)), int32&>);    // BAD - std::cbegin is broken
    // Print(StdFilteredConst);                                                         // BAD - won't compile
}

{
    const auto UrFilteredConst = Ref(Array).Filter(Ur::Fn::Even);
    const auto UrTakenConst = Ref(Array).Take(2);
    auto UrTakenNonConst = Ref(Array).Take(2);

    static_assert(std::is_same_v<decltype(*UrFilteredConst.begin()), const int32&>);        // OK
    static_assert(std::is_same_v<decltype(*UrFilteredConst.cbegin()), const int32&>);       // OK
    static_assert(std::is_same_v<decltype(*UrTakenConst.begin()), const int32&>);           // OK (const-propagation)
    static_assert(std::is_same_v<decltype(*std::cbegin(UrTakenConst)), const int32&>);      // OK
    static_assert(std::is_same_v<decltype(*std::cbegin(UrTakenNonConst)), const int32&>);   // OK
    Print(UrFilteredConst);                                                                 // OK
}
```


## Known issues and future work ##

1. add proper STD containers support + try to satisfy some STD concepts (`input_range`, `forward_range` etc.) is possible
1. `constexpr` for STD containers (isn't possible for UE containers unfortunately coz they aren't constexpr)
1. `own_view`
1. `enable_borrowed_range` support
1. consider to use `int` indices instead of iterators in `Cursor` implementation for random access UE containers

## Stability ##

UnrealRanges is still pre-1.0. As such, there are no API stability guarantees at this time.

Once UnrealRanges 1.0 is released I'm aiming to follow [semantic versioning](https://semver.org).

## License ##

UnrealRanges is available under the [MIT license](https://mit-license.org/)