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
A = int32([9 0 -7 5 3 8 -10 4 2]);
R = sort(A);
REF = int32([-10    -7     0     2     3     4     5     8     9]);
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]);
R = sort(A, 1);
REF = int32([ 9     0    -7     5     3     8   -10     4     2]);
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]);
R = sort(A, 2);
REF = int32([-10    -7     0     2     3     4     5     8     9]);
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]');
R = sort(A, 1);
REF = int32([ -10; -7; 0; 2; 3; 4; 5; 8; 9]);
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]');
R = sort(A, 2);
REF = int32([ 9; 0; -7; 5; 3; 8; -10; 4; 2]);
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]);
[B, R] = sort(A);
REF = [ 7     3     2     9     5     8     4     6     1];
assert_isequal(R, REF)
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]);
[B, R] = sort(A, 1);
REF = [1     1     1     1     1     1     1     1     1];
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]);
[B, R] = sort(A, 2);
REF = [7     3     2     9     5     8     4     6     1];
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]');
[B, R] = sort(A, 1);
REF = [    7;     3;     2;     9;     5;     8;     4;     6;     1];
assert_isequal(R, REF);
%=============================================================================
A = int32([9 0 -7 5 3 8 -10 4 2]');
[B, R] = sort(A, 2);
REF = [1;     1;     1;     1;     1;     1;     1;     1;     1];
assert_isequal(R, REF);
%=============================================================================
A = int32([1 3 2; 4 6 8; 5 9 7]);
R = sort(A, 'ascend');
REF = [   1   3   2;
   4   6   7;
   5   9   8];
REF = int32(REF);
assert_isequal(R, REF);
%=============================================================================
A = int32([1 3 2; 4 6 8; 5 9 7]);
R = sort(A, 'descend');
REF = [   5   9   8;
   4   6   7;
   1   3   2];
REF = int32(REF);
assert_isequal(R, REF);
%=============================================================================
A = int32([1 3 2; 4 6 8; 5 9 7]);
[R, I] = sort(A, 'ascend');
REF = [     1     1     1;
     2     2     3;
     3     3     2];
assert_isequal(I, REF);
%=============================================================================
A = int32([1 3 2; 4 6 8; 5 9 7]);
[R, I] = sort(A, 'descend');
REF = [     3     3     2;
     2     2     3;
     1     1     1];
assert_isequal(I, REF);
%=============================================================================
A = int32([ 95    45    92    41    13     1    84;
    23     1    73    89    20    74    52;
    60    82    17     5    19    44    20;
    48    44    40    35    60    93    67;
    89    61    93    81    27    46    83;
    76    79    91     0    19    41     1]);
R = sort(A, 1);
REF = int32([ 23     1    17     0    13     1     1;
    48    44    40     5    19    41    20;
    60    45    73    35    19    44    52;
    76    61    91    41    20    46    67;
    89    79    92    81    27    74    83;
    95    82    93    89    60    93    84]);
assert_isequal(R, REF);
%=============================================================================
R = sort(A, 2, 'descend');
REF = int32([   95   92   84   45   41   13    1;
   89   74   73   52   23   20    1;
   82   60   44   20   19   17    5;
   93   67   60   48   44   40   35;
   93   89   83   81   61   46   27;
   91   79   76   41   19    1    0]);
assert_isequal(R, REF);
%=============================================================================