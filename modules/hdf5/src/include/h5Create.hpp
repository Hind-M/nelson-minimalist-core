//=============================================================================
// Copyright (c) 2016-2019 Allan CORNET (Nelson)
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
#pragma once
//=============================================================================
#include <boost/container/vector.hpp>
#include "nlsHdf5_exports.h"
#include "ArrayOf.hpp"
//=============================================================================
namespace Nelson {
//=============================================================================
NLSHDF5_IMPEXP void
h5Create(const std::wstring& filename, const std::wstring& dataSetName,
    boost::container::vector<double> sizeData, Class dataType,
    boost::container::vector<double> chunksize, int deflate, ArrayOf fillvalue,
    bool fletcher32 = false, bool shuffle = false, const std::wstring& textEncoding = L"system");
//=============================================================================
} // namespace Nelson
//=============================================================================