cmake_minimum_required(VERSION 3.16)

project(reflexaCameraApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appreflexaCameraApp
    main.cpp
    app.rc
)

qt_add_qml_module(appreflexaCameraApp
    URI reflexaCameraApp
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        SOURCES controllers/WindowController.h controllers/WindowController.cpp
        SOURCES utilities/CameraImageProvider.h utilities/CameraImageProvider.cpp
        SOURCES controllers/CameraService.h controllers/CameraService.cpp

    RESOURCES
        assets/logo.png
        assets/logo.ico
        assets/face.png
        assets/grid.png
        assets/hdr.png
        assets/camera.png
        assets/video.png
        assets/timer.png
        assets/settings.png
        assets/qr.png
        assets/stop.png
        assets/dropdown.png
        assets/recording.gif
        assets/play.png
        assets/pause.png
        assets/arrow_left.png
        assets/arrow_right.png
        assets/video_thumb.png

        assets/delete.png
        assets/zoom_out.png
        assets/zoom_in.png
        assets/rotate.png


        assets/filter_images/1.jpg
        assets/filter_images/2.jpg
        assets/filter_images/3.jpg
        assets/filter_images/4.jpg
        assets/filter_images/5.jpg
        assets/filter_images/6.jpg
        assets/filter_images/7.jpg
        QML_FILES ui/TitleBar.qml
        QML_FILES ui/ControlButton.qml
        QML_FILES ui/ControlsView.qml
        QML_FILES ui/FiltersView.qml
        QML_FILES ui/SettingsView.qml
        QML_FILES ui/CameraControlButton.qml
        QML_FILES ui/CameraBehaviorButton.qml
        SOURCES utilities/CameraControls.h utilities/CameraControls.cpp
        QML_FILES ui/CustomComboBox.qml
        SOURCES controllers/ImageTextureController.h controllers/ImageTextureController.cpp
        SOURCES
        QML_FILES ui/CustomSlider.qml
        SOURCES controllers/ImageFiltersController.h controllers/ImageFiltersController.cpp
        SOURCES utilities/CapturedImageProvider.h utilities/CapturedImageProvider.cpp
        QML_FILES ui/RecordTimer.qml
        SOURCES utilities/FaceDetector.h utilities/FaceDetector.cpp
        data/res10_300x300_ssd_iter_140000.caffemodel
        data/deploy.prototxt
        QML_FILES ui/GridLines.qml
        QML_FILES
        QML_FILES ui/GalleryView.qml
        SOURCES utilities/QRDetector.h utilities/QRDetector.cpp
        SOURCES utilities/ClipboardHelper.h utilities/ClipboardHelper.cpp
        SOURCES models/MediaListModel.h models/MediaListModel.cpp

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appreflexaCameraApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appreflexaCameraApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appreflexaCameraApp
    PRIVATE Qt6::Quick
    ${OpenCV_LIBS}
)

target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Multimedia)
target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Core)
target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Widgets)
target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Qml)
target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Core)
target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Core)
target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Core)
target_link_libraries(appreflexaCameraApp PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appreflexaCameraApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(appreflexaCameraApp PROPERTIES
    WIN32_EXECUTABLE TRUE
)
