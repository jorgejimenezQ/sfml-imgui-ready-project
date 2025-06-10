message(STATUS "Fetching SFML")

# No need to build audio and network modules
#set(SFML_BUILD_AUDIO OFF)
#set(SFML_BUILD_NETWORK OFF)

FetchContent_MakeAvailable(SFML)