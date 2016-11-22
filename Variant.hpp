//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include <Base/Types.hpp>

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#include <tuple>
#include <typeinfo>
#include <type_traits>
#include <utility>

namespace NxA {

struct BadVariantAccess : public FatalException
{
    explicit BadVariantAccess(const character* what_arg) : FatalException{what_arg} {}
};


namespace VariantDetail {

    static constexpr count invalidValue = count(-1);

    template <typename T, typename... Types>
    struct DirectTypeIndex;

    template <typename T, typename First, typename... Types>
    struct DirectTypeIndex<T, First, Types...>
    {
        static constexpr count index = std::is_same<T, First>::value
            ? sizeof...(Types)
            : DirectTypeIndex<T, Types...>::index;
    };

    template <typename T>
    struct DirectTypeIndex<T>
    {
        static constexpr count index = invalidValue;
    };

#if __cpp_lib_logical_traits >= 201510L
    using std::disjunction;
#else

    template <typename...>
    struct disjunction : std::false_type {};

    template <typename B1>
    struct disjunction<B1> : B1 {};

    template<class B1, class... Bn>
    struct disjunction<B1, Bn...> : std::conditional_t<B1::value != false, B1, disjunction<Bn...>>  { };

#endif

    template <typename T, typename... Types>
    struct ConvertibleTypeIndex;

    template <typename T, typename First, typename... Types>
    struct ConvertibleTypeIndex<T, First, Types...>
    {
        static constexpr count index = std::is_convertible<T, First>::value
            ? disjunction<std::is_convertible<T, Types>...>::value ? invalidValue : sizeof...(Types)
            : ConvertibleTypeIndex<T, Types...>::index;
    };

    template <typename T>
    struct ConvertibleTypeIndex<T>
    {
        static constexpr count index = invalidValue;
    };

    template <typename T, typename... Types>
    struct ValueTraits
    {
        using ValueType = typename std::remove_const<typename std::remove_reference<T>::type>::type;
        static constexpr count directIndex = DirectTypeIndex<ValueType, Types...>::index;
        static constexpr boolean isDirect = directIndex != invalidValue;
        static constexpr count index = isDirect ? directIndex : ConvertibleTypeIndex<ValueType, Types...>::index;
        static constexpr boolean isValid = index != invalidValue;
        static constexpr count typeIndex = isValid ? sizeof...(Types) - index : 0;
        using TargetType = typename std::tuple_element<typeIndex, std::tuple<void, Types...>>::type;
    };

    template <typename... Types>
    struct VariantHelper;

    template <typename T, typename... Types>
    struct VariantHelper<T, Types...>
    {
        inline static void move(const count oldTypeIndex, void *oldValue, void *newValue)
        {
            if (oldTypeIndex == sizeof...(Types)) {
                new(newValue) T(std::move(*reinterpret_cast<T*>(oldValue)));
            }
            else  {
                VariantHelper<Types...>::move(oldTypeIndex, oldValue, newValue);
            }
        }

        inline static void copy(const count oldTypeIndex, const void *oldValue, void *newValue)
        {
            if (oldTypeIndex == sizeof...(Types)) {
                new(newValue) T(*reinterpret_cast<const T*>(oldValue));
            }
            else {
                VariantHelper<Types...>::copy(oldTypeIndex, oldValue, newValue);
            }
        }

        inline static void destroy(const count typeIndex, void *data)
        {
            if (typeIndex == sizeof...(Types)) {
                reinterpret_cast<T*>(data)->~T();
            }
            else {
                VariantHelper<Types...>::destroy(typeIndex, data);
            }
        }
    };

    template <>
    struct VariantHelper<>
    {
        inline static void move(const count, void*, void*) {}
        inline static void copy(const count, const void*, void*) {}
        inline static void destroy(const count, void*) {}
    };

    template <typename Function, typename Variant, typename... Types>
    struct Apply;

    template <typename Function, typename Variant, typename T, typename... Types>
    struct Apply<Function, Variant, T, Types...>
    {
        inline static decltype(auto) apply(Variant&& variant, Function&& function)
        {
            if (variant.template isType<T>()) {
                return function(variant.template getUnchecked<T>());
            }
            else {
                return Apply<Function, Variant, Types...>::apply(std::forward<Variant>(variant), std::forward<Function>(function));
            }
        }
    };

    template <typename Function, typename Variant, typename T>
    struct Apply<Function, Variant, T>
    {
        inline static decltype(auto) apply(Variant&& variant, Function&& function)
        {
            return function(variant.template getUnchecked<T>());
        }
    };


