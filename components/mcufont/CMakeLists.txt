set(srcs "src/mf_bwfont.c" "src/mf_encoding.c" "src/mf_font.c" "src/mf_justify.c" "src/mf_kerning.c" "src/mf_rlefont.c" "src/mf_scaledfont.c" "src/mf_wordwrap.c" )

set(include_dirs "include")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}")