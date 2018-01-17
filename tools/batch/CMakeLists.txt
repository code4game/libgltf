set(TOOLS_BATCH_PATH "${TOOLS_PATH}/batch")

set(TOOLS_BATCH_FILE_LIST
    ${TOOLS_BATCH_PATH}/update_parser_by_scheme.bat
    ${TOOLS_BATCH_PATH}/update_parser_by_scheme.sh
    ${ROOT_PATH}/.gitlab-ci.yml
    ${ROOT_PATH}/.appveyor.yml
    ${ROOT_PATH}/.travis.yml
    )

source_group("script" FILES ${TOOLS_BATCH_FILE_LIST})

add_custom_target(batch SOURCES ${TOOLS_BATCH_FILE_LIST})
set_property(TARGET batch PROPERTY FOLDER tools)
