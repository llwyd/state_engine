idf_component_register(SRCS "esp32.c"
                        "../../src/state.c"
                    INCLUDE_DIRS "." "../../src")
add_definitions(-DTARGET_ESP32)
