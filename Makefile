ifeq ($(OS),Windows_NT)
	$(error Windows is not supported)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		ifeq ($(CXX),g++)
			MKSPEC = linux-g++
		else
			MKSPEC = linux-clang
		endif
	endif
	ifeq ($(UNAME_S),Darwin)
		ifeq ($(CXX),g++)
			MKSPEC = macx-g++
		else
			MKSPEC = macx-clang
		endif
	endif
endif

ifeq ($(MKSPEC),"")
	 $(error MKSPEC environment variable is not defined)
endif

all: default

default: configure debug release

debug: configure _debug

release: configure _release

configure:
	if [ ! -d "./build" ]; then mkdir -p build;  fi

_debug:
	qmake -makefile -spec $(MKSPEC) -o ./build/debug/Makefile TajirStoreBuilder.pro  "CONFIG+=debug" 
	$(MAKE) -C ./build/debug

_release:
	qmake -makefile -spec $(MKSPEC) -o ./build/release/Makefile TajirStoreBuilder.pro  "CONFIG+=release"
	$(MAKE) -C ./build/release

clean:
	rm -rf build/*
	rm -f .qmake.stash