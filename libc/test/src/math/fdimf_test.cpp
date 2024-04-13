//===-- Unittests for fdimf -----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "FDimTest.h"

#include "hdr/math_macros.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/math/fdimf.h"
#include "test/UnitTest/FPMatcher.h"
#include "test/UnitTest/Test.h"

LIST_FDIM_TESTS(float, LIBC_NAMESPACE::fdimf);
