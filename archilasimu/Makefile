#
# Makefile to use with GLFW+emscripten
# See https://emscripten.org/docs/getting_started/downloads.html
# for installation instructions.
#
# This Makefile assumes you have loaded emscripten's environment.
# (On Windows, you may need to execute emsdk_env.bat or encmdprompt.bat ahead)
#
# Running `make -f Makefile.emscripten` will produce three files:
#  - web/index.html
#  - web/index.js
#  - web/index.wasm
#
# All three are needed to run the demo.

CC = emcc
CXX = em++
WEB_DIR = web
EXE = $(WEB_DIR)/index.html
IMGUI_DIR = ..
SOURCES = main.cpp stringUtils.cpp imGuiApp.cpp ArchiLaSimuApp.cpp CircuitElements.cpp ArchiTheme.cpp ArchiCircuit.cpp CPU.cpp RAM.cpp Sequencer.cpp MicrocodeTable.cpp Assembleur.cpp APropos.cpp CodeUploadDownload.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
CPPFLAGS = 
LDFLAGS = -sEXPORTED_FUNCTIONS=[_main,_malloc,_free] -sEXPORTED_RUNTIME_METHODS=[ccall] -sALLOW_MEMORY_GROWTH=1 
EMS =

##---------------------------------------------------------------------
## EMSCRIPTEN OPTIONS
##---------------------------------------------------------------------

# ("EMS" options gets added to both CPPFLAGS and LDFLAGS, whereas some options are for linker only)
EMS += -s DISABLE_EXCEPTION_CATCHING=1
LDFLAGS += -s USE_GLFW=3 -s USE_WEBGL2=1 -s  WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1

# Uncomment next line to fix possible rendering bugs with Emscripten version older then 1.39.0 (https://github.com/ocornut/imgui/issues/2877)
#EMS += -s BINARYEN_TRAP_MODE=clamp
#EMS += -s SAFE_HEAP=1    ## Adds overhead

# Emscripten allows preloading a file or folder to be accessible at runtime.
# The Makefile for this example project suggests embedding the misc/fonts/ folder into our application, it will then be accessible as "/fonts"
# See documentation for more details: https://emscripten.org/docs/porting/files/packaging_files.html
# (Default value is 0. Set to 1 to enable file-system and include the misc/fonts/ folder as part of the build.)
USE_FILE_SYSTEM ?= 1
ifeq ($(USE_FILE_SYSTEM), 0)
LDFLAGS += -s NO_FILESYSTEM=1
CPPFLAGS += -DIMGUI_DISABLE_FILE_FUNCTIONS
endif
ifeq ($(USE_FILE_SYSTEM), 1)
LDFLAGS += --no-heap-copy --preload-file ../misc/fonts@/fonts --no-heap-copy  --preload-file  ../misc/imgs@/imgs
endif

##---------------------------------------------------------------------
## FINAL BUILD FLAGS
##---------------------------------------------------------------------

CPPFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
#CPPFLAGS += -g
CPPFLAGS += -Wall -Wformat -Os $(EMS)
LDFLAGS += --shell-file $(SHELL_TEMPLATE) -s FULL_ES2=1 -s USE_WEBGL2=1
LDFLAGS += $(EMS)
BUILD_ID := $(shell date +%s)
SHELL_TEMPLATE := ../libs/emscripten/shell_minimal.html


##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(EXE)

$(WEB_DIR):
	mkdir $@

serve: all
	python3 -m http.server -d $(WEB_DIR) --bind 127.0.0.1 8000

$(EXE): $(OBJS) $(WEB_DIR)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	# Injecte la valeur RÉELLE de BUILD_ID dans le HTML (version cache-busting)
	sed -i 's|<script async type="text/javascript" src="index.js">|<script async src="index.js?v=$(BUILD_ID)">|' $(WEB_DIR)/index.html

	# Injecte LITTÉRALEMENT $(BUILD_ID) dans le JavaScript, pour une substitution future
	sed -i 's|var PACKAGE_NAME = '\''web/index.data'\'';|var PACKAGE_NAME = '\''web/index.$(BUILD_ID).data'\'';|' $(WEB_DIR)/index.js
	sed -i 's|var REMOTE_PACKAGE_BASE = '\''index.data'\'';|var REMOTE_PACKAGE_BASE = '\''index.$(BUILD_ID).data'\'';|' $(WEB_DIR)/index.js
	sed -i 's|wasmBinaryFile = '\''index.wasm'\'';|wasmBinaryFile = '\''index.$(BUILD_ID).wasm'\'';|' $(WEB_DIR)/index.js

	mv $(WEB_DIR)/index.wasm $(WEB_DIR)/index.$(BUILD_ID).wasm
	mv $(WEB_DIR)/index.data $(WEB_DIR)/index.$(BUILD_ID).data


clean:
	rm -rf $(OBJS) $(WEB_DIR) $(SHELL_OUTPUT)
