ADD_EXECUTABLE(project2 CubicBspline.cpp GenericException.cpp Ground.cpp Track.cpp Building.cpp Mountain.cpp World.cpp WorldWindow.cpp libtarga.c)

TARGET_LINK_LIBRARIES(project2 ${FLTK_LIBRARIES})
TARGET_LINK_LIBRARIES(project2 ${OPENGL_LIBRARIES})
