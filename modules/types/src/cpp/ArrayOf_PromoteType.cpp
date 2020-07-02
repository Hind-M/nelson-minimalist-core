//=============================================================================
// Copyright (c) 2019-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program. If not, see <http://www.gnu.org/licenses/>.
// LICENCE_BLOCK_END
//=============================================================================
#include <limits>
#include <Eigen/Dense>
#include "ArrayOf.hpp"
#include "Data.hpp"
#include "Dimensions.hpp"
#include "Types.hpp"
#include "SparseDynamicFunctions.hpp"
#include "nlsConfig.h"
//=============================================================================
namespace Nelson {
//=============================================================================
template <typename TIN, typename TOUT>
inline TOUT
numeric_cast(TIN value)
{
    const bool positive_overflow_possible
        = std::numeric_limits<TOUT>::max() < std::numeric_limits<TIN>::max();
    const bool negative_overflow_possible = std::numeric_limits<TIN>::is_signed
        || (std::numeric_limits<TOUT>::lowest() > std::numeric_limits<TIN>::lowest());

    // unsigned <-- unsigned
    if ((!std::numeric_limits<TOUT>::is_signed) && (!std::numeric_limits<TIN>::is_signed)) {
        if (positive_overflow_possible && (value > std::numeric_limits<TOUT>::max())) {
            return std::numeric_limits<TOUT>::max();
        }
    }
    // unsigned <-- signed
    else if ((!std::numeric_limits<TOUT>::is_signed) && std::numeric_limits<TIN>::is_signed) {
        if (positive_overflow_possible && (value > std::numeric_limits<TOUT>::max())) {
            return std::numeric_limits<TOUT>::max();
        } else if (negative_overflow_possible && (value < 0)) {
            return std::numeric_limits<TOUT>::min();
        }
    }
    // signed <-- unsigned
    else if (std::numeric_limits<TOUT>::is_signed && (!std::numeric_limits<TIN>::is_signed)) {
        if (positive_overflow_possible && (value > std::numeric_limits<TOUT>::max())) {
            return std::numeric_limits<TOUT>::max();
        }
    }
    // signed <-- signed
    else if (std::numeric_limits<TOUT>::is_signed && std::numeric_limits<TIN>::is_signed) {
        if (positive_overflow_possible && (value > std::numeric_limits<TOUT>::max())) {
            return std::numeric_limits<TOUT>::max();
        } else if (negative_overflow_possible && (value < std::numeric_limits<TOUT>::lowest())) {
            return std::numeric_limits<TOUT>::min();
        }
    }
    if (typeid(TOUT) != typeid(single) || typeid(TOUT) != typeid(double)) {
        if (typeid(TIN) == typeid(single) || typeid(TIN) == typeid(double)) {
            return static_cast<TOUT>((long double)std::round(value));
        }
    }
    return static_cast<TOUT>(value);
}
//=============================================================================
template <class TIN, class TOUT>
void
saturate(Class classIn, Class classOut, const void* pIn, void* pOut, indexType count)
{
    const TIN* sp = (const TIN*)pIn;
    TOUT* qp = (TOUT*)pOut;
    if (classIn != classOut) {
        bool checkNaN = false;
        if (typeid(TOUT) != typeid(single) || typeid(TOUT) != typeid(double)) {
            if (typeid(TIN) == typeid(single) || typeid(TIN) == typeid(double)) {
                checkNaN = true;
            }
        }
        if (checkNaN) {
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
            for (ompIndexType i = 0; i < (ompIndexType)count; i++) {
                if (std::isnan((double)sp[i])) {
                    qp[i] = (TOUT)0;
                } else {
                    qp[i] = (TOUT)numeric_cast<TIN, TOUT>(sp[i]);
                }
            }
        } else {
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
            for (ompIndexType i = 0; i < (ompIndexType)count; i++) {
                qp[i] = numeric_cast<TIN, TOUT>(sp[i]);
            }
        }
    }
}
//=============================================================================
/**
 * Promote our data to a new type.
 *
 * Copy data from our data array to the specified
 * array, converting the data as we go.  We can only
 * convert data to or from base types.  So if the source
 * or destination types are reference types, we cannot
 * perform the conversion.
 *
 * For the remaining types, we have a matrix of
 * possibilities.  Here we list the conversion rules.
 *
 * Source type
 *  - string
 *    - logical dest = (source == 0) ? 0 : 1
 *    - real dest = (double) source
 *    - complex dest = (double) source
 *  - logical
 *    - string dest = (char) source
 *    - real   dest = (double) source
 *    - complex dest = (double) source
 *  - real
 *    - string dest = (char) source
 *    - logical dest = (source == 0) ? 0 : 1
 *    - complex dest = (double) source
 *  - complex
 *    - string dest = (char) real(source)
 *    - logical dest = (real(source) == 0 && imag(source) == 0) ? 0:1
 *    - real dest = real(source)
 */
//=============================================================================
template <class TIN, class TOUT>
TOUT*
promoteAsReal(Class dstClass, const TIN* ptr, indexType count)
{
    TOUT* dstPtr = (TOUT*)ArrayOf::allocateArrayOf(dstClass, count);
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
    for (ompIndexType i = 0; i < (ompIndexType)count; i++) {
        dstPtr[i] = (TOUT)ptr[i];
    }
    return dstPtr;
}
//=============================================================================
template <class TIN, class TOUT>
TOUT*
promoteAsLogical(Class dstClass, const TIN* ptr, indexType count)
{
    TOUT* dstPtr = (TOUT*)ArrayOf::allocateArrayOf(dstClass, count);
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
    for (ompIndexType i = 0; i < (ompIndexType)count; i++) {
        dstPtr[i] = (ptr[i] == 0) ? 0 : 1;
    }
    return dstPtr;
}
//=============================================================================
template <class TIN, class TOUT>
TOUT*
promoteComplexAsLogical(Class dstClass, const TIN* ptr, indexType count)
{
    TOUT* dstPtr = (TOUT*)ArrayOf::allocateArrayOf(dstClass, count);
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
    for (ompIndexType i = 0; i < (ompIndexType)count; i++) {
        dstPtr[i] = (ptr[i * 2] == 0) ? 0 : 1;
    }
    return dstPtr;
}
//=============================================================================
template <class TIN, class TOUT>
TOUT*
promoteComplexAsReal(Class dstClass, const TIN* ptr, indexType count)
{
    TOUT* dstPtr = (TOUT*)ArrayOf::allocateArrayOf(dstClass, count);
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
    for (ompIndexType i = 0; i < (ompIndexType)count; i = i + 1) {
        dstPtr[i] = (TOUT)ptr[i * 2];
    }
    return dstPtr;
}
//=============================================================================
template <class TIN, class TOUT>
TOUT*
promoteComplexAsComplex(Class dstClass, const TIN* ptr, indexType count)
{
    TOUT* dstPtr = (TOUT*)ArrayOf::allocateArrayOf(dstClass, count);
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
    for (ompIndexType i = 0; i < (ompIndexType)count * 2; i = i + 2) {
        dstPtr[i] = (TOUT)ptr[i];
        dstPtr[i + 1] = (TOUT)ptr[i + 1];
    }
    return dstPtr;
}
//=============================================================================
template <class TIN, class TOUT>
TOUT*
promoteAsComplex(Class dstClass, const TIN* ptr, indexType count)
{
    TOUT* dstPtr = (TOUT*)ArrayOf::allocateArrayOf(dstClass, count, stringVector(), true);
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
    for (ompIndexType i = 0; i < (ompIndexType)count; i = i + 1) {
        dstPtr[i << 1] = (TOUT)ptr[i];
    }
    return dstPtr;
}
//=============================================================================
template <class TIN, class TOUT>
TOUT*
promoteComplexAsInteger(Class dstClass, const TIN* ptr, indexType count)
{
    TOUT* dstPtr = (TOUT*)ArrayOf::allocateArrayOf(dstClass, count);
    bool checkNaN = false;
    if (typeid(TOUT) != typeid(single) || typeid(TOUT) != typeid(double)) {
        if (typeid(TIN) == typeid(single) || typeid(TIN) == typeid(double)) {
            checkNaN = true;
        }
    }
    if (checkNaN) {
#if defined(_NLS_WITH_OPENMP)
#pragma omp parallel for
#endif
        for (ompIndexType i = 0; i < (ompIndexType)count; i++) {
            if (std::isnan((double)ptr[i * 2])) {
                dstPtr[i] = (TOUT)0;
            } else {
                dstPtr[i] = (TOUT)numeric_cast<TIN, TOUT>(ptr[i * 2]);
            }
        }
    } else {
        for (ompIndexType i = 0; i < (ompIndexType)count; i++) {
            dstPtr[i] = numeric_cast<TIN, TOUT>(ptr[i * 2]);
        }
    }
    return dstPtr;
}
//=============================================================================
void
ArrayOf::promoteType(Class dstClass, stringVector fNames)
{
    if (isEmpty()) {
        dp = dp->putData(dstClass, dp->dimensions, NULL, isSparse(), fNames);
        return;
    }
    if (dp->dataClass != dstClass) {
        if ((dstClass == NLS_STRING_ARRAY) || (dstClass == NLS_CELL_ARRAY)
            || (dstClass == NLS_STRUCT_ARRAY)) {
            Error(_W("Cannot convert base types to reference types."));
        }
    }
    // Do nothing for promoting to same class (no-op).
    if (isSparse()) {
        dp = dp->putData(dstClass, dp->dimensions,
            TypeConvertSparseDynamicFunction(dstClass, dp->dimensions[0], dp->dimensions[1], dp->getData(), dp->dataClass),
            true);
        return;
    }
    indexType count = getLength();
    void* dstPtr = nullptr;
    switch (dp->dataClass) {
    case NLS_HANDLE: {
        if (dstClass == NLS_HANDLE) {
            return;
        } else {
            Error(_W("Cannot convert handle-arrays to any other type."));
        }
    } break;
    case NLS_CELL_ARRAY: {
        if (dstClass == NLS_CELL_ARRAY) {
            return;
        } else {
            Error(_W("Cannot convert cell-arrays to any other type."));
        }
    } break;
    case NLS_STRUCT_ARRAY: {
        if (dstClass == NLS_STRUCT_ARRAY) {
            // TODO: Generalize this code to allow for one more field in destination
            // than in source...
            if (dp->fieldNames.size() > fNames.size()) {
                Error(_W("Cannot combine structures with different fields if the "
                         "combination "
                         "requires fields to be deleted from one of the structures."));
            }
            // We are promoting a struct array to a struct array.
            // To do so, we have to make sure that the field names work out.
            // The only thing we must check for is that every field name
            // in fieldnames is present in fnames.
            int extraCount = 0;
            int matchCount = 0;
            indexType i;
            for (i = 0; i < (int)fNames.size(); i++) {
                int64 ndx = getFieldIndex(fNames[i]);
                if (ndx == -1) {
                    extraCount++;
                } else {
                    matchCount++;
                }
            }
            // Now, matchCount should be equal to the size of fieldNames
            if (matchCount != dp->fieldNames.size()) {
                Error(_W("Cannot combine structures with different fields if the "
                         "combination "
                         "requires fields to be deleted from one of the structures."));
            }
            void* dstPtr = allocateArrayOf(dp->dataClass, getLength(), fNames, false);
            const ArrayOf* src_rp = (const ArrayOf*)dp->getData();
            ArrayOf* dst_rp = (ArrayOf*)dstPtr;
            indexType elCount(getLength());
            indexType fieldCount(dp->fieldNames.size());
            indexType newFieldCount(fNames.size());
            ;
            // Now we have to copy our existing fields into the new order...
            for (i = 0; i < fieldCount; i++) {
                int64 newNdx = getFieldIndexFromList(dp->fieldNames[i], fNames);
                for (indexType j = 0; j < elCount; j++) {
                    dst_rp[j * newFieldCount + newNdx] = src_rp[j * fieldCount + i];
                }
            }
            dp = dp->putData(dp->dataClass, dp->dimensions, dstPtr, false, fNames);
            return;
        } else {
            Error(_W("Cannot convert struct-arrays to any other type."));
        }
    } break;
    case NLS_STRING_ARRAY: {
        if (dstClass == NLS_STRING_ARRAY) {
            return;
        } else {
            Error(_W("Cannot convert string-arrays to any other type."));
        }
    } break;
    case NLS_LOGICAL: {
        const logical* sp = (const logical*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            return;
        } break;
        case NLS_UINT8: {
            dstPtr = (void*)promoteAsReal<logical, uint8>(dstClass, sp, count);
        } break;
        case NLS_INT8: {
            dstPtr = (void*)promoteAsReal<logical, int8>(dstClass, sp, count);
        } break;
        case NLS_UINT16: {
            dstPtr = (void*)promoteAsReal<logical, uint16>(dstClass, sp, count);
        } break;
        case NLS_INT16: {
            dstPtr = (void*)promoteAsReal<logical, int16>(dstClass, sp, count);
        } break;
        case NLS_UINT32: {
            dstPtr = (void*)promoteAsReal<logical, uint32>(dstClass, sp, count);
        } break;
        case NLS_INT32: {
            dstPtr = (void*)promoteAsReal<logical, int32>(dstClass, sp, count);
        } break;
        case NLS_UINT64: {
            dstPtr = (void*)promoteAsReal<logical, uint64>(dstClass, sp, count);
        } break;
        case NLS_INT64: {
            dstPtr = (void*)promoteAsReal<logical, int64>(dstClass, sp, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = (void*)promoteAsReal<logical, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = (void*)promoteAsReal<logical, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<logical, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<logical, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = (void*)promoteAsReal<logical, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_UINT8: {
        const uint8* sp = (const uint8*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<uint8, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            return;
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint8, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint8, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint8, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint8, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint8, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint8, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint8, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<uint8, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<uint8, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<uint8, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<uint8, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<uint8, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_INT8: {
        const int8* sp = (const int8*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<int8, logical>(dstClass, sp, count);
        } break;
        case NLS_INT8: {
            return;
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int8, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int8, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int8, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int8, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int8, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int8, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int8, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<int8, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<int8, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<int8, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<int8, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsComplex<int8, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_UINT16: {
        const uint16* sp = (const uint16*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsReal<uint16, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint16, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint16, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            return;
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint16, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint16, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint16, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint16, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint16, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<uint16, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<uint16, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<uint16, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<uint16, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<uint16, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_INT16: {
        const int16* sp = (const int16*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<int16, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int16, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int16, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            return;
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int16, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int16, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int16, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int16, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int16, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<int16, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<int16, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<int16, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<int16, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<int16, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_UINT32: {
        const uint32* sp = (const uint32*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<uint32, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint32, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint32, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint32, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint32, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            return;
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint32, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint32, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint32, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<uint32, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<uint32, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<uint32, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<uint32, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<uint32, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_INT32: {
        const int32* sp = (const int32*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<int32, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int32, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int32, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int32, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int32, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            return;
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int32, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int32, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int32, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<int32, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<int32, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<int32, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<int32, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<int32, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_UINT64: {
        const uint64* sp = (const uint64*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<uint64, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint64, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint64, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint64, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint64, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint64, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint64, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            return;
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<uint64, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<uint64, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<uint64, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<uint64, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<uint64, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<uint64, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_INT64: {
        const int64* sp = (const int64*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<int64, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int64, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int64, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int64, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int64, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int64, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int64, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            return;
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<int64, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<int64, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<int64, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<int64, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<int64, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<int64, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_SINGLE: {
        const single* sp = (const single*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<single, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<single, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            return;
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteAsReal<single, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<single, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<single, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<single, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_DOUBLE: {
        const double* sp = (const double*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteAsLogical<double, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<double, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteAsReal<double, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            return;
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteAsComplex<double, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteAsComplex<double, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteAsReal<double, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_SCOMPLEX: {
        const single* sp = (const single*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteComplexAsLogical<single, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = promoteComplexAsInteger<single, uint8>(dstClass, sp, count);
        } break;
        case NLS_INT8: {
            dstPtr = promoteComplexAsInteger<single, int8>(dstClass, sp, count);
        } break;
        case NLS_UINT16: {
            dstPtr = promoteComplexAsInteger<single, uint16>(dstClass, sp, count);
        } break;
        case NLS_INT16: {
            dstPtr = promoteComplexAsInteger<single, int16>(dstClass, sp, count);
        } break;
        case NLS_UINT32: {
            dstPtr = promoteComplexAsInteger<single, uint32>(dstClass, sp, count);
        } break;
        case NLS_INT32: {
            dstPtr = promoteComplexAsInteger<single, int32>(dstClass, sp, count);
        } break;
        case NLS_UINT64: {
            dstPtr = promoteComplexAsInteger<single, uint64>(dstClass, sp, count);
        } break;
        case NLS_INT64: {
            dstPtr = promoteComplexAsInteger<single, int64>(dstClass, sp, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteComplexAsReal<single, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteComplexAsReal<single, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            return;
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = promoteComplexAsComplex<single, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteComplexAsReal<single, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_DCOMPLEX: {
        const double* sp = (const double*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = promoteComplexAsLogical<double, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = promoteComplexAsInteger<double, uint8>(dstClass, sp, count);
        } break;
        case NLS_INT8: {
            dstPtr = promoteComplexAsInteger<double, int8>(dstClass, sp, count);
        } break;
        case NLS_UINT16: {
            dstPtr = promoteComplexAsInteger<double, uint16>(dstClass, sp, count);
        } break;
        case NLS_INT16: {
            dstPtr = promoteComplexAsInteger<double, int16>(dstClass, sp, count);
        } break;
        case NLS_UINT32: {
            dstPtr = promoteComplexAsInteger<double, uint32>(dstClass, sp, count);
        } break;
        case NLS_INT32: {
            dstPtr = promoteComplexAsInteger<double, int32>(dstClass, sp, count);
        } break;
        case NLS_UINT64: {
            dstPtr = promoteComplexAsInteger<double, uint64>(dstClass, sp, count);
        } break;
        case NLS_INT64: {
            dstPtr = promoteComplexAsInteger<double, int64>(dstClass, sp, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = promoteComplexAsReal<double, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = promoteComplexAsReal<double, double>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            return;
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = promoteComplexAsComplex<double, single>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            dstPtr = promoteComplexAsReal<double, charType>(dstClass, sp, count);
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    case NLS_CHAR: {
        charType* sp = (charType*)dp->getData();
        switch (dstClass) {
        case NLS_LOGICAL: {
            dstPtr = (void*)promoteAsLogical<charType, logical>(dstClass, sp, count);
        } break;
        case NLS_UINT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, uint8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT8: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, int8>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, uint16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT16: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, int16>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, uint32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT32: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, int32>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_UINT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, uint64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_INT64: {
            dstPtr = ArrayOf::allocateArrayOf(dstClass, count);
            saturate<charType, int64>(dp->dataClass, dstClass, dp->getData(), dstPtr, count);
        } break;
        case NLS_SINGLE: {
            dstPtr = (void*)promoteAsReal<charType, single>(dstClass, sp, count);
        } break;
        case NLS_DOUBLE: {
            dstPtr = (void*)promoteAsReal<charType, double>(dstClass, sp, count);
        } break;
        case NLS_SCOMPLEX: {
            dstPtr = (void*)promoteAsComplex<charType, single>(dstClass, sp, count);
        } break;
        case NLS_DCOMPLEX: {
            dstPtr = (void*)promoteAsComplex<charType, double>(dstClass, sp, count);
        } break;
        case NLS_CHAR: {
            return;
        } break;
        default: {
            Error(_("Type not managed."));
        } break;
        }
    } break;
    default: {
        Error(_W("Type not managed."));
    } break;
    }
    dp = dp->putData(dstClass, dp->dimensions, dstPtr);
}
//=============================================================================
void
ArrayOf::promoteType(Class dstClass)
{
    stringVector dummy;
    promoteType(dstClass, dummy);
}
//=============================================================================
}
//=============================================================================
