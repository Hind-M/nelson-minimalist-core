# ==============================================================================
# Copyright (c) 2016-present Allan CORNET (Nelson)
# ==============================================================================
# This file is part of the Nelson.
# =============================================================================
# LICENCE_BLOCK_BEGIN
# SPDX-License-Identifier: LGPL-3.0-or-later
# LICENCE_BLOCK_END
# ==============================================================================
set(module_name os_functions)
set(module_library_name nlsOs_functions)
# ==============================================================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../types/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../api_nelson/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../error_manager/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../commons/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../stream_manager/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../characters_encoding/no-src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../nelson_manager/src/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../i18n/no-src/include)
# ==============================================================================
file(GLOB OS_FUNCTIONS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/*.cpp")
list(FILTER OS_FUNCTIONS_SRC EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/dllMain.cpp") 
# ==============================================================================
add_library(${module_library_name} SHARED ${OS_FUNCTIONS_SRC})
# ==============================================================================
target_link_libraries(
  ${module_library_name}
  nlsApi_nelson
  nlsCharacters_encoding
  nlsCommons
  nlsError_manager
  nlsI18n
  nlsNelson_manager
  nlsStream_manager
  nlsCommons)
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
