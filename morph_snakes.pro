TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ../bin

win32:INCLUDEPATH+=D:\Program\opencv2410\build\include\
D:\Program\opencv2410\build\include\opencv\
D:\Program\opencv2410\build\include\opencv2\

win32:Release:LIBS+=D:\Program\opencv2410\build\x86\vc10\lib\opencv_calib3d2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_contrib2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_core2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_features2d2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_flann2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_gpu2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_highgui2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_imgproc2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_legacy2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_ml2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_nonfree2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_objdetect2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_ocl2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_photo2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_stitching2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_superres2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_ts2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_video2410.lib\
D:\Program\opencv2410\build\x86\vc10\lib\opencv_videostab2410.lib

SOURCES += main.cpp

