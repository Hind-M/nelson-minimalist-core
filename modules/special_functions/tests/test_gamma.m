%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
%=============================================================================
% This file is part of the Nelson.
%=============================================================================
% LICENCE_BLOCK_BEGIN
% This program is free software; you can redistribute it and/or
% modify it under the terms of the GNU Lesser General Public
% License as published by the Free Software Foundation; either
% version 2.1 of the License, or (at your option) any later version.
%
% Alternatively, you can redistribute it and/or
% modify it under the terms of the GNU General Public License as
% published by the Free Software Foundation; either version 2 of
% the License, or (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU Lesser General Public License for more details.
%
% You should have received a copy of the GNU Lesser General Public
% License along with this program. If not, see <http://www.gnu.org/licenses/>.
% LICENCE_BLOCK_END
%=============================================================================
assert_isequal(nargin('gamma'), 1)
assert_isequal(nargout('gamma'), 1)
%=============================================================================
R = gamma(0);
REF = Inf;
assert_isequal(R, REF);
%=============================================================================
R = gamma(1);
REF = 1;
assert_isequal(R, REF);
%=============================================================================
R = gamma(Inf);
REF = Inf;
assert_isequal(R, REF);
%=============================================================================
R = gamma(-Inf);
REF = Inf;
assert_isequal(R, REF);
%=============================================================================
R = gamma(NaN);
REF = NaN;
assert_isequal(R, REF);
%=============================================================================
R = gamma(0.005);
REF = 199.4277;
assert_isapprox(R, REF, 1e-4);
%=============================================================================
R = gamma([-3.5:3.5]);
REF = [0.2701   -0.9453    2.3633   -3.5449    1.7725    0.8862    1.3293    3.3234];
assert_isapprox(R, REF, 1e-4);
%=============================================================================
R = gamma(2);
REF = 1;
assert_isequal(R, REF);
%=============================================================================
R = gamma(3);
REF = 2;
assert_isequal(R, REF);
%=============================================================================
R = gamma(172);
REF = Inf;
assert_isequal(R, REF);
%=============================================================================
R = gamma(single(36));
REF = single(Inf);
assert_isequal(R, REF);
%=============================================================================
R = gamma(single([-3.5:3.5]));
REF = single([0.2701   -0.9453    2.3633   -3.5449    1.7725    0.8862    1.3293    3.3234]);
assert_isapprox(R, REF, 1e-4);
%=============================================================================
R = gamma([-1.e308 -170.6 -1.5 -2 -1 -0.5 1.e-307 1.e-100 1.e-30]);
REF = [Inf -2.082900836150298e-308 2.363271801207355e+00 Inf Inf -3.544907701811032e+00 1.000000000000000e+307 1.000000000000000e+100 9.999999999999999e+29];
assert_isapprox(R, REF, 1e-4);
%=============================================================================