TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# opencv_win begin
OPENCV_VER = 340
COMPILER = vs2015
PLATFORM = x86

OPENCV_DIR = E:/0-myWorkSpace/0-3rdparty/opencv$$OPENCV_VER
message($$OPENCV_DIR)
OPENCV_LIB_DIR = $$OPENCV_DIR/$$COMPILER/$$PLATFORM/lib
message($$OPENCV_LIB_DIR)


INCLUDEPATH += $$OPENCV_DIR/include
INCLUDEPATH += $$OPENCV_DIR/include/opencv
INCLUDEPATH += $$OPENCV_DIR/include/opencv2

message($$PWD)

CONFIG(debug, debug|release){
    message(debug mode)

    DESTDIR = $$PWD/bin-debug

#    LIBS += $$OPENCV_LIB_DIR/opencv_calib3d"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_core"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_dnn"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_features2d"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_flann"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_highgui"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_imgcodecs"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_imgproc"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_ml"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_objdetect"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_photo"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_shape"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_stitching"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_superres"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_ts"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_video"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_videoio"$$OPENCV_VER"d.lib
#    LIBS += $$OPENCV_LIB_DIR/opencv_videostab"$$OPENCV_VER"d.lib

}else{
    message(release mode)

    DESTDIR = $$PWD/bin

    LIBS += $$OPENCV_LIB_DIR/opencv_calib3d"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_core"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_dnn"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_features2d"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_flann"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_highgui"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_imgcodecs"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_imgproc"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_ml"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_objdetect"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_photo"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_shape"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_stitching"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_superres"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_ts"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_video"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_videoio"$$OPENCV_VER".lib
    LIBS += $$OPENCV_LIB_DIR/opencv_videostab"$$OPENCV_VER".lib

}

# opencv_win end

SOURCES += main.cpp \
    morph_snakes.cpp

HEADERS += \
    morph_snakes.h

