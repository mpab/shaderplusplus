#! make

##@ This is a location-independent Makefile

########################################################################
######################### Configuration ################################
########################################################################

# Environment
DISTRO_NAME := $(shell uname -s | sed 's/-.*//')
MAKEFILE_FILEPATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_PATH := $(shell dirname $(MAKEFILE_FILEPATH))
CURRENT_DIR := $(shell basename  $(PROJECT_PATH))
MAKE_SELF := $(MAKE) -f $(MAKEFILE_FILEPATH)
# $(info OS/DISTRO: $(DISTRO_NAME))
# $(info MAKEFILE_FILEPATH: $(MAKEFILE_FILEPATH))
# $(info PROJECT_PATH: $(PROJECT_PATH))
# $(info CURRENT_DIR: $(CURRENT_DIR))
# $(info MAKE_SELF: $(MAKE_SELF))

# Project
RAY-APPNAME := ray
SDL-APPNAME := sdl
CON-APPNAME := console
TEST-GLSL-APPNAME := test-glsl
TEST-TASKS-APPNAME := test-tasks

CC = clang++
SRCEXT := .c*
HDREXT := .h*
SRCDIR := $(PROJECT_PATH)/src
SRCSHADERDIR := $(PROJECT_PATH)/src/shaders
SRCLIBDIR := $(PROJECT_PATH)/lib/shfl
TESTSRCDIR := $(PROJECT_PATH)/tests
OBJDIR := $(PROJECT_PATH)/bin
TESTOBJDIR := $(PROJECT_PATH)/bin-test
ASSETSDIR := $(PROJECT_PATH)/assets

ALLSRC := $(wildcard $(SRCDIR)/*$(SRCEXT)) $(wildcard $(SRCSHADERDIR)/*$(SRCEXT)) $(wildcard $(SRCLIBDIR)/*$(SRCEXT)) 
ALLHDR := $(wildcard $(SRCDIR)/*$(HDREXT)) $(wildcard $(SRCSHADERDIR)/*$(HDREXT)) $(wildcard $(SRCLIBDIR)/*$(HDREXT)) 

$(info -SRC: $(ALLSRC))
$(info ALLSRCDIRS: $(ALLSRCDIRS))

RAY-APPSRC := $(SRCDIR)/$(RAY-APPNAME)$(SRCEXT)
RAY-EXE := $(OBJDIR)/$(RAY-APPNAME)

SDL-APPSRC := $(SRCDIR)/$(SDL-APPNAME)$(SRCEXT)
SDL-EXE := $(OBJDIR)/$(SDL-APPNAME)

CON-APPSRC := $(SRCDIR)/$(CON-APPNAME)$(SRCEXT)
CON-EXE := $(OBJDIR)/$(CON-APPNAME)

TEST-GLSL-APPSRC := $(TESTSRCDIR)/$(TEST-GLSL-APPNAME)$(SRCEXT)
TEST-GLSL-EXE := $(TESTOBJDIR)/$(TEST-GLSL-APPNAME)

TEST-TASKS-APPSRC := $(TESTSRCDIR)/$(TEST-TASKS-APPNAME)$(SRCEXT)
TEST-TASKS-EXE := $(TESTOBJDIR)/$(TEST-TASKS-APPNAME)

RM := rm -f
MD := mkdir -p

########################################################################
############################# Targets ##################################
########################################################################

##@ Targets

build: build-ray ## (default) redirects to build-ray

run: run-ray ## redirects to run-ray

test: run-glsl-test ## redirects to run-glsl-test

# IMPORTANT! src/object commands come first on Mingw64
# ensure "-o $@ $^" is at the start of the expression
build-ray: create-dirs configure $(RAY-EXE) ## build the raylib gui app
$(RAY-EXE): $(RAY-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(RAY-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS) $(RAY_FLAGS)

build-ray-dbg: create-dirs configure $(RAY-EXE)-dbg ## build the raylib gui app, no optimizations, debug symbols enabled
$(RAY-EXE)-dbg: $(RAY-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(RAY-APPSRC) $(DEBUG) $(CXXFLAGS) $(LDFLAGS) $(RAY_FLAGS)

run-ray: build-ray ## build, then run the raylib gui app
	$(RAY-EXE)

.PHONY: echo-build-ray
echo-build-ray: configure ## echo the build command line
	@echo $(CC) -o $@ $(RAY-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS) $(RAY_FLAGS)

# IMPORTANT! src/object commands come first on Mingw64
# ensure "-o $@ $^" is at the start of the expression
build-sdl: copy-assets configure $(SDL-EXE) ## build the sdl gui app
$(SDL-EXE): $(SDL-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(SDL-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS) $(SDL_FLAGS)

build-sdl-dbg: copy-assets configure $(SDL-EXE)-dbg ## build the sdl gui app, no optimizations, debug symbols enabled
$(SDL-EXE)-dbg: $(SDL-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(SDL-APPSRC) $(DEBUG) $(CXXFLAGS) $(LDFLAGS) $(SDL_FLAGS)

run-sdl: build-sdl ## build, then run the sdl gui app
	$(SDL-EXE)

.PHONY: echo-build-sdl
echo-build-sdl: configure ## echo the build command line
	@echo $(CC) -o $@ $(SDL-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS) $(SDL_FLAGS)

build-con: create-dirs configure $(CON-EXE) ## build the console app
$(CON-EXE): $(CON-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(CON-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS)

build-con-dbg: create-dirs configure $(CON-EXE)-dbg ## build the console app, no optimizations, debug symbols enabled
$(CON-EXE)-dbg: $(CON-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(CON-APPSRC) $(DEBUG) $(CXXFLAGS) $(LDFLAGS)

run-con: build-con ## build, then run the console app
	$(CON-EXE)

.PHONY: echo-build-con
echo-build-con: configure ## echo the build command line
	@echo $(CC) -o $@ $(CON-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS)

build-test-glsl: create-dirs configure $(TEST-GLSL-EXE) ## build the glsl test app
$(TEST-GLSL-EXE): $(TEST-GLSL-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(TEST-GLSL-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS)

run-test-glsl: build-test-glsl ## build, then run the glsl test app
	$(TEST-GLSL-EXE)

build-test-tasks: create-dirs configure $(TEST-TASKS-EXE) ## build the glsl test app
$(TEST-TASKS-EXE): $(TEST-TASKS-APPSRC) $(ALLSRC) $(ALLHDR)
	$(CC) -o $@ $(TEST-TASKS-APPSRC) $(RELEASE) $(CXXFLAGS) $(LDFLAGS)

run-test-tasks: build-test-tasks ## build, then run the glsl test app
	$(TEST-TASKS-EXE)

clean: ## clean the output folders
	$(RM) $(OBJDIR)/*
	$(RM) $(TESTOBJDIR)/*

.PHONY: create-dirs
create-dirs: ## create the output folders
	$(MD) $(OBJDIR)
	$(MD) $(TESTOBJDIR)

.PHONY: copy-assets
copy-assets: create-dirs
	cp -R $(ASSETSDIR)/* $(OBJDIR)/

all: create-dirs copy-assets build-sdl build-con build-ray ## build all apps in release mode

run-all-tests: run-test-glsl run-test-tasks ## build, then run all tests

.PHONY: configure
configure:
ifeq ($(DISTRO_NAME), Linux)
	@echo "Configuring for Linux"
CXXFLAGS := -std=c++20 -Wall -I"$(SRCDIR)" -I"$(SRCLIBDIR)" 
LDFLAGS := -lX11 -lGL -lpthread -lm -lstdc++ 
SDL_FLAGS := -I"$(SDL3_DIR)/include" -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf -L"$(SDL3_DIR)/lib"
RAY_FLAGS := -I./lib/raylib-5.5_linux_amd64/include/ -L./lib/raylib-5.5_linux_amd64/lib/ -l:libraylib.a
# endif

else ifeq ($(DISTRO_NAME),Darwin)
	@echo "Configuring for MacOS"
MACOSX_DEPLOYMENT_TARGET := "$(shell sw_vers -productVersion 2> /dev/null)"
BREW_PREFIX := "$(shell brew --prefix 2> /dev/null)"
CXXFLAGS := -std=c++20 -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET) -Wall \
-framework OpenGL -framework GLUT -framework Carbon \
-I"$(SRCDIR)" -I"$(SRCLIBDIR)" -I$(BREW_PREFIX)/include
LDFLAGS := -L"$(BREW_PREFIX)/lib"
SDL_FLAGS := -I"$(SDL3_DIR)/include" -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf -L"$(SDL3_DIR)/lib" -rpath "$(SDL3_DIR)/lib"
RAY_FLAGS := -I./lib/raylib-5.5_macos/include -L./lib/raylib-5.5_macos/lib -rpath ./lib/raylib-5.5_macos -lraylib
# endif

else ifeq ($(DISTRO_NAME),MINGW64_NT)
	@echo Configuring for Mingw64
CXXFLAGS = -lstdc++fs -std=c++20 -I"$(SRCDIR)" -I"$(SRCLIBDIR)"
LDFLAGS = -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lwinmm
SDL_FLAGS := -I"$(SDL3_DIR)/include" -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf -L"$(SDL3_DIR)/lib"
RAY_FLAGS := -I./lib/raylib-5.5_win64_mingw-w64/include/ -L./lib/raylib-5.5_win64_mingw-w64/lib/ -l:libraylib.a -lgdi32 -lwinmm
# endif

else
# if you see this error, you should add the missing configuration
	$(error configure - unknown/unsupported environment: $(DISTRO_NAME))  
endif

RELEASE := -O3
DEBUG := -ggdb

##@ Help commands
.PHONY: help
help: ## display this help
	@awk 'BEGIN {FS = ":.*##";                                            		\
		printf "Usage: make \033[36m<target>\033[0m\n"} /^[a-zA-Z_-]+:.*?##/ 	\
		{ printf "  \033[36m%-20s\033[0m %s\n", $$1, $$2 } /^##@/            	\
		{ printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } '                  	\
		$(MAKEFILE_LIST)
