%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
%=============================================================================
% This file is part of the Nelson.
%=============================================================================
% LICENCE_BLOCK_BEGIN
% SPDX-License-Identifier: LGPL-3.0-or-later
% LICENCE_BLOCK_END
%=============================================================================
R = sinc(ones(2, 0));
REF = zeros(2, 0);
assert_isequal(R, REF);
%=============================================================================
R = sinc(pi);
REF = -0.0436;
assert_isapprox(R, REF, 1e-4);
%=============================================================================
R = sinc(0);
REF = 1;
assert_isequal(R, REF);
%=============================================================================
R = sinc(0.5);
REF = 0.6366;
assert_isapprox(R, REF, 1e-4);
%=============================================================================