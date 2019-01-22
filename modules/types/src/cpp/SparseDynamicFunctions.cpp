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
#include "SparseDynamicFunctions.hpp"
#include "dynamic_library.hpp"
#include "Error.hpp"
#include <boost/function.hpp>
#include <cstdio>
//=============================================================================
static library_handle nlsSparseHandleDynamicLibrary = nullptr;
static bool bFirstDynamicLibraryCall = true;
//=============================================================================
static void
initSparseDynamicLibrary()
{
    if (bFirstDynamicLibraryCall) {
        std::string fullpathSparseSharedLibrary = "libnlsSparse" + get_dynamic_library_extension();
#ifdef _MSC_VER
        char* buf;
        try {
            buf = new char[MAX_PATH];
        } catch (const std::bad_alloc&) {
            buf = nullptr;
        }
        if (buf) {
            DWORD dwRet = ::GetEnvironmentVariableA("NELSON_BINARY_PATH", buf, MAX_PATH);
            if (dwRet) {
                fullpathSparseSharedLibrary
                    = std::string(buf) + std::string("/") + fullpathSparseSharedLibrary;
            }
            delete[] buf;
        }
#else
        char const* tmp = std::getenv("NELSON_BINARY_PATH");
        if (tmp != nullptr) {
            fullpathSparseSharedLibrary
                = std::string(tmp) + std::string("/") + fullpathSparseSharedLibrary;
        }
#endif
        nlsSparseHandleDynamicLibrary = load_dynamic_library(fullpathSparseSharedLibrary);
        if (nlsSparseHandleDynamicLibrary) {
            bFirstDynamicLibraryCall = false;
        }
    }
}
//=============================================================================
void*
EyeSparseMatrixConstructorDynamicFunction(Class dclass, indexType rows, indexType cols)
{
    using PROC_EyeSparseMatrixConstructor = void* (*)(Class, indexType, indexType);
    static PROC_EyeSparseMatrixConstructor Eigen_EyeSparseMatrixConstructorPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_EyeSparseMatrixConstructorPtr) {
        Eigen_EyeSparseMatrixConstructorPtr = reinterpret_cast<PROC_EyeSparseMatrixConstructor>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_EyeSparseMatrixConstructor"));
        if (!Eigen_EyeSparseMatrixConstructorPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_EyeSparseMatrixConstructorPtr(dclass, rows, cols);
}
//=============================================================================
void*
LogicalSparseMatrixConstructorDynamicFunction(indexType rows, indexType cols, bool bMotif)
{
    using PROC_LogicalSparseMatrixConstructor = void* (*)(indexType, indexType, bool);
    static PROC_LogicalSparseMatrixConstructor Eigen_LogicalSparseMatrixConstructorPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_LogicalSparseMatrixConstructorPtr) {
        Eigen_LogicalSparseMatrixConstructorPtr
            = reinterpret_cast<PROC_LogicalSparseMatrixConstructor>(get_function(
                nlsSparseHandleDynamicLibrary, "Eigen_LogicalSparseMatrixConstructor"));
        if (!Eigen_LogicalSparseMatrixConstructorPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_LogicalSparseMatrixConstructorPtr(rows, cols, bMotif);
}
//=============================================================================
void
DeleteSparseMatrixDynamicFunction(Class dclass, indexType rows, indexType cols, void* cp)
{
    using PROC_DeleteSparseMatrix = void (*)(Class, indexType, indexType, void**);
    static PROC_DeleteSparseMatrix Eigen_DeleteSparseMatrixPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_DeleteSparseMatrixPtr) {
        Eigen_DeleteSparseMatrixPtr = reinterpret_cast<PROC_DeleteSparseMatrix>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_DeleteSparseMatrix"));
        if (!Eigen_DeleteSparseMatrixPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    Eigen_DeleteSparseMatrixPtr(dclass, rows, cols, &cp);
}
//=============================================================================
void*
MakeDenseArrayOfDynamicFunction(Class dclass, indexType rows, indexType cols, const void* cp)
{
    using PROC_MakeDenseArrayOf = void* (*)(Class, indexType, indexType, const void*);
    static PROC_MakeDenseArrayOf Eigen_MakeDenseArrayOfPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_MakeDenseArrayOfPtr) {
        Eigen_MakeDenseArrayOfPtr = reinterpret_cast<PROC_MakeDenseArrayOf>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_MakeDenseArrayOf"));
        if (!Eigen_MakeDenseArrayOfPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_MakeDenseArrayOfPtr(dclass, rows, cols, cp);
}
//=============================================================================
void*
MakeSparseArrayOfDynamicFunction(Class dclass, indexType rows, indexType cols, const void* cp)
{
    using PROC_MakeSparseArrayOf = void* (*)(Class, indexType, indexType, const void*);
    static PROC_MakeSparseArrayOf Eigen_MakeSparseArrayOfPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_MakeSparseArrayOfPtr) {
        Eigen_MakeSparseArrayOfPtr = reinterpret_cast<PROC_MakeSparseArrayOf>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_MakeSparseArrayOf"));
        if (!Eigen_MakeSparseArrayOfPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_MakeSparseArrayOfPtr(dclass, rows, cols, cp);
}
//=============================================================================
void*
TypeConvertSparseDynamicFunction(
    Class dclass, indexType rows, indexType cols, const void* cp, Class oclass)
{
    using PROC_TypeConvertSparse = void* (*)(Class, indexType, indexType, const void*, Class);
    static PROC_TypeConvertSparse Eigen_TypeConvertSparsePtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_TypeConvertSparsePtr) {
        Eigen_TypeConvertSparsePtr = reinterpret_cast<PROC_TypeConvertSparse>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_TypeConvertSparse"));
        if (!Eigen_TypeConvertSparsePtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_TypeConvertSparsePtr(dclass, rows, cols, cp, oclass);
}
//=============================================================================
void*
CopySparseMatrixDynamicFunction(Class dclass, indexType rows, indexType cols, const void* cp)
{
    using PROC_CopySparseMatrix = void* (*)(Class, indexType, indexType, const void*);
    static PROC_CopySparseMatrix Eigen_CopySparseMatrixPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_CopySparseMatrixPtr) {
        Eigen_CopySparseMatrixPtr = reinterpret_cast<PROC_CopySparseMatrix>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_CopySparseMatrix"));
        if (!Eigen_CopySparseMatrixPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_CopySparseMatrixPtr(dclass, rows, cols, cp);
}
//=============================================================================
indexType
CountNonzerosMaxDynamicFunction(Class dclass, indexType rows, indexType cols, const void* cp)
{
    using PROC_CountNonzerosMax = indexType (*)(Class, indexType, indexType, const void*);
    static PROC_CountNonzerosMax Eigen_CountNonzerosMaxPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_CountNonzerosMaxPtr) {
        Eigen_CountNonzerosMaxPtr = reinterpret_cast<PROC_CountNonzerosMax>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_CountNonzerosMax"));
        if (!Eigen_CountNonzerosMaxPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_CountNonzerosMaxPtr(dclass, rows, cols, cp);
}
//=============================================================================
indexType
CountNonzerosDynamicFunction(Class dclass, indexType rows, indexType cols, const void* cp)
{
    using PROC_CountNonzeros = indexType (*)(Class, indexType, indexType, const void*);
    static PROC_CountNonzeros Eigen_CountNonzerosPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_CountNonzerosPtr) {
        Eigen_CountNonzerosPtr = reinterpret_cast<PROC_CountNonzeros>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_CountNonzeros"));
        if (!Eigen_CountNonzerosPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_CountNonzerosPtr(dclass, rows, cols, cp);
}
//=============================================================================
void*
SparseMatrixConstructorDynamicFunction(
    Class dclass, indexType rows, indexType cols, ArrayOfMatrix m)
{
    using PROC_SparseMatrixConstructor = void* (*)(Class, indexType, indexType, ArrayOfMatrix);
    static PROC_SparseMatrixConstructor Eigen_SparseMatrixConstructorPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_SparseMatrixConstructorPtr) {
        Eigen_SparseMatrixConstructorPtr = reinterpret_cast<PROC_SparseMatrixConstructor>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_SparseMatrixConstructor"));
        if (!Eigen_SparseMatrixConstructorPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_SparseMatrixConstructorPtr(dclass, rows, cols, m);
}
//=============================================================================
void*
GetSparseVectorSubsetsDynamicFunction(Class dclass, indexType rows, indexType cols, const void* src,
    const indexType* indx, indexType irows, indexType icols)
{
    using PROC_GetSparseVectorSubsets = void* (*)(Class, indexType, indexType, const void*,
        const indexType*, indexType, indexType);
    static PROC_GetSparseVectorSubsets Eigen_GetSparseVectorSubsetsPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_GetSparseVectorSubsetsPtr) {
        Eigen_GetSparseVectorSubsetsPtr = reinterpret_cast<PROC_GetSparseVectorSubsets>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_GetSparseVectorSubsets"));
        if (!Eigen_GetSparseVectorSubsetsPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_GetSparseVectorSubsetsPtr(dclass, rows, cols, src, indx, irows, icols);
}
//=============================================================================
void*
GetSparseNDimSubsetsDynamicFunction(Class dclass, indexType rows, indexType cols, const void* src,
    const indexType* rindx, indexType irows, const indexType* cindx, indexType icols)
{
    using PROC_GetSparseNDimSubsets = void* (*)(Class, indexType, indexType, const void*,
        const indexType*, indexType, const indexType*, indexType);
    static PROC_GetSparseNDimSubsets Eigen_GetSparseNDimSubsetsPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_GetSparseNDimSubsetsPtr) {
        Eigen_GetSparseNDimSubsetsPtr = reinterpret_cast<PROC_GetSparseNDimSubsets>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_GetSparseNDimSubsets"));
        if (!Eigen_GetSparseNDimSubsetsPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_GetSparseNDimSubsetsPtr(dclass, rows, cols, src, rindx, irows, cindx, icols);
}
//=============================================================================
void*
SetSparseVectorSubsetsDynamicFunction(Class dclass, indexType& rows, indexType& cols,
    const void* src, const indexType* indx, indexType irows, indexType icols, const void* data,
    int advance)
{
    using PROC_SetSparseVectorSubsets = void* (*)(Class, indexType&, indexType&, const void*,
        const indexType*, indexType, indexType, const void*, int);
    static PROC_SetSparseVectorSubsets Eigen_SetSparseVectorSubsetsPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_SetSparseVectorSubsetsPtr) {
        Eigen_SetSparseVectorSubsetsPtr = reinterpret_cast<PROC_SetSparseVectorSubsets>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_SetSparseVectorSubsets"));
        if (!Eigen_SetSparseVectorSubsetsPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_SetSparseVectorSubsetsPtr(
        dclass, rows, cols, src, indx, irows, icols, data, advance);
}
//=============================================================================
void*
SetSparseNDimSubsetsDynamicFunction(Class dclass, indexType& rows, indexType& cols, const void* src,
    const indexType* rindx, indexType irows, const indexType* cindx, indexType icols,
    const void* data, int advance)
{
    using PROC_SetSparseNDimSubsets = void* (*)(Class, indexType&, indexType&, const void*,
        const indexType*, indexType, const indexType*, indexType, const void*, int);
    static PROC_SetSparseNDimSubsets Eigen_SetSparseNDimSubsetsPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_SetSparseNDimSubsetsPtr) {
        Eigen_SetSparseNDimSubsetsPtr = reinterpret_cast<PROC_SetSparseNDimSubsets>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_SetSparseNDimSubsets"));
        if (!Eigen_SetSparseNDimSubsetsPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_SetSparseNDimSubsetsPtr(
        dclass, rows, cols, src, rindx, irows, cindx, icols, data, advance);
}
//=============================================================================
void*
GetSparseScalarElementDynamicFunction(
    Class dclass, indexType rows, indexType cols, const void* src, indexType rindx, indexType cindx)
{
    using PROC_GetSparseScalarElement
        = void* (*)(Class, indexType, indexType, const void*, indexType, indexType);
    static PROC_GetSparseScalarElement Eigen_GetSparseScalarElementPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_GetSparseScalarElementPtr) {
        Eigen_GetSparseScalarElementPtr = reinterpret_cast<PROC_GetSparseScalarElement>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_GetSparseScalarElement"));
        if (!Eigen_GetSparseScalarElementPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_GetSparseScalarElementPtr(dclass, rows, cols, src, rindx, cindx);
}
//=============================================================================
void*
DeleteSparseMatrixColsDynamicFunction(
    Class dclass, indexType rows, indexType cols, const void* cp, bool* dmap)
{
    using PROC_DeleteSparseMatrixCols = void* (*)(Class, indexType, indexType, const void*, bool*);
    static PROC_DeleteSparseMatrixCols Eigen_DeleteSparseMatrixColsPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_DeleteSparseMatrixColsPtr) {
        Eigen_DeleteSparseMatrixColsPtr = reinterpret_cast<PROC_DeleteSparseMatrixCols>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_DeleteSparseMatrixCols"));
        if (!Eigen_DeleteSparseMatrixColsPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_DeleteSparseMatrixColsPtr(dclass, rows, cols, cp, dmap);
}
//=============================================================================
void*
DeleteSparseMatrixRowsDynamicFunction(
    Class dclass, indexType rows, indexType cols, const void* cp, bool* dmap)
{
    using PROC_DeleteSparseMatrixRows = void* (*)(Class, indexType, indexType, const void*, bool*);
    static PROC_DeleteSparseMatrixRows Eigen_DeleteSparseMatrixRowsPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_DeleteSparseMatrixRowsPtr) {
        Eigen_DeleteSparseMatrixRowsPtr = reinterpret_cast<PROC_DeleteSparseMatrixRows>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_DeleteSparseMatrixRows"));
        if (!Eigen_DeleteSparseMatrixRowsPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_DeleteSparseMatrixRowsPtr(dclass, rows, cols, cp, dmap);
}
//=============================================================================
void*
DeleteSparseMatrixVectorSubsetDynamicFunction(Class dclass, indexType& rows, indexType& cols,
    const void* cp, const indexType* todel, indexType delete_len)
{
    using PROC_DeleteSparseMatrixVectorSubset
        = void* (*)(Class, indexType&, indexType&, const void*, const indexType*, indexType);
    static PROC_DeleteSparseMatrixVectorSubset Eigen_DeleteSparseMatrixVectorSubsetPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_DeleteSparseMatrixVectorSubsetPtr) {
        Eigen_DeleteSparseMatrixVectorSubsetPtr
            = reinterpret_cast<PROC_DeleteSparseMatrixVectorSubset>(get_function(
                nlsSparseHandleDynamicLibrary, "Eigen_DeleteSparseMatrixVectorSubset"));
        if (!Eigen_DeleteSparseMatrixVectorSubsetPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_DeleteSparseMatrixVectorSubsetPtr(dclass, rows, cols, cp, todel, delete_len);
}
//=============================================================================
void*
ReshapeSparseMatrixDynamicFunction(Class dclass, indexType rows, indexType cols, indexType newrows,
    indexType newcols, const void* cp)
{
    using PROC_Eigen_ReshapeSparseMatrix
        = void* (*)(Class, indexType, indexType, indexType, indexType, const void*);
    static PROC_Eigen_ReshapeSparseMatrix Eigen_ReshapeSparseMatrixPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_ReshapeSparseMatrixPtr) {
        Eigen_ReshapeSparseMatrixPtr = reinterpret_cast<PROC_Eigen_ReshapeSparseMatrix>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_ReshapeSparseMatrix"));
        if (!Eigen_ReshapeSparseMatrixPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_ReshapeSparseMatrixPtr(dclass, rows, cols, newrows, newcols, cp);
}
//=============================================================================
void *SparseToIJVDynamicFunction(Class dclass, indexType rows, indexType cols, const void* cp, indexType*& I, indexType*& J, int& nnz)
{
    using PROC_Eigen_SparseToIJV
        = void* (*)(Class, indexType, indexType,
        const void*, indexType*&, indexType*&, int&);
    static PROC_Eigen_SparseToIJV Eigen_SparseToIJVPtr = nullptr;
    initSparseDynamicLibrary();
    if (!Eigen_SparseToIJVPtr) {
        Eigen_SparseToIJVPtr = reinterpret_cast<PROC_Eigen_SparseToIJV>(
            get_function(nlsSparseHandleDynamicLibrary, "Eigen_SparseToIJV"));
        if (!Eigen_SparseToIJVPtr) {
            Error(_W("Sparse Function not loaded."));
        }
    }
    return Eigen_SparseToIJVPtr(dclass, rows, cols, cp, I, J, nnz);
}
//=============================================================================
