//===-- MemSpaces.h -----------------------------------------------*- C++ -*--//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// TODO
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_STATICANALYZER_CHECKERS_MEMSPACES_H
#define LLVM_CLANG_STATICANALYZER_CHECKERS_MEMSPACES_H

#include "clang/StaticAnalyzer/Core/PathSensitive/MemRegion.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramState_Fwd.h"

namespace clang {
namespace ento {

class MemRegion;
class MemSpaceRegion;
  
namespace memspace {

using MemRegionTy = const MemRegion *;
using MemSpaceTy = const MemSpaceRegion *;

[[nodiscard]] ProgramStateRef setMemSpace(ProgramStateRef State, MemRegionTy MR, MemSpaceTy MS);

[[nodiscard]] bool hasMemSpace(ProgramStateRef State, MemRegionTy MR);

[[nodiscard]] MemSpaceTy getMemSpace(ProgramStateRef State, MemRegionTy MR);

template <typename T>
bool regionIsMemSpaceType(ProgramStateRef State, MemRegionTy MR);

} // namespace memspace
} // namespace ento
} // namespace clang

#endif // LLVM_CLANG_STATICANALYZER_CHECKERS_MEMSPACES_H
