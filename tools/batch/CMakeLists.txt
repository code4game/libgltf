set(TOOLS_BATCH_PATH "${TOOLS_PATH}/batch")

set(TOOLS_BATCH_FILE_LIST_SCRIPTS
    ${TOOLS_BATCH_PATH}/update_parser_by_scheme.bat
    ${TOOLS_BATCH_PATH}/update_parser_by_scheme.sh
    ${ROOT_PATH}/.gitlab-ci.yml
    ${ROOT_PATH}/.appveyor.yml
    ${ROOT_PATH}/.travis.yml
    )

set(TOOLS_BATCH_FILE_LIST_CODES
    ${TOOLS_BATCH_PATH}/codes/extension.schema.json.h
    ${TOOLS_BATCH_PATH}/codes/extension.schema.json.variable.cpp
    ${TOOLS_BATCH_PATH}/codes/extension.schema.json.function.cpp
    ${TOOLS_BATCH_PATH}/codes/extension.schema.json.parser.to.cpp
    ${TOOLS_BATCH_PATH}/codes/extension.schema.json.parser.from.cpp
    ${TOOLS_BATCH_PATH}/codes/libgltf.header.include.h
    ${TOOLS_BATCH_PATH}/codes/libgltf.header.namespace.begin.h
    ${TOOLS_BATCH_PATH}/codes/libgltf.header.namespace.end.h
    ${TOOLS_BATCH_PATH}/codes/libgltf.license.h
    )

source_group("scripts" FILES ${TOOLS_BATCH_FILE_LIST_SCRIPTS})
source_group("codes" FILES ${TOOLS_BATCH_FILE_LIST_CODES})

add_custom_target(batch SOURCES ${TOOLS_BATCH_FILE_LIST_SCRIPTS} ${TOOLS_BATCH_FILE_LIST_CODES})
set_property(TARGET batch PROPERTY FOLDER tools)
