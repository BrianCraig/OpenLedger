set(srcs "src/fontx.c"
         "src/pngle.c"
         "src/st7789.c")

set(include_dirs "include")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES esp32 freertos driver spiffs)
