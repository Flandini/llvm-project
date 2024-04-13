//===-- Utility class to test different flavors of fdim ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#include "src/__support/FPUtil/BasicOperations.h"
#include "src/__support/FPUtil/FPBits.h"
#include "test/UnitTest/FPMatcher.h"
#include "test/UnitTest/Test.h"

template <typename T>
class FDimTestTemplate : public LIBC_NAMESPACE::testing::Test {
public:
  using FuncPtr = T (*)(T, T);

  DECLARE_SPECIAL_CONSTANTS(T)

  void test_nan_arg(FuncPtr func) {
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(aNaN, func(aNaN, inf));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(aNaN, func(neg_inf, aNaN));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(aNaN, func(aNaN, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(aNaN, func(neg_zero, aNaN));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(aNaN, func(aNaN, T(-1.2345)));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(aNaN, func(T(1.2345), aNaN));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(func(aNaN, aNaN), aNaN);
  }

  void test_inf_arg(FuncPtr func) {
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_inf, inf));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(inf, func(inf, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_zero, inf));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(inf, func(inf, T(1.2345)));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(T(-1.2345), inf));
  }

  void test_neg_inf_arg(FuncPtr func) {
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(inf, func(inf, neg_inf));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_inf, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(inf, func(neg_zero, neg_inf));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_inf, T(-1.2345)));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(inf, func(T(1.2345), neg_inf));
  }

  void test_both_zero(FuncPtr func) {
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(zero, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(zero, neg_zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_zero, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_zero, neg_zero));
  }

  void test_special(FuncPtr func) {
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_max_normal, neg_min_denormal));

    // ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(max_normal, min_denormal));
    // ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(max_normal, max_denormal));

    // ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(max_normal, neg_min_denormal));
    // ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(max_normal, neg_max_denormal));

    // ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_max_normal, neg_min_denormal));
    // ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_max_normal, max_denormal));
  }

  void test_in_range(FuncPtr func) {
    constexpr StorageType STORAGE_MAX =
        LIBC_NAMESPACE::cpp::numeric_limits<StorageType>::max();
    constexpr StorageType COUNT = 100'001;
    constexpr StorageType STEP = STORAGE_MAX / COUNT;
    for (StorageType i = 0, v = 0, w = STORAGE_MAX; i <= COUNT;
         ++i, v += STEP, w -= STEP) {
      FPBits xbits(v), ybits(w);
      if (xbits.is_inf_or_nan())
        continue;
      if (ybits.is_inf_or_nan())
        continue;

      T x = xbits.get_val();
      T y = ybits.get_val();

      if (x > y) {
        EXPECT_FP_EQ(x - y, func(x, y));
      } else {
        EXPECT_FP_EQ(zero, func(x, y));
      }
    }
  }
};

#define LIST_FDIM_TESTS(T, func)                                               \
  using LlvmLibcFDimTest = FDimTestTemplate<T>;                                \
  TEST_F(LlvmLibcFDimTest, NaNArg) { test_nan_arg(&func); }                    \
  TEST_F(LlvmLibcFDimTest, InfArg) { test_inf_arg(&func); }                    \
  TEST_F(LlvmLibcFDimTest, NegInfArg) { test_neg_inf_arg(&func); }             \
  TEST_F(LlvmLibcFDimTest, BothZero) { test_both_zero(&func); }                \
  TEST_F(LlvmLibcFDimTest, InFloatRange) { test_in_range(&func); }             \
  TEST_F(LlvmLibcFDimTest, TestSpecial) { test_special(&func); }               \
  static_assert(true, "Require semicolon.")