    struct EqualCompare
    {
        template <typename T>
        boolean operator()(T const& lhs, T const& rhs) const
        {
            return lhs == rhs;
        }
    };

    struct LessCompare
    {
        template <typename T>
        boolean operator()(T const& lhs, T const& rhs) const
        {
            return lhs < rhs;
        }
    };

    template <typename Value, typename Comparator>
    struct CompareVariant
    {
        explicit CompareVariant(Value const& lhs) noexcept : lhs_{lhs} {}
        CompareVariant& operator=(CompareVariant const&) = delete;

        template <typename T>
        boolean operator()(T const& rhsContent) const
        {
            T const& lhsContent = lhs_.template getUnchecked<T>();
            return Comparator()(lhsContent, rhsContent);
        }

    private:
        Value const& lhs_;
    };


    template <count argument1, count... others>
    struct Max;

    template <count argument>
    struct Max<argument>
    {
        static constexpr count value = argument;
    };

    template <count argument1, count argument2, count... others>
    struct Max<argument1, argument2, others...>
    {
        static constexpr count value = argument1 >= argument2 ? Max<argument1, others...>::value : Max<argument2, others...>::value;
    };

}

struct NoInit
{
};



template <typename... Types>
class Variant final
{
    static_assert(sizeof...(Types) > 0, "Template parameter list of Variant can not be empty");
    static_assert(!VariantDetail::disjunction<std::is_reference<Types>...>::value, "Variant can not hold references");

public:
    using TypesTuple = std::tuple<Types...>;
private:
    static constexpr count dataSize = VariantDetail::Max<sizeof(Types)...>::value;
    static constexpr count dataAlign = VariantDetail::Max<alignof(Types)...>::value;
    using FirstType = typename std::tuple_element<0, TypesTuple>::type;
    using DataType = typename std::aligned_storage<dataSize, dataAlign>::type;
    using HelperType = VariantDetail::VariantHelper<Types...>;

    count typeIndex;
    DataType data;

public:
    inline Variant() noexcept(std::is_nothrow_default_constructible<FirstType>::value) : typeIndex{sizeof...(Types)-1}
    {
        static_assert(std::is_default_constructible<FirstType>::value, "First type of Variant must be default constructible");
        new(&data) FirstType();
    }

    inline Variant(NoInit) noexcept : typeIndex{VariantDetail::invalidValue} {}

    template <typename T,  typename Traits = VariantDetail::ValueTraits<T, Types...>, typename Enable = typename std::enable_if<Traits::isValid && !std::is_same<Variant<Types...>, typename Traits::ValueType>::value>::type>
    inline Variant(T&& val) noexcept(std::is_nothrow_constructible<typename Traits::TargetType, T&&>::value) : typeIndex{Traits::index}
    {
        new(&data) typename Traits::TargetType(std::forward<T>(val));
    }

    inline Variant(Variant<Types...> const& old) : typeIndex{old.typeIndex}
    {
        HelperType::copy(old.typeIndex, &old.data, &data);
    }

    inline Variant(Variant<Types...>&& old) noexcept(std::is_nothrow_move_constructible<TypesTuple>::value) : typeIndex{old.typeIndex}
    {
        HelperType::move(old.typeIndex, &old.data, &data);
    }

private:
    inline void copyAssign(Variant<Types...> const& rhs)
    {
        HelperType::destroy(typeIndex, &data);
        typeIndex = VariantDetail::invalidValue;
        HelperType::copy(rhs.typeIndex, &rhs.data, &data);
        typeIndex = rhs.typeIndex;
    }

    inline void moveAssign(Variant<Types...>&& rhs)
    {
        HelperType::destroy(typeIndex, &data);
        typeIndex = VariantDetail::invalidValue;
        HelperType::move(rhs.typeIndex, &rhs.data, &data);
        typeIndex = rhs.typeIndex;
    }

    template <typename T, typename std::enable_if<(VariantDetail::DirectTypeIndex<T, Types...>::index != VariantDetail::invalidValue)>::type* = nullptr>
    inline T& getUnchecked()
    {
        return *reinterpret_cast<T*>(&data);
    }

    template <typename T, typename std::enable_if<(VariantDetail::DirectTypeIndex<T, Types...>::index != VariantDetail::invalidValue)>::type* = nullptr>
    inline T const& getUnchecked() const
    {
        return *reinterpret_cast<T const*>(&data);
    }

public:
    inline Variant<Types...>& operator=(Variant<Types...>&& other)
    {
        moveAssign(std::move(other));
        return *this;
    }

    inline Variant<Types...>& operator=(Variant<Types...> const& other)
    {
        copyAssign(other);
        return *this;
    }

