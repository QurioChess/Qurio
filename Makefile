# ===== Compiler =====
CC      := clang
CSTD    := -std=c17

# ===== Directories =====
SRC_DIR := src
INC_DIR := include
BIN     := engine

# ===== Sources =====
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:.c=.o)

# ===== Common flags =====
CFLAGS_COMMON := $(CSTD) -I$(INC_DIR) -Wall -Wextra -Wshadow -Wconversion

# ===== Release flags =====
CFLAGS_RELEASE := -O3 -march=native -flto -DNDEBUG

# ===== Debug flags =====
CFLAGS_DEBUG := -O0 -g

# ===== Link flags =====
LDFLAGS := -flto -fuse-ld=lld

# ===== Default =====
all: release

# ===== Release build =====
release: CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_RELEASE)
release: $(BIN)

# ===== Debug build =====
debug: CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_DEBUG)
debug: LDFLAGS :=
debug: $(BIN)

# ===== Build rules =====
$(BIN): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ===== Clean =====
clean:
	rm -f $(OBJS) $(BIN)

.PHONY: all release debug clean
