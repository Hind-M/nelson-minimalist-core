//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
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
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include "PathFunc.hpp"
#include "characters_encoding.hpp"
#include "MxGetExtension.hpp"
#include "Error.hpp"
//=============================================================================
namespace Nelson {
//=============================================================================
bool
PathFunc::isdir(const std::wstring& path)
{
    boost::filesystem::path data_dir(path);
    bool bRes = false;
    try {
        bRes = boost::filesystem::is_directory(data_dir);
    } catch (const boost::filesystem::filesystem_error& e) {
        if (e.code() == boost::system::errc::permission_denied) {
            // ONLY FOR DEBUG
        }
        bRes = false;
    }
    return bRes;
}
//=============================================================================
PathFunc::PathFunc(const std::wstring& path, bool withWatcher)
{
    this->withWatcher = withWatcher;
    if (isdir(path)) {
        _path = path;
    } else {
        _path.clear();
    }
    rehash();
}
//=============================================================================
bool
PathFunc::comparePathname(const std::wstring& path1, const std::wstring& path2)
{
    boost::filesystem::path p1(path1);
    boost::filesystem::path p2(path2);
    bool res = false;
    try {
        boost::filesystem::equivalent(p1, p2);
    } catch (const boost::filesystem::filesystem_error&) {
        res = (p1 == p2);
    }
    return res;
}
//=============================================================================
PathFunc::~PathFunc()
{
    for (boost::unordered_map<std::wstring, FileFunction*>::iterator it = mapAllFiles.begin();
         it != mapAllFiles.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    mapRecentFiles.clear();
    mapAllFiles.clear();
    _path.clear();
}
//=============================================================================
wstringVector
PathFunc::getFunctionsName(const std::wstring& prefix)
{
    wstringVector functionsName;
    for (boost::unordered_map<std::wstring, FileFunction*>::iterator it = mapAllFiles.begin();
         it != mapAllFiles.end(); ++it) {
        if (it->second) {
            if (prefix.empty()) {
                functionsName.push_back(it->second->getName());
            } else {
                std::wstring name = it->second->getName();
                if (boost::starts_with(name, prefix)) {
                    functionsName.push_back(name);
                }
            }
        }
    }
    return functionsName;
}
//=============================================================================
wstringVector
PathFunc::getFunctionsFilename()
{
    wstringVector functionsFilename;
    for (boost::unordered_map<std::wstring, FileFunction*>::iterator it = mapAllFiles.begin();
         it != mapAllFiles.end(); ++it) {
        if (it->second) {
            functionsFilename.push_back(it->second->getFilename());
        }
    }
    return functionsFilename;
}
//=============================================================================
std::wstring
PathFunc::getPath()
{
    return _path;
}
//=============================================================================
bool
PathFunc::isSupportedFuncFilename(const std::wstring& name)
{
    for (int c : name) {
        bool bSupportedChar
            = (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c == '_') || (c >= 48 && c <= 57);
        if (!bSupportedChar) {
            return false;
        }
    }
    return true;
}
//=============================================================================
void
PathFunc::rehash()
{
    if (!_path.empty()) {
        mapRecentFiles.clear();
        try {
            boost::filesystem::directory_iterator end_iter;
            for (boost::filesystem::directory_iterator dir_iter(_path); dir_iter != end_iter;
                 ++dir_iter) {
                boost::filesystem::path current = dir_iter->path();
                std::wstring ext = current.extension().generic_wstring();
                bool isMacro = ext == L".m";
                bool isMex = ext == L"." + getMexExtension();
                if (isMacro || isMex) {
                    std::wstring name = current.stem().generic_wstring();
                    if (isSupportedFuncFilename(name)) {
                        FileFunction* ff = nullptr;
                        try {
                            ff = new FileFunction(_path, name, isMex, withWatcher);
                        } catch (const std::bad_alloc&) {
                            ff = nullptr;
                        }
                        if (ff) {
                            mapAllFiles.emplace(name, ff);
                        }
                    }
                }
            }
        } catch (const boost::filesystem::filesystem_error&) {
        }
    }
}
//=============================================================================
static bool
isfile(const std::wstring& filename)
{
    try {
        return boost::filesystem::exists(filename) && !boost::filesystem::is_directory(filename);

    } catch (const boost::filesystem::filesystem_error&) {
    }
    return false;
}
//=============================================================================
bool
PathFunc::findFuncName(const std::wstring& functionName, std::wstring& filename)
{
    boost::unordered_map<std::wstring, FileFunction*>::iterator found
        = mapAllFiles.find(functionName);
    if (found != mapAllFiles.end()) {
        filename = found->second->getFilename();
        if (isfile(filename)) {
            return true;
        }
    }
    if (withWatcher) {
        const std::wstring mexFullFilename = _path + L"/" + functionName + L"." + getMexExtension();
        if (isfile(mexFullFilename)) {
            filename = mexFullFilename;
            return true;
        }
        const std::wstring macroFullFilename = _path + L"/" + functionName + L".m";
        if (isfile(macroFullFilename)) {
            filename = macroFullFilename;
            return true;
        }
    }
    return false;
}
//=============================================================================
bool
PathFunc::findFuncName(const std::wstring& functionName, FileFunction** ff)
{
    boost::unordered_map<std::wstring, FileFunction*>::const_iterator foundit
        = mapRecentFiles.find(functionName);
    if (foundit != mapRecentFiles.end()) {
        *ff = foundit->second;
        if (isfile(foundit->second->getFilename())) {
            return true;
        } else {
            if (!withWatcher) {
                std::wstring msg
                    = str(boost::wformat(_W("Previously accessible file '%s' is now inaccessible."))
                        % foundit->second->getFilename());
                Error(msg);
            }
        }
    }
    boost::unordered_map<std::wstring, FileFunction*>::iterator found
        = mapAllFiles.find(functionName);
    if (found != mapAllFiles.end()) {
        *ff = found->second;
        if (isfile(found->second->getFilename())) {
            mapRecentFiles.emplace(functionName, *ff);
            return true;
        } else {
            if (!withWatcher) {
                std::wstring msg
                    = str(boost::wformat(_W("Previously accessible file '%s' is now inaccessible."))
                        % found->second->getFilename());
                Error(msg);
            }
        }
    }
    if (withWatcher) {
        const std::wstring mexFullFilename = _path + L"/" + functionName + L"." + getMexExtension();
        bool foundAsMacro = false;
        bool foundAsMex = false;
        foundAsMex = isfile(mexFullFilename);
        if (!foundAsMex) {
            const std::wstring macroFullFilename = _path + L"/" + functionName + L".m";
            foundAsMacro = isfile(macroFullFilename);
        }
        if (foundAsMex || foundAsMacro) {
            try {
                *ff = new FileFunction(_path, functionName, foundAsMex, withWatcher);
            } catch (const std::bad_alloc&) {
                *ff = nullptr;
            }
            if (ff) {
                mapAllFiles.emplace(functionName, *ff);
            }
            return true;
        }
    }
    return false;
}
//=============================================================================
} // namespace Nelson
//=============================================================================
