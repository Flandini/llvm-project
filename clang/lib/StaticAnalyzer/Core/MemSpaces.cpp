//===-- MemSpaces.cpp ---------------------------------------------*- C++ -*--//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines getters and setters for the memory space trait which
// associates memory regions with memory spaces in the program state. It also
// defines the MemSpacesMap which maps memory regions to memory spaces.
//
// These functions keep the following invariants over the MemSpacesMap:
// 1. (Temporary as an intermediate step) Memory space traits are only
//    mapped for memory regions that have an unknown memory space
// 2. Only base memory regions are mapped in the trait
// 3. Memory regions which have no mapping in the trait are assumed to be
//    unknown; no memory region is allowed to be mapped to an unknown memory
//    space in the trait;
//
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Core/PathSensitive/MemSpaces.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramState.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramStateTrait.h"
#include <cassert>

REGISTER_MAP_WITH_PROGRAMSTATE(MemSpacesMap, const clang::ento::MemRegion *,
                               const clang::ento::MemSpaceRegion *)

namespace clang {
namespace ento {
namespace memspace {} // namespace memspace
} // namespace ento
} // namespace clang