    template <typename T>
    inline Variant<Types...>& operator=(T&& rhs) noexcept
    {
        Variant<Types...> temp(std::forward<T>(rhs));
        moveAssign(std::move(temp));
        return *this;
    }

    template <typename T>
    inline Variant<Types...>& operator=(T const& rhs)
    {
        Variant<Types...> temp(rhs);
        copyAssign(temp);
        return *this;
    }

    template <typename T, typename std::enable_if<
    (VariantDetail::DirectTypeIndex<T, Types...>::index != VariantDetail::invalidValue)>::type* = nullptr>
    inline boolean isType() const
    {
        return typeIndex == VariantDetail::DirectTypeIndex<T, Types...>::index;
    }

    inline boolean isValid() const
    {
        return typeIndex != VariantDetail::invalidValue;
    }

    template <typename T, typename... Args>
    inline void set(Args&&... arguments)
    {
        HelperType::destroy(typeIndex, &data);
        typeIndex = VariantDetail::invalidValue;
        new(&data) T(std::forward<Args>(arguments)...);
        typeIndex = VariantDetail::DirectTypeIndex<T, Types...>::index;
    }

    template <typename T, typename std::enable_if<(VariantDetail::DirectTypeIndex<T, Types...>::index != VariantDetail::invalidValue)>::type* = nullptr>
    inline T& get()
    {
        if (typeIndex == VariantDetail::DirectTypeIndex<T, Types...>::index) {
            return getUnchecked<T>();
        }
        else {
            throw BadVariantAccess("in get<T>()");
        }
    }

    template <typename T, typename std::enable_if<(VariantDetail::DirectTypeIndex<T, Types...>::index != VariantDetail::invalidValue)>::type* = nullptr>
    inline T const& get() const
    {
        if (typeIndex == VariantDetail::DirectTypeIndex<T, Types...>::index) {
            return getUnchecked<T>();
        }
        else {
            throw BadVariantAccess("in get<T>()");
        }
    }

    inline integer whichType() const noexcept
    {
        return static_cast<integer>(sizeof...(Types)-typeIndex - 1);
    }

    template <typename T, typename std::enable_if<
    (VariantDetail::DirectTypeIndex<T, Types...>::index != VariantDetail::invalidValue)>::type* = nullptr>
    inline static constexpr integer whichType() noexcept
    {
        return static_cast<integer>(sizeof...(Types)-VariantDetail::DirectTypeIndex<T, Types...>::index - 1);
    }

    template <typename F, typename V, typename... Ts> friend struct VariantDetail::Apply;

    template <typename Function, typename V>
    decltype(auto) inline static visit(V&& variant, Function&& function)
    {
        return VariantDetail::Apply<Function, V, Types...>::apply(std::forward<V>(variant), std::forward<Function>(function));
    }

    ~Variant() noexcept
    {
        HelperType::destroy(typeIndex, &data);
    }

    inline boolean operator==(Variant const& rhs) const
    {
        NXA_ASSERT_TRUE(isValid() && rhs.isValid());
        if (this->whichType() != rhs.whichType()) {
            return false;
        }
        VariantDetail::CompareVariant<Variant, VariantDetail::EqualCompare> visitor(*this);
        return visit(rhs, visitor);
    }
    
    inline boolean operator!=(Variant const& rhs) const
    {
        return !(*this == rhs);
    }
    
    inline boolean operator<(Variant const& rhs) const
    {
        NXA_ASSERT_TRUE(isValid() && rhs.isValid());
        if (this->whichType() != rhs.whichType()) {
            return this->whichType() < rhs.whichType();
        }
        VariantDetail::CompareVariant<Variant, VariantDetail::LessCompare> visitor(*this);
        return visit(rhs, visitor);
    }
    
    inline boolean operator>(Variant const& rhs) const
    {
        return rhs < *this;
    }
    
    inline boolean operator<=(Variant const& rhs) const
    {
        return !(*this > rhs);
    }
    
    inline boolean operator>=(Variant const& rhs) const
    {
        return !(*this < rhs);
    }
};

template <typename Variant, typename Function>
decltype(auto) inline withVariant(Variant const& variant, Function&& function)
{
    return Variant::visit(variant, std::forward<Function>(function));
}

template <typename Variant, typename Function>
decltype(auto) inline withVariant(Variant& variant, Function&& function)
{
    return Variant::visit(variant, std::forward<Function>(function));
}

template <typename ResultType, typename T>
decltype(auto) get(T&& someVariant)
{
    return someVariant.template get<ResultType>();
}


}
