//===-- Utility class to test different flavors of fdim ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#include "hdr/math_macros.h"
#include "src/__support/FPUtil/BasicOperations.h"
#include "src/__support/FPUtil/FPBits.h"
#include "test/UnitTest/FPMatcher.h"
#include "test/UnitTest/Test.h"

template <typename T>
class FDimTestTemplate : public LIBC_NAMESPACE::testing::Test {
public:
  using FuncPtr = T (*)(T, T);
  using FPBits = LIBC_NAMESPACE::fputil::FPBits<T>;
  using StorageType = typename FPBits::StorageType;

  const T inf = FPBits::inf(Sign::POS).get_val();
  const T neg_inf = FPBits::inf(Sign::NEG).get_val();
  const T zero = FPBits::zero(Sign::POS).get_val();
  const T neg_zero = FPBits::zero(Sign::NEG).get_val();
  const T nan = FPBits::quiet_nan().get_val();

  void test_na_n_arg(FuncPtr func) {
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(nan, func(nan, inf));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(nan, func(neg_inf, nan));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(nan, func(nan, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(nan, func(neg_zero, nan));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(nan, func(nan, T(-1.2345)));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(nan, func(T(1.2345), nan));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(func(nan, nan), nan);
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
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_inf, neg_inf));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_inf, T(-1.2345)));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(inf, func(T(1.2345), neg_inf));
  }

  void test_both_zero(FuncPtr func) {
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(zero, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(zero, neg_zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_zero, zero));
    ASSERT_FP_EQ_NO_ERRNO_EXCEPTION(zero, func(neg_zero, neg_zero));
  }

  void test_in_range(FuncPtr func) {
    constexpr StorageType STORAGE_MAX =
        LIBC_NAMESPACE::cpp::numeric_limits<StorageType>::max();
    constexpr StorageType COUNT = 100'001;
    constexpr StorageType STEP = STORAGE_MAX / COUNT;
    for (StorageType i = 0, v = 0, w = STORAGE_MAX; i <= COUNT;
         ++i, v += STEP, w -= STEP) {
      T x = FPBits(v).get_val(), y = FPBits(w).get_val();
      if (isnan(x) || isinf(x))
        continue;
      if (isnan(y) || isinf(y))
        continue;

      if (x > y) {
        EXPECT_FP_EQ(x - y, func(x, y));
      } else {
        EXPECT_FP_EQ(zero, func(x, y));
      }
    }
  }
};
