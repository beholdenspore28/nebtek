# default platform is unix-like using X11
PLATFORM      ?= X11

# this is the relative directory where the object files and the final executable
# is created
SRC_DIR        = src
BUILD_DIR      = build

# select compiler by setting the CC variable below
# use clang plus custom flags or use CLANG for the recommended flags
GCC            = gcc -fanalyzer
CLANG          = clang -fsanitize=address,undefined
CC             = $(CLANG)

# select debug or release mode by appending CFLAGS_DEBUG or CFLAGS_RELEASE to
# CFLAGS variable
CFLAGS_DEBUG   = -g3
CFLAGS_RELEASE = -O3 -flto
CFLAGS         = -Wall            \
                 -Wextra          \
                 -Wpedantic       \
                 -std=c99         \
                  $(CFLAGS_DEBUG) \

# check if PLATFORM is X11 and if so, link to libraries for unix-like
# environment with X11
ifeq ($(PLATFORM), X11)

LIBS           = -lX11            \
                 -lrt             \

endif

# this part is fully platform independant. it just finds all the source files
# and adds them to SRC. for each source file, a .o file is made.
SRC            = $(wildcard $(SRC_DIR)/*.c)
OBJ            = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# include the working directory and any library's include directories.
INC            = -Isrc           \
                 -Iglad/include

# define library archive files, executables, and object files that will be needed later.
LIBRARY        = $(BUILD_DIR)/platform_x11.a
TEST           = $(BUILD_DIR)/platform_test
GLAD           = $(BUILD_DIR)/glad.o
GLX            = $(BUILD_DIR)/glx.o

# build targets ===============================================================

$(TEST): $(LIBRARY)
	$(CC) $(CFLAGS) $< test/main.c -o $(TEST) $(INC) -L. -l:$(LIBRARY) $(LIBS)
	./build/platform_test

$(LIBRARY): $(OBJ) $(GLAD) $(GLX)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(BUILD_DIR):
	mkdir -p build

$(GLAD): $(BUILD_DIR)
	$(CC) $(CFLAGS) -c glad/src/gl.c -o $(GLAD) -Iglad/include

$(GLX): $(BUILD_DIR)
	$(CC) $(CFLAGS) -c glad/src/glx.c -o $(GLX) -Iglad/include
