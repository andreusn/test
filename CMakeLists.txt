cmake_minimum_required(VERSION 3.5)
project(ServerAdmin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# �������������� ��������� Qt-����������� ������
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ����� Qt
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql Network)

# ��������� ����������
set(SOURCES
    src/main.cpp
    src/serveradmin.cpp
    src/servergui.cpp
    src/databasehandler.cpp
)

set(HEADERS
    include/serveradmin.h
    include/servergui.h
    include/databasehandler.h
)

# �������� ������������ �����
add_executable(ServerAdmin ${SOURCES} ${HEADERS})

# ����������� Qt �������
target_link_libraries(ServerAdmin PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Sql
    Qt5::Network
)