//=============================================================================
// Copyright (c) 2016-2018 Allan CORNET (Nelson)
//=============================================================================
// LICENCE_BLOCK_BEGIN
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// LICENCE_BLOCK_END
//=============================================================================
#include "NelsonGateway.hpp"
#include "errorBuiltin.hpp"
#include "lasterrorBuiltin.hpp"
#include "lastwarnBuiltin.hpp"
#include "warningBuiltin.hpp"
//=============================================================================
using namespace Nelson;
//=============================================================================
const std::wstring gatewayName = L"error_manager";
//=============================================================================
static const nlsGateway gateway[] = {
    { "error", Nelson::ErrorManagerGateway::errorBuiltin, 0, 1 },
    { "warning", Nelson::ErrorManagerGateway::warningBuiltin, 1, -1 },
    { "lasterror", Nelson::ErrorManagerGateway::lasterrorBuiltin, 1, 1 },
    { "lastwarn", Nelson::ErrorManagerGateway::lastwarnBuiltin, 2, 2 },
};
//=============================================================================
NLSGATEWAYFUNC(gateway)
//=============================================================================
NLSGATEWAYINFO(gateway)
//=============================================================================
NLSGATEWAYREMOVE(gateway)
//=============================================================================
NLSGATEWAYNAME()
//=============================================================================
