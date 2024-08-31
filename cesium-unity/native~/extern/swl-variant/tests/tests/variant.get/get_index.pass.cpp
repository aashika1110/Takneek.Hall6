// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// Throwing bad_variant_access is supported starting in macosx10.13
// XFAIL: with_system_cxx_lib=macosx10.12 && !no-exceptions
// XFAIL: with_system_cxx_lib=macosx10.11 && !no-exceptions
// XFAIL: with_system_cxx_lib=macosx10.10 && !no-exceptions
// XFAIL: with_system_cxx_lib=macosx10.9 && !no-exceptions

// <variant>

// template <size_t I, class... Types>
//   constexpr variant_alternative_t<I, variant<Types...>>&
//   get(variant<Types...>& v);
// template <size_t I, class... Types>
//   constexpr variant_alternative_t<I, variant<Types...>>&&
//   get(variant<Types...>&& v);
// template <size_t I, class... Types>
//   constexpr variant_alternative_t<I, variant<Types...>> const& get(const
//   variant<Types...>& v);
// template <size_t I, class... Types>
//  constexpr variant_alternative_t<I, variant<Types...>> const&& get(const
//  variant<Types...>&& v);

#include "test_macros.h"
#include "test_workarounds.h"
#include "variant_test_helpers.h"
 
#include <type_traits>
#include <utility>

#include <swl_assert.hpp> 
#include <swl/variant.hpp> 

void test_const_lvalue_get() {
  {
    using V = swl::variant<int, const long>;
    constexpr V v(42);
#ifdef TEST_WORKAROUND_CONSTEXPR_IMPLIES_NOEXCEPT
    ASSERT_NOEXCEPT(swl::get<0>(v));
#else
    ASSERT_NOT_NOEXCEPT(swl::get<0>(v));
#endif
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), const int &);
    static_assert(swl::get<0>(v) == 42, "");
  }
  {
    using V = swl::variant<int, const long>;
    const V v(42);
    ASSERT_NOT_NOEXCEPT(swl::get<0>(v));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), const int &);
    SWL_ASSERT(swl::get<0>(v) == 42);
  }
  {
    using V = swl::variant<int, const long>;
    constexpr V v(42l);
#ifdef TEST_WORKAROUND_CONSTEXPR_IMPLIES_NOEXCEPT
    ASSERT_NOEXCEPT(swl::get<1>(v));
#else
    ASSERT_NOT_NOEXCEPT(swl::get<1>(v));
#endif
    ASSERT_SAME_TYPE(decltype(swl::get<1>(v)), const long &);
    static_assert(swl::get<1>(v) == 42, "");
  }
  {
    using V = swl::variant<int, const long>;
    const V v(42l);
    ASSERT_NOT_NOEXCEPT(swl::get<1>(v));
    ASSERT_SAME_TYPE(decltype(swl::get<1>(v)), const long &);
    SWL_ASSERT(swl::get<1>(v) == 42);
  }
// FIXME: Remove these once reference support is reinstated
#if !defined(TEST_VARIANT_HAS_NO_REFERENCES)
  {
    using V = swl::variant<int &>;
    int x = 42;
    const V v(x);
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), int &);
    SWL_ASSERT(&swl::get<0>(v) == &x);
  }
  {
    using V = swl::variant<int &&>;
    int x = 42;
    const V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), int &);
    SWL_ASSERT(&swl::get<0>(v) == &x);
  }
  {
    using V = swl::variant<const int &&>;
    int x = 42;
    const V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), const int &);
    SWL_ASSERT(&swl::get<0>(v) == &x);
  }
#endif
}

void test_lvalue_get() {
  {
    using V = swl::variant<int, const long>;
    V v(42);
    ASSERT_NOT_NOEXCEPT(swl::get<0>(v));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), int &);
    SWL_ASSERT(swl::get<0>(v) == 42);
  }
  {
    using V = swl::variant<int, const long>;
    V v(42l);
    ASSERT_SAME_TYPE(decltype(swl::get<1>(v)), const long &);
    SWL_ASSERT(swl::get<1>(v) == 42);
  }
// FIXME: Remove these once reference support is reinstated
#if !defined(TEST_VARIANT_HAS_NO_REFERENCES)
  {
    using V = swl::variant<int &>;
    int x = 42;
    V v(x);
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), int &);
    SWL_ASSERT(&swl::get<0>(v) == &x);
  }
  {
    using V = swl::variant<const int &>;
    int x = 42;
    V v(x);
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), const int &);
    SWL_ASSERT(&swl::get<0>(v) == &x);
  }
  {
    using V = swl::variant<int &&>;
    int x = 42;
    V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), int &);
    SWL_ASSERT(&swl::get<0>(v) == &x);
  }
  {
    using V = swl::variant<const int &&>;
    int x = 42;
    V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(v)), const int &);
    SWL_ASSERT(&swl::get<0>(v) == &x);
  }
#endif
}

void test_rvalue_get() {
  {
    using V = swl::variant<int, const long>;
    V v(42);
    ASSERT_NOT_NOEXCEPT(swl::get<0>(std::move(v)));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), int &&);
    SWL_ASSERT(swl::get<0>(std::move(v)) == 42);
  }
  {
    using V = swl::variant<int, const long>;
    V v(42l);
    ASSERT_SAME_TYPE(decltype(swl::get<1>(std::move(v))), const long &&);
    SWL_ASSERT(swl::get<1>(std::move(v)) == 42);
  }
// FIXME: Remove these once reference support is reinstated
#if !defined(TEST_VARIANT_HAS_NO_REFERENCES)
  {
    using V = swl::variant<int &>;
    int x = 42;
    V v(x);
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), int &);
    SWL_ASSERT(&swl::get<0>(std::move(v)) == &x);
  }
  {
    using V = swl::variant<const int &>;
    int x = 42;
    V v(x);
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), const int &);
    SWL_ASSERT(&swl::get<0>(std::move(v)) == &x);
  }
  {
    using V = swl::variant<int &&>;
    int x = 42;
    V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), int &&);
    int &&xref = swl::get<0>(std::move(v));
    SWL_ASSERT(&xref == &x);
  }
  {
    using V = swl::variant<const int &&>;
    int x = 42;
    V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), const int &&);
    const int &&xref = swl::get<0>(std::move(v));
    SWL_ASSERT(&xref == &x);
  }
#endif
}

void test_const_rvalue_get() {
  {
    using V = swl::variant<int, const long>;
    const V v(42);
    ASSERT_NOT_NOEXCEPT(swl::get<0>(std::move(v)));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), const int &&);
    SWL_ASSERT(swl::get<0>(std::move(v)) == 42);
  }
  {
    using V = swl::variant<int, const long>;
    const V v(42l);
    ASSERT_SAME_TYPE(decltype(swl::get<1>(std::move(v))), const long &&);
    SWL_ASSERT(swl::get<1>(std::move(v)) == 42);
  }
// FIXME: Remove these once reference support is reinstated
#if !defined(TEST_VARIANT_HAS_NO_REFERENCES)
  {
    using V = swl::variant<int &>;
    int x = 42;
    const V v(x);
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), int &);
    SWL_ASSERT(&swl::get<0>(std::move(v)) == &x);
  }
  {
    using V = swl::variant<const int &>;
    int x = 42;
    const V v(x);
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), const int &);
    SWL_ASSERT(&swl::get<0>(std::move(v)) == &x);
  }
  {
    using V = swl::variant<int &&>;
    int x = 42;
    const V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), int &&);
    int &&xref = swl::get<0>(std::move(v));
    SWL_ASSERT(&xref == &x);
  }
  {
    using V = swl::variant<const int &&>;
    int x = 42;
    const V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(swl::get<0>(std::move(v))), const int &&);
    const int &&xref = swl::get<0>(std::move(v));
    SWL_ASSERT(&xref == &x);
  }
#endif
}

template <std::size_t I> using Idx = std::integral_constant<size_t, I>;

void test_throws_for_all_value_categories() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  using V = swl::variant<int, long>;
  V v0(42);
  const V &cv0 = v0;
  SWL_ASSERT(v0.index() == 0);
  V v1(42l);
  const V &cv1 = v1;
  SWL_ASSERT(v1.index() == 1);
  std::integral_constant<size_t, 0> zero;
  std::integral_constant<size_t, 1> one;
  auto test = [](auto idx, auto &&v) {
    using Idx = decltype(idx);
    try {
      TEST_IGNORE_NODISCARD swl::get<Idx::value>(std::forward<decltype(v)>(v));
    } catch (const swl::bad_variant_access &) {
      return true;
    } catch (...) { /* ... */
    }
    return false;
  };
  { // lvalue test cases
    SWL_ASSERT(test(one, v0));
    SWL_ASSERT(test(zero, v1));
  }
  { // const lvalue test cases
    SWL_ASSERT(test(one, cv0));
    SWL_ASSERT(test(zero, cv1));
  }
  { // rvalue test cases
    SWL_ASSERT(test(one, std::move(v0)));
    SWL_ASSERT(test(zero, std::move(v1)));
  }
  { // const rvalue test cases
    SWL_ASSERT(test(one, std::move(cv0)));
    SWL_ASSERT(test(zero, std::move(cv1)));
  }
#endif
}

int main(int, char**) {
  test_const_lvalue_get();
  test_lvalue_get();
  test_rvalue_get();
  test_const_rvalue_get();
  test_throws_for_all_value_categories();

  SWL_END_TEST_SIGNAL 
 return 0; 

}
