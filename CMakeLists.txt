cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("BankDepositSelectionSystem")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

if(EXISTS ${CMAKE_SOURCE_DIR}/data/deposits.db)
    file(COPY ${CMAKE_SOURCE_DIR}/data/deposits.db DESTINATION ${CMAKE_BINARY_DIR}/data)
else()
    file(WRITE ${CMAKE_BINARY_DIR}/data/deposits.db "")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Database
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Analysis
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Recommendation
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Report
)

set(SOURCES
    BankDepositSelectionSystem.cpp
    src/Client.cpp
    src/Deposit.cpp
    src/Bank.cpp
    src/Database/SQLiteDatabase.cpp
    src/Analysis/BasicDepositAnalyzer.cpp
    src/Recommendation/FlexibleTermStrategy.cpp
    src/Report/HTMLReportGenerator.cpp
    src/Report/TextReportGenerator.cpp
    src/AppController.cpp
    src/Factory.cpp
)

add_executable(BankDepositSelectionSystem ${SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BankDepositSelectionSystem PROPERTY CXX_STANDARD 20)
endif()

set(SQLITE3_ROOT "${CMAKE_SOURCE_DIR}/sqlite3")

include_directories("${SQLITE3_ROOT}/include")
link_directories("${SQLITE3_ROOT}/lib")

target_link_libraries(BankDepositSelectionSystem PRIVATE "${SQLITE3_ROOT}/lib/sqlite3.lib")

add_custom_command(TARGET BankDepositSelectionSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${SQLITE3_ROOT}/lib/sqlite3.dll"
        $<TARGET_FILE_DIR:BankDepositSelectionSystem>
)

if (MSVC)
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
endif()