QT += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug
CONFIG += console


DEFINES += QT_DEPRECATED_WARNINGS


# Dirs
DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui
RCC_DIR = rcc


TARGET = AirlineSystem

# Includes
INCLUDEPATH += include

# Source files
SOURCES += \
    src/main_qt.cpp \
    src/MainWindow.cpp \
    src/Person.cpp \
    src/Passenger.cpp \
    src/Admin.cpp \
    src/Flight.cpp \
    src/Ticket.cpp \
    src/FileManager.cpp \
    src/ReservationSystem.cpp \
    src/chatBot.cpp \
    src/ChatbotWidget.cpp

# Header files
HEADERS += \
    include/MainWindow.h \
    include/Person.h \
    include/Passenger.h \
    include/Admin.h \
    include/Flight.h \
    include/Ticket.h \
    include/FileManager.h \
    include/ReservationSystem.h \
    include/ChatBot.h \
    include/ChatbotWidget.h




# mac
macx {

        # C++ flags
    QMAKE_CXXFLAGS += -std=c++17 -stdlib=libc++
    
    # Disable SDK version check
    CONFIG += sdk_no_version_check
    
    # System libraries
    LIBS += -lcurl
    
    # Homebrew paths
    exists(/opt/homebrew/include) {
        INCLUDEPATH += /opt/homebrew/include
        LIBS += -L/opt/homebrew/lib
    } else:exists(/usr/local/include) {
        INCLUDEPATH += /usr/local/include
        LIBS += -L/usr/local/lib
    }
    
    # JsonCpp
    LIBS += -ljsoncpp
    
    # Onnx runtimes

    exists($$PWD/onnxruntime-osx-arm64-1.16.3) {
        ONNX_PATH = $$PWD/onnxruntime-osx-arm64-1.16.3
    } 

    else:exists($$PWD/onnxruntime-osx-x86_64-1.16.3) {
        ONNX_PATH = $$PWD/onnxruntime-osx-x86_64-1.16.3
    }
    else {
        error("ONNX Runtime not found")
    }
    
    INCLUDEPATH += $$ONNX_PATH/include
    LIBS += -L$$ONNX_PATH/lib -lonnxruntime
    QMAKE_LFLAGS += -Wl,-rpath,$$ONNX_PATH/lib

    QMAKE_POST_LINK += mkdir -p data
}


# Linux
unix:!macx {    
    # C++ flags
    QMAKE_CXXFLAGS += -std=c++17
    
    # System libraries
    LIBS += -lcurl -ljsoncpp
    


    INCLUDEPATH += /usr/include /usr/local/include
    LIBS += -L/usr/lib -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu
    

    # Onnx runtime
    exists($$PWD/onnxruntime-linux-x64-1.16.3) {
        ONNX_PATH = $$PWD/onnxruntime-linux-x64-1.16.3
    }
    else {
        error("ONNX Runtime not found")
    }
    
    INCLUDEPATH += $$ONNX_PATH/include
    LIBS += -L$$ONNX_PATH/lib -lonnxruntime
    QMAKE_LFLAGS += -Wl,-rpath,$$ONNX_PATH/lib
    
    # Install path
    target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target

    QMAKE_POST_LINK += mkdir -p data
}

# Windows
win32 {
    QMAKE_CXXFLAGS += /std:c++17
    
    # Windows defines
    DEFINES += _WIN32_WINNT=0x0601
    DEFINES += NOMINMAX 
    
    # vcpkg paths first
    exists(C:/vcpkg/installed/x64-windows) {
        VCPKG_PATH = C:/vcpkg/installed/x64-windows
        INCLUDEPATH += $$VCPKG_PATH/include
        LIBS += -L$$VCPKG_PATH/lib
    }
    
    # Libraries
    LIBS += -lcurl -ljsoncpp
    LIBS += -lws2_32 -lwldap32 -ladvapi32 -lcrypt32
    
    # Onnx runtime
    exists($$PWD/onnxruntime-win-x64-1.16.3) {
        ONNX_PATH = $$PWD/onnxruntime-win-x64-1.16.3
    }
    else {
        error("ONNX Runtime not found")
    }
    
    INCLUDEPATH += $$ONNX_PATH/include
    LIBS += -L$$ONNX_PATH/lib -lonnxruntime
    
    QMAKE_POST_LINK += mkdir data


    QMAKE_POST_LINK += $(COPY_FILE) $$shell_path($$ONNX_PATH/lib/onnxruntime.dll) $$shell_path($$DESTDIR) $$escape_expand(\n\t)
}






# Clean targets
QMAKE_CLEAN += -r $$DESTDIR $$OBJECTS_DIR $$MOC_DIR $$UI_DIR $$RCC_DIR

