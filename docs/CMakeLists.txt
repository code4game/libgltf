set(DOC_ROOT_FILE_LIST
    ${ROOT_PATH}/README.md
    ${ROOT_PATH}/CONTRIBUTING.md
    ${ROOT_PATH}/CODE_OF_CONDUCT.md
    ${ROOT_PATH}/CHANGELOG.md
    ${ROOT_PATH}/LICENSE
    )

set(DOC_HELP_FILE_LIST
    ${ROOT_PATH}/docs/conf.py
    ${ROOT_PATH}/docs/index.rst
    )

source_group("root" FILES ${DOC_ROOT_FILE_LIST})
source_group("help" FILES ${DOC_HELP_FILE_LIST})

add_custom_target(DOC SOURCES ${DOC_ROOT_FILE_LIST} ${DOC_HELP_FILE_LIST})
set_property(TARGET DOC PROPERTY FOLDER CMakePredefinedTargets)
