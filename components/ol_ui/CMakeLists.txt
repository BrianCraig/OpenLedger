set(srcs "src/ol_ui.cpp" "src/ol_ui_example.cpp" "src/ol_text.cpp" src/_ol_layout.cpp)

set(include_dirs "include")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       PRIV_INCLUDE_DIRS private_include
                       REQUIRES ttgo_display system_status mcufont)
