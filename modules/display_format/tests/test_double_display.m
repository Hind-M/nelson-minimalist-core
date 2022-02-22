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
% <--CHECK REF-->
% <--ENGLISH IMPOSED-->
%=============================================================================
format short
%==============================================================================
eps
%==============================================================================
pi
%==============================================================================
NaN
%==============================================================================
Inf
%==============================================================================
[0 eps]
%==============================================================================
[pi*i() 3; pi() pi*2]
%==============================================================================
[1 2 3]
%==============================================================================
[1 1e8; 1e8 1]
%==============================================================================
[1 1e9; 1e9 1]
%==============================================================================
eye(5, 5)
%==============================================================================
eye(5, 5) * 0.1
%==============================================================================
[1 1; -1e8 1]
%==============================================================================
[1 1e9; -1e9 1]
%==============================================================================
rng('default')
A = rand(5, 3)
B = rand(5, 3)
A + B * i
%==============================================================================
eye(5, 3) + eye(5, 3) * i
%==============================================================================
eye(5, 3) * 1000 + eye(5, 3) * 1000 * i
%==============================================================================