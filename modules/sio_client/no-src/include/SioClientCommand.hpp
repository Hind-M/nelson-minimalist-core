//=============================================================================
// Copyright (c) 2016-2018 Allan CORNET (Nelson)
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
#include <string>
#include "nlsSio_client_exports.h"
//=============================================================================
namespace Nelson {
class NLSSIO_CLIENT_IMPEXP SioClientCommand
{
public:
    static SioClientCommand*
    getInstance();
    void
    reply(std::string stringToReply);
    std::string
    getCommand();
    void
    updateCommand(std::string command);
    bool
    isInitialized();
    bool
    create(const std::string& ipAddress);
private:
    SioClientCommand();
    bool
    createConnection(const std::string& ipAddress);
    static SioClientCommand* m_pInstance;

};
}
//=============================================================================