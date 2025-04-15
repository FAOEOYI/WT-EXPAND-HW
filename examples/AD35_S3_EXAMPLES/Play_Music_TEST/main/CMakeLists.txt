idf_component_register(SRCS "Play_Music_TEST.c"
                    INCLUDE_DIRS ".")


spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)
