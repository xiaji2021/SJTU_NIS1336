QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

LIBS += -L/libs -l bcrypt
INCLUDEPATH += C:\Users\31161\Desktop\qt project\myapp\include\bcrypt

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addtask_widget.cpp \
    adduser_dialog.cpp \
    changepassword_dialog.cpp \
    deluser_dialog.cpp \
    give_admin_dialog.cpp \
    login_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    mythread.cpp \
    showuser_dialog.cpp \
    task_manager.cpp \
    Users.cpp


HEADERS += \
    addtask_widget.h \
    adduser_dialog.h \
    changepassword_dialog.h \
    deluser_dialog.h \
    give_admin_dialog.h \
    login_widget.h \
    mainwindow.h  \
    mythread.h \
    showuser_dialog.h \
    task_manager.h \
    Users.h


FORMS += \
    addtask_widget.ui \
    adduser_dialog.ui \
    changepassword_dialog.ui \
    deluser_dialog.ui \
    give_admin_dialog.ui \
    login_widget.ui \
    mainwindow.ui \
    showuser_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
