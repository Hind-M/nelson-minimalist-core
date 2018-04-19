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
#include "dlsym_ispropBuiltin.hpp"
#include "Error.hpp"
#include "DynamicLinkSymbolObject.hpp"
#include "HandleGenericObject.hpp"
#include "HandleManager.hpp"
//=============================================================================
using namespace Nelson;
//=============================================================================
ArrayOfVector Nelson::DynamicLinkGateway::dlsym_ispropBuiltin(Evaluator* eval, int nLhs, const ArrayOfVector& argIn)
{
	if (argIn.size() != 2)
	{
		Error(eval, ERROR_WRONG_NUMBERS_INPUT_ARGS);
	}
	if (nLhs > 1)
	{
		Error(eval, ERROR_WRONG_NUMBERS_OUTPUT_ARGS);
	}
	ArrayOfVector retval;
	ArrayOf param1 = argIn[0];
	if (param1.isHandle())
	{
		nelson_handle *qp = (nelson_handle*)param1.getDataPointer();
		nelson_handle hl = qp[0];
		HandleGenericObject *hlObj = HandleManager::getInstance()->getPointer(hl);
		if (hlObj->getCategory() != DLSYM_CATEGORY_STR)
		{
			Error(eval, _W("dlsym handle expected."));
		}
		ArrayOf param2 = argIn[1];
		std::wstring propertyName = param2.getContentAsWideString();
		DynamicLinkSymbolObject *objDlsym = (DynamicLinkSymbolObject *)hlObj;
		ArrayOf res = ArrayOf::logicalConstructor(objDlsym->isproperty(propertyName));
		retval.push_back(res);
	}
	else
	{
		Error(eval, _W("dlsym handle expected."));
	}
	return retval;
}
//=============================================================================