TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -llapack -lblas

TARGET = gtest

QT += widgets
QT += core
QT += charts

# Remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# Add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3

# MPI Settings
QMAKE_CXX = mpicxx
QMAKE_CXX_RELEASE       = $$QMAKE_CXX
QMAKE_CXX_DEBUG         = $$QMAKE_CXX
QMAKE_LINK              = $$QMAKE_CXX
QMAKE_CC                = mpicc

QMAKE_CFLAGS           += $$system(mpicc  --showme:compile)
QMAKE_LFLAGS           += $$system(mpicxx --showme:link)
QMAKE_CXXFLAGS         += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
QMAKE_CXXFLAGS_RELEASE += $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK

# Load internal source files
SOURCES += main.cpp \
           test_basis.cpp

HEADERS += ../src/Basis/basis.h \
    test_basis.h
    gtest/gtest.h
