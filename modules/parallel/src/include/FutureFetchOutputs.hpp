//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#pragma once
//=============================================================================
#include "nlsParallel_exports.h"
#include "Evaluator.hpp"
#include "FevalFutureObject.hpp"
//=============================================================================
namespace Nelson {
//=============================================================================
NLSPARALLEL_IMPEXP
ArrayOfVector
FutureFetchOutputs(Evaluator* eval, FevalFutureObject* fevalFutureObject);
//=============================================================================
}
//=============================================================================
