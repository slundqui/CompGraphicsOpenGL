ADD_EXECUTABLE(tutorial MyWindow.cpp main.cpp TargaImage.cpp libtarga.c)

TARGET_LINK_LIBRARIES(tutorial ${FLTK_LIBRARIES})
TARGET_LINK_LIBRARIES(tutorial ${OPENGL_LIBRARIES})
