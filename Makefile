# Makefile for MacOS and Linux

UNAME := $(shell uname)

CC=gcc
CFLAGS=-std=c99 -Wall
LDFLAGS=
SOURCES=tetris.c
EXECUTABLE=tetris


ifeq ($(UNAME), Darwin)
	LDFLAGS=-framework GLUT -framework OPENGL -framework FOUNDATION
endif

ifeq ($(UNAME), Linux)
	LDFLAGS=-lGL -lGLU -lglut
endif

all: 	$(SOURCES)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(CFLAGS) $(LDFLAGS)