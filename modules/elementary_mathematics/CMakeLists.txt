# ==============================================================================
# Copyright (c) 2016-present Allan CORNET (Nelson)
# ==============================================================================
# This file is part of the Nelson.
# =============================================================================
# LICENCE_BLOCK_BEGIN
# SPDX-License-Identifier: LGPL-3.0-or-later
# LICENCE_BLOCK_END
# ==============================================================================
set(module_name elementary_mathematics)
set(module_library_name nlsElementary_mathematics)
set(module_library_builtin_name ${module_library_name}_builtin)
# ==============================================================================
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../i18n/no-src/include)
# ==============================================================================
file(GLOB ELEMENTARY_MATHEMATICS_SRC
     "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/*.cpp")
list(FILTER ELEMENTARY_MATHEMATICS_SRC EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/dllMain.cpp") 
# ==============================================================================
add_library(${module_library_name} SHARED ${ELEMENTARY_MATHEMATICS_SRC})
# ==============================================================================
set_target_properties(
  ${module_library_name}
  PROPERTIES
    VERSION
    ${Nelson_VERSION_MAJOR}.${Nelson_VERSION_MINOR}.${Nelson_VERSION_MAINTENANCE}
    SOVERSION ${Nelson_VERSION_MAJOR})
# ==============================================================================
install(TARGETS ${module_library_name} LIBRARY DESTINATION ${LIBRARY_OUTPUT})
# install(TARGETS ${module_library_builtin_name} LIBRARY DESTINATION
# ${LIBRARY_OUTPUT})
# ==============================================================================
