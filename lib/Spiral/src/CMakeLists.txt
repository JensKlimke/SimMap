set(SOURCE_FILES
        odrSpiral.c
        )

# create target
add_library(odr_spiral STATIC
        ${SOURCE_FILES}
        )