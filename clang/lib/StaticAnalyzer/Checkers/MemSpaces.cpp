//===-- MemSpaces.cpp ---------------------------------------------*- C++ -*--//
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

#include "clang/StaticAnalyzer/Checkers/MemSpaces.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramState.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramStateTrait.h"

using namespace clang;
using namespace ento;
using namespace memspace;

REGISTER_MAP_WITH_PROGRAMSTATE(MemSpacesMap, MemRegionTy, MemSpaceTy)

ProgramStateRef setMemSpace(ProgramStateRef State, MemRegionTy MR, MemSpaceTy MS) {
  ProgramStateRef NewState = State->set<MemSpacesMap>(MR, MS);
  return NewState;
}

bool hasMemSpace(ProgramStateRef State, MemRegionTy MR) {
  const MemSpaceTy *Result = State->get<MemSpacesMap>(MR);
  return Result;
}

MemSpaceTy getMemSpace(ProgramStateRef State, MemRegionTy MR) {
  const MemSpaceTy *Result = State->get<MemSpacesMap>(MR);
  if (!Result)
    return nullptr;
  return *Result;
}
