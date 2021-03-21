set(srcs "src/ol_ui.cpp")

set(include_dirs "include")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES ttgo_display)
