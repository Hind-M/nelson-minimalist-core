//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "FevalFuture_displayBuiltin.hpp"
#include "FevalFutureObject.hpp"
#include "Error.hpp"
#include "HandleGenericObject.hpp"
#include "HandleManager.hpp"
#include "DisplayVariableHelpers.hpp"
//=============================================================================
using namespace Nelson;
//=============================================================================
ArrayOfVector
Nelson::ParallelGateway::FevalFuture_displayBuiltin(
    Evaluator* eval, int nLhs, const ArrayOfVector& argIn)
{
    ArrayOfVector retval;
    nargoutcheck(nLhs, 0, 0);
    nargincheck(argIn, 1, 2);
    ArrayOf param1 = argIn[0];
    if (param1.isHandle()) {
        std::wstring name;
        if (argIn.size() == 2) {
            name = argIn[1].getContentAsWideString();
        }
        Interface* io = eval->getInterface();
        DisplayVariableHeader(io, param1, name, false);
        if (param1.isScalar()) {
            if (param1.getHandleCategory() != FEVALFUTURE_CATEGORY_STR) {
                Error(_W("FevalFuture handle expected."));
            }
            auto* fevalFutureObject = (FevalFutureObject*)param1.getContentAsHandleScalar();
            fevalFutureObject->display(io);
        } else {
            Dimensions dims = param1.getDimensions();
            nelson_handle* qp = (nelson_handle*)(param1.getDataPointer());
            size_t elementCount = static_cast<size_t>(dims.getElementCount());
            if (elementCount) {
                io->outputMessage(L"\n");
            }
            size_t idx = 1;
            for (size_t k = 0; k < elementCount; k++) {
                nelson_handle hl = qp[k];
                HandleGenericObject* hlObj = HandleManager::getInstance()->getPointer(hl);
                if (hlObj) {
                    if (hlObj->getCategory() == FEVALFUTURE_CATEGORY_STR) {
                        auto* fevalFutureObject = (FevalFutureObject*)hlObj;
                        fevalFutureObject->displayOnOneLine(io, idx);
                        idx++;
                    }
                }
            }
        }
        DisplayVariableFooter(io, name.empty());
    } else {
        Error(_W("FevalFuture handle expected."));
    }
    return retval;
}
//=============================================================================