#pragma once
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"
#include <memory>
#include <functional>

template<typename T>
struct TOptional<T&> : private TOptional<T*>
{
private:
	using BaseOptional = TOptional<T*>;
	using OptionalType = T&;
	using UnderlyingOptionalType = T*;

	// below is basically a copy-paste from Optional.h
	// changes:
	//   1. delete "from rvalue" ctor / "rvalue" assign operator / "rvalue" Emplace
	//   2. no "in-place" ctor

public:
	using ElementType = OptionalType;

	/** Construct an OptionalType with a valid value.
	*   1. we can construct T& from another T&
	*   2. we can construct const T& from T&
	*   3. we can construct T& from U& where U is derived from T
	*/
	TOptional(T& InValue)
		: BaseOptional(std::addressof(InValue))
	{
	}
	template<typename U>
	TOptional(std::reference_wrapper<U> InValue) requires std::is_constructible_v<T&, U&>
		: BaseOptional(std::addressof(InValue.get()))
	{
	}
	TOptional(T&& InValue) = delete;

	/** Construct an OptionalType with an invalid value. */
	TOptional(FNullOpt)
		: BaseOptional()
	{
	}

	/** Construct an OptionalType with no value; i.e. unset */
	TOptional()
		: BaseOptional()
	{
	}

	/** Copy/Move construction */
	TOptional(const TOptional& Other)
		: BaseOptional(Other)
	{
	}
	TOptional(TOptional&& Other)
		: BaseOptional(MoveTemp(Other))
	{
	}

	TOptional& operator=(const TOptional& Other)
	{
		BaseOptional::operator=(Other);
		return *this;
	}
	TOptional& operator=(TOptional&& Other)
	{
		BaseOptional::operator=(MoveTemp(Other));
		return *this;
	}

	TOptional& operator=(T& InValue)
	{
		Emplace(InValue);
		return *this;
	}
	template<typename U>
	TOptional& operator=(std::reference_wrapper<U> InValue) requires std::is_constructible_v<T&, U&>
	{
		Emplace(InValue.get());
		return *this;
	}
	TOptional& operator=(T&& InValue) = delete;

	void Reset()
	{
		BaseOptional::Reset();
	}

	T& Emplace(T& Val)
	{
		return *BaseOptional::Emplace(std::addressof(Val));
	}

	template<typename U>
	T& Emplace(std::reference_wrapper<U> Val) requires std::is_constructible_v<T&, U&>
	{
		return Emplace(Val.get());
	}

	T& Emplace(T&& Val) = delete;

	friend bool operator==(const TOptional& Lhs, const TOptional& Rhs)
	{
		return operator==(static_cast<const BaseOptional&>(Lhs), static_cast<const BaseOptional&>(Rhs));
	}

	friend bool operator!=(const TOptional& Lhs, const TOptional& Rhs)
	{
		return !(Lhs == Rhs);
	}

	void Serialize(FArchive& Ar)
	{
		BaseOptional::Serialize(Ar);
	}

	/** @return true when the value is meaningful; false if calling GetValue() is undefined. */
	bool IsSet() const
	{
		return BaseOptional::IsSet();
	}
	FORCEINLINE explicit operator bool() const
	{
		return IsSet();
	}

	/** @return The optional value; undefined when IsSet() returns false. */
	T& GetValue()
	{
		return *BaseOptional::GetValue();
	}
	FORCEINLINE const T& GetValue() const
	{
		return const_cast<TOptional*>(this)->GetValue();
	}

	T* operator->()
	{
		return &GetValue();
	}
	FORCEINLINE const T* operator->() const
	{
		return const_cast<TOptional*>(this)->operator->();
	}

	T& operator*()
	{
		return GetValue();
	}
	FORCEINLINE const T& operator*() const
	{
		return const_cast<TOptional*>(this)->operator*();
	}

	/** @return The optional value when set; DefaultValue otherwise. */
	const T& Get(const T& DefaultValue UE_LIFETIMEBOUND) const UE_LIFETIMEBOUND
	{
		return *BaseOptional::Get(std::addressof(DefaultValue));
	}

	/** @return A pointer to the optional value when set, nullptr otherwise. */
	T* GetPtrOrNull()
	{
		return IsSet() ? std::addressof(GetValue()) : nullptr;
	}
	FORCEINLINE const T* GetPtrOrNull() const
	{
		return const_cast<TOptional*>(this)->GetPtrOrNull();
	}
};