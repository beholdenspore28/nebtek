PLATFORM          ?= X11

SRC_DIR            = src
BUILD_DIR          = build

GCC                = gcc -fanalyzer
MINGW_GCC_ANALYZER = x86_64-w64-mingw32-gcc -fanalyzer
MINGW_GCC          = x86_64-w64-mingw32-gcc
CLANG              = clang -fsanitize=address,undefined

CFLAGS_DEBUG       = -g3
CFLAGS_RELEASE     = -O3 -flto
CFLAGS             = -Wall            \
                     -Wextra          \
                     -Wpedantic       \
                     -std=c99         \
                      $(CFLAGS_DEBUG) \

SRC                = $(wildcard $(SRC_DIR)/*.c)
OBJ                = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

INC                = -Isrc           \
                     -Iglad/include

LIBRARY            = $(BUILD_DIR)/platform.a
TEST               = $(BUILD_DIR)/platform_test

GLAD               = $(BUILD_DIR)/glad.o
GLX                = $(BUILD_DIR)/glx.o
WGL                = $(BUILD_DIR)/wgl.o

ifeq ($(PLATFORM), X11)

CC                 = $(CLANG)
LIBS               = -lX11            \
                     -lrt             \

all: $(TEST)
	$(TEST)

$(LIBRARY): $(OBJ) $(GLAD) $(GLX)
	ar rcs $@ $^

endif

ifeq ($(PLATFORM), WIN32)

CC                 = $(MINGW_GCC)
LIBS               = -lgdi32          \
                     -lopengl32       \

all: $(TEST)
	wine $(TEST).exe

$(LIBRARY): $(OBJ) $(GLAD) $(WGL)
	ar rcs $@ $^

endif

$(TEST): $(LIBRARY)
	$(CC) $(CFLAGS) $< test/main.c -o $(TEST) $(INC) -L. -l:$(LIBRARY) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(BUILD_DIR):
	mkdir -p build

$(GLAD): $(BUILD_DIR)
	$(CC) $(CFLAGS) -c glad/src/gl.c -o $(GLAD) -Iglad/include

$(WGL): $(BUILD_DIR)
	$(CC) $(CFLAGS) -c glad/src/wgl.c -o $(WGL) -Iglad/include

$(GLX): $(BUILD_DIR)
	$(CC) $(CFLAGS) -c glad/src/glx.c -o $(GLX) -Iglad/include
