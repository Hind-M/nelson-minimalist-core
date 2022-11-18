//=============================================================================
// This file is part of the Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
// Generated by Nelson Interface Generator 1.0.0
//=============================================================================
#include <algorithm>
#include "slicot_mb05odBuiltin.hpp"
#include "Error.hpp"
#include "i18n.hpp"
//=============================================================================
using namespace Nelson;
//=============================================================================
#ifdef __cplusplus
extern "C"
{
#endif
    extern int
    mb05od_(const char* BALANC, int* N, int* NDIAG, double* DELTA, double* A, int* LDA, int* MDIG,
        int* IDIG, int* IWORK, double* DWORK, int* LDWORK, int* IWARN, int* INFO);
#ifdef __cplusplus
}
#endif
//=============================================================================
// [A_OUT, MDIG, IDIG, IWARN, INFO] = slicot_mb05od(BALANC, NDIAG, DELTA, A_IN)
//=============================================================================
ArrayOfVector
Nelson::SlicotGateway::slicot_mb05odBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    ArrayOfVector retval;
    nargoutcheck(nLhs, 0, 5);
    nargincheck(argIn, 4, 4);
    // INPUT VARIABLES
    ArrayOf BALANC = argIn[0];
    Dimensions dimsBALANC = BALANC.getDimensions();
    std::string BALANC_string = BALANC.getContentAsCString();
    const char* BALANC_ptr = BALANC_string.c_str();
    ArrayOf NDIAG = argIn[1];
    Dimensions dimsNDIAG = NDIAG.getDimensions();
    NDIAG.promoteType(NLS_INT32);
    int* NDIAG_ptr = (int*)NDIAG.getDataPointer();
    ArrayOf DELTA = argIn[2];
    Dimensions dimsDELTA = DELTA.getDimensions();
    DELTA.promoteType(NLS_DOUBLE);
    double* DELTA_ptr = (double*)DELTA.getDataPointer();
    // IN/OUT VARIABLES
    ArrayOf A = argIn[3];
    Dimensions dimsA = A.getDimensions();
    A.promoteType(NLS_DOUBLE);
    ArrayOf A_output = A;
    A_output.ensureSingleOwner();
    double* A_output_ptr = (double*)A_output.getDataPointer();
    // LOCAL VARIABLES
    ArrayOf N = ArrayOf::int32VectorConstructor(1);
    int* N_ptr = (int*)N.getDataPointer();
    N_ptr[0] = (int)A.getColumns();
    ArrayOf LDA = ArrayOf::int32VectorConstructor(1);
    int* LDA_ptr = (int*)LDA.getDataPointer();
    LDA_ptr[0] = std::max(1, (int)N.getContentAsInteger32Scalar());
    ArrayOf IWORK = ArrayOf::int32Matrix2dConstructor(1, (int)N.getContentAsInteger32Scalar());
    int* IWORK_ptr = (int*)IWORK.getDataPointer();
    ArrayOf DWORK = ArrayOf::doubleMatrix2dConstructor(1,
        (int)N.getContentAsInteger32Scalar()
                * (2 * (int)N.getContentAsInteger32Scalar()
                    + (int)NDIAG.getContentAsInteger32Scalar() + 1)
            + (int)NDIAG.getContentAsInteger32Scalar());
    double* DWORK_ptr = (double*)DWORK.getDataPointer();
    ArrayOf LDWORK = ArrayOf::int32VectorConstructor(1);
    int* LDWORK_ptr = (int*)LDWORK.getDataPointer();
    LDWORK_ptr[0] = (int)N.getContentAsInteger32Scalar()
            * (2 * (int)N.getContentAsInteger32Scalar() + (int)NDIAG.getContentAsInteger32Scalar()
                + 1)
        + (int)NDIAG.getContentAsInteger32Scalar();
    // OUTPUT VARIABLES
    ArrayOf MDIG_output = ArrayOf::int32VectorConstructor(1);
    int* MDIG_output_ptr = (int*)MDIG_output.getDataPointer();
    ArrayOf IDIG_output = ArrayOf::int32VectorConstructor(1);
    int* IDIG_output_ptr = (int*)IDIG_output.getDataPointer();
    ArrayOf IWARN_output = ArrayOf::int32VectorConstructor(1);
    int* IWARN_output_ptr = (int*)IWARN_output.getDataPointer();
    ArrayOf INFO_output = ArrayOf::int32VectorConstructor(1);
    int* INFO_output_ptr = (int*)INFO_output.getDataPointer();
    // CHECK INPUT VARIABLES DIMENSIONS
    if (!dimsBALANC.isScalar()) {
        Error(_W("Input argument #1: scalar expected."));
    }
    if (!dimsNDIAG.isScalar()) {
        Error(_W("Input argument #2: scalar expected."));
    }
    if (!dimsDELTA.isScalar()) {
        Error(_W("Input argument #3: scalar expected."));
    }
    Dimensions dimsA_expected(
        std::max(1, (int)N.getContentAsInteger32Scalar()), (int)N.getContentAsInteger32Scalar());
    if (!dimsA.equals(dimsA_expected)) {
        Error(_("Input argument #4: wrong size.") + " " + dimsA_expected.toString() + " "
            + "expected" + ".");
    }
    // CALL EXTERN FUNCTION
    try {
        mb05od_(BALANC_ptr, N_ptr, NDIAG_ptr, DELTA_ptr, A_output_ptr, LDA_ptr, MDIG_output_ptr,
            IDIG_output_ptr, IWORK_ptr, DWORK_ptr, LDWORK_ptr, IWARN_output_ptr, INFO_output_ptr);
    } catch (const std::runtime_error&) {
        Error("mb05od function fails.");
    }
    // ASSIGN OUTPUT VARIABLES
    if (nLhs > 0) {
        retval << A_output;
    }
    if (nLhs > 1) {
        retval << MDIG_output;
    }
    if (nLhs > 2) {
        retval << IDIG_output;
    }
    if (nLhs > 3) {
        retval << IWARN_output;
    }
    if (nLhs > 4) {
        retval << INFO_output;
    }
    return retval;
}
//=============================================================================
