cmake_minimum_required(VERSION 3.28)
project(sandboxapp VERSION 0.0)

set(CMAKE_C_STANDARD 17)

execute_process (
    COMMAND bash -c "psp-config --pspsdk-path"
    OUTPUT_VARIABLE outVar
)

set(PSPSDK OUTPUT_VARIABLE)

set(INC_PATH Bullet/src/include)

set(SOURCE_PATH sandbox/src)

set(ASSETS_PATH sandbox/assets)

set(ASSETS_END_PATH assets/)

set(TEXTURE_END_PATH assets/textures/)

set(AUDIO_END_PATH assets/mp3/)

configure_file(cmake_config.h.in ../cmake_config.h)

include_directories(${INC_PATH})
include_directories(${PSPSDK}/lib/build.mak)
include_directories(./)

file(COPY "${ASSETS_PATH}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(${PROJECT_NAME}
${INC_PATH}/application.c
${INC_PATH}/callbacks.c 
${INC_PATH}/graphics.c 
${INC_PATH}/logger.c
${INC_PATH}/input.c
${INC_PATH}/file.c
${INC_PATH}/savefile.c
#${INC_PATH}/audio.c

${SOURCE_PATH}/sandboxapp.c
${SOURCE_PATH}/cube.c
${SOURCE_PATH}/camera.c
${SOURCE_PATH}/rainbow_plane.c
${SOURCE_PATH}/UI.c
)

if(PLATFORM_PSP)
    
    create_pbp_file(
        TARGET ${PROJECT_NAME}

        #Menu Icon 144x82 .png
        ICON_PATH ../MenuItem.png
        #Background 480x272 .png (Appears behind everything)
        BACKGROUND_PATH ../HighIcon.png
        #Menu Image 480x272 .png (Smaller to the right)
        PREVIEW_PATH ../Background.png

        #Music .at3
        MUSIC_PATH ../SND0.AT3

        #Name
        TITLE Sandbox App

        BUILD_PRX
    )
    target_link_libraries(${PROJECT_NAME}
        pspgu
        pspgum
        pspdebug
        pspdisplay
        pspge
        pspctrl
        pspfpu
        pspaudio
        pspmp3
    )
endif()
