# workspaceFolder = .

# # Detect OS
# ifeq ($(OS),Windows_NT) # Windows
#     CPPFLAGS = g++ --std=c++17 -fdiagnostics-color=always -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
#     CFLAGS = gcc -std=c11 -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
#     CLIBS = -L${workspaceFolder}/lib/windows
#     LDFLAGS = -lglfw3dll -lopengl32
#     all: copy_lib_w copy_res_w build
# else
#     UNAME_S := $(shell uname -s)
#     ifeq ($(UNAME_S), Darwin) # macOS
#         CPPFLAGS = clang++ -std=c++17 -fcolor-diagnostics -fansi-escape-codes -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
#         CFLAGS = clang -std=c11 -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
#         CLIBS = -L${workspaceFolder}/lib/macOS ${workspaceFolder}/bin/libglfw.3.dylib
#         LDFLAGS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated -Wl,-rpath,.
#         all: copy_lib_m copy_res_m build
#     else
#         $(error Unsupported OS: $(UNAME_S))
#     endif
# endif

# # Source and object files
# SRC_FILES = $(wildcard ${workspaceFolder}/src/*.cpp)
# OBJ_FILES = $(patsubst ${workspaceFolder}/src/%.cpp, ${workspaceFolder}/bin/%.o, $(SRC_FILES)) ${workspaceFolder}/bin/glad.o

# # Rule to compile .o files from .cpp files
# ${workspaceFolder}/bin/%.o: ${workspaceFolder}/src/%.cpp | $(workspaceFolder)/bin
# 	$(CPPFLAGS) -c $< -o $@

# # Rule to compile glad.o
# ${workspaceFolder}/bin/glad.o: ${workspaceFolder}/src/glad.c | $(workspaceFolder)/bin
# 	$(CFLAGS) -c $< -o $@

# build: $(OBJ_FILES) | $(workspaceFolder)/bin
# 	$(CPPFLAGS) $(CLIBS) $(OBJ_FILES) -o ${workspaceFolder}/bin/main $(LDFLAGS)

# # Copy resources (MacOS)
# copy_lib_m:
# 	@echo "Copying library for MacOS..."
# 	cp -rf ${workspaceFolder}/lib/macOS/libglfw.3.dylib ${workspaceFolder}/bin/libglfw.3.dylib

# copy_res_m:
# 	@echo "Copying resources for MacOS..."
# 	mkdir -p ${workspaceFolder}/bin/res && cp -rf ${workspaceFolder}/src/res/* ${workspaceFolder}/bin/res

# # Copy resources (Windows)
# copy_lib_w:
# 	@echo "Copying library for Windows..."
# 	copy /Y ${workspaceFolder}\lib\windows\glfw3.dll ${workspaceFolder}\bin\glfw3.dll

# copy_res_w:
# 	@echo "Copying resources for Windows..."
# 	xcopy /E /I /Y ${workspaceFolder}\src\res ${workspaceFolder}\bin\res

# # clean:
# #     @echo "Cleaning..."
# #     rm -f ${workspaceFolder}\bin\main ${workspaceFolder}\bin\*.o

# # Parallel build (add -jN option to run with N jobs)
# .PHONY: all copy_res_m copy_res_w clean

workspaceFolder = .

# Detect OS
ifeq ($(OS),Windows_NT) # Windows
    CPPFLAGS = g++ --std=c++17 -fdiagnostics-color=always -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
    CFLAGS = gcc -std=c11 -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
    CLIBS = -L${workspaceFolder}/lib/windows
    LDFLAGS = -lglfw3dll -lopengl32
    all: clean copy_lib_w copy_res_w build
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Darwin) # macOS
        CPPFLAGS = clang++ -std=c++17 -fcolor-diagnostics -fansi-escape-codes -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
        CFLAGS = clang -std=c11 -Wall -g -I${workspaceFolder}/include -I${workspaceFolder}/src
        CLIBS = -L${workspaceFolder}/lib/macOS ${workspaceFolder}/bin/libglfw.3.dylib
        LDFLAGS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated -Wl,-rpath,.
        all: clean copy_lib_m copy_res_m build
    else
        $(error Unsupported OS: $(UNAME_S))
    endif
endif

# Source and object files
SRC_FILES = $(wildcard ${workspaceFolder}/src/*.cpp)
OBJ_FILES = $(patsubst ${workspaceFolder}/src/%.cpp, ${workspaceFolder}/bin/%.o, $(SRC_FILES)) ${workspaceFolder}/bin/glad.o

# Rule to compile .o files from .cpp files
${workspaceFolder}/bin/%.o: ${workspaceFolder}/src/%.cpp | $(workspaceFolder)/bin
	$(CPPFLAGS) -c $< -o $@

# Rule to compile glad.o
${workspaceFolder}/bin/glad.o: ${workspaceFolder}/src/glad.c | $(workspaceFolder)/bin
	$(CFLAGS) -c $< -o $@

build: $(OBJ_FILES) | $(workspaceFolder)/bin
	$(CPPFLAGS) $(CLIBS) $(OBJ_FILES) -o ${workspaceFolder}/bin/main $(LDFLAGS)

# Copy resources (MacOS)
copy_lib_m:
	@echo "Copying library for MacOS..."
	cp -rf ${workspaceFolder}/lib/macOS/libglfw.3.dylib ${workspaceFolder}/bin/libglfw.3.dylib

copy_res_m:
	@echo "Copying resources for MacOS..."
	mkdir -p ${workspaceFolder}/bin/res && cp -rf ${workspaceFolder}/src/res/* ${workspaceFolder}/bin/res

# Copy resources (Windows)
copy_lib_w:
	@echo "Copying library for Windows..."
	copy /Y ${workspaceFolder}\lib\windows\glfw3.dll ${workspaceFolder}\bin\glfw3.dll

copy_res_w:
	@echo "Copying resources for Windows..."
	xcopy /E /I /Y ${workspaceFolder}\src\res ${workspaceFolder}\bin\res

# Clean old builds
clean:
clean:
	@echo "Cleaning..."
	if exist ${workspaceFolder}\bin\main del /Q ${workspaceFolder}\bin\main
	if exist ${workspaceFolder}\bin\*.o del /Q ${workspaceFolder}\bin\*.o
	if exist ${workspaceFolder}\bin\res rmdir /S /Q ${workspaceFolder}\bin\res
	if exist ${workspaceFolder}\bin\libglfw.3.dylib del /Q ${workspaceFolder}\bin\libglfw.3.dylib


# Parallel build (add -jN option to run with N jobs)
.PHONY: all copy_res_m copy_res_w clean