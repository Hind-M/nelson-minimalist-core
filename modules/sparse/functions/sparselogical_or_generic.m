%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
%=============================================================================
% This file is part of the Nelson.
%=============================================================================
% LICENCE_BLOCK_BEGIN
% SPDX-License-Identifier: LGPL-3.0-or-later
% LICENCE_BLOCK_END
%=============================================================================
function R = sparselogical_or_generic(A, B)
  % internal function (overload)
  if (issparse(B))
    R = sparselogical_or_sparselogical(A, B);
  else
    AA = full(A);
    R = AA | B;
  end
end
%=============================================================================
