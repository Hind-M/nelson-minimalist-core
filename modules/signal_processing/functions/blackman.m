%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
%=============================================================================
% This file is part of the Nelson.
%=============================================================================
% LICENCE_BLOCK_BEGIN
% SPDX-License-Identifier: LGPL-3.0-or-later
% LICENCE_BLOCK_END
%=============================================================================
function c = blackman(varargin)
  % https://www.sciencedirect.com/topics/engineering/blackman-window
  narginchk(1, 2);
  m = varargin{1};
  isPositiveIntegerOrEmpty = isempty(m) || (isscalar(m) && (m == fix (m)) && (m > 0));
  if ~isPositiveIntegerOrEmpty
    error(_('M must be a positive integer.'));
  end
  N = m - 1;
  
  if (nargin == 2)
    opt = varargin{2};
  else
    opt = 'symetric';
  end
  
  isPeriodic = strcmp(opt, 'periodic');
  if isPeriodic
    N = m;
  else
    isSymetric = strcmp(opt, 'symetric');
    if ~isSymetric
      msg = _('"periodic" or "symmetric" expected.');
      error (msg);
    end
  end
  
  if isempty(m)
    c = zeros(0, 1);
  elseif (m ~= 1)
    m = m - 1;
    k = (0 : m)' / N;
    c = 0.42 - (0.5 * cos (2 * pi * k)) + (0.08 * cos (4 * pi * k));
  else
    c = 1;
  end
end
%=============================================================================
