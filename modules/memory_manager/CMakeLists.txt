# ==============================================================================
# Copyright (c) 2016-present Allan CORNET (Nelson)
# ==============================================================================
# This file is part of the Nelson.
# =============================================================================
# LICENCE_BLOCK_BEGIN SPDX-License-Identifier: LGPL-3.0-or-later
# LICENCE_BLOCK_END
# ==============================================================================
set(module_name memory_manager)
set(module_library_name nlsMemory_manager)
# ==============================================================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../types/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../interpreter/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../stream_manager/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../commons/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../error_manager/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../characters_encoding/no-src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../overload/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../i18n/no-src/include)
# ==============================================================================
file(GLOB MEMORY_MANAGER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/*.cpp")
list(FILTER MEMORY_MANAGER_SRC EXCLUDE REGEX
     "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/dllMain.cpp")
# ==============================================================================
add_library(${module_library_name} SHARED ${MEMORY_MANAGER_SRC})
# ==============================================================================
target_link_libraries(${module_library_name} 
  nlsCharacters_encoding
  nlsInterpreter
  nlsError_manager
  nlsI18n
  nlsCommons
  nlsTypes)
# ==============================================================================
set_target_properties(
  ${module_library_name}
  PROPERTIES
    VERSION
    ${Nelson_VERSION_MAJOR}.${Nelson_VERSION_MINOR}.${Nelson_VERSION_MAINTENANCE}
    SOVERSION ${Nelson_VERSION_MAJOR})
# ==============================================================================
install(TARGETS ${module_library_name} LIBRARY DESTINATION ${LIBRARY_OUTPUT})
# ==============================================================================
