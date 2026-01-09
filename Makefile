# ===== Compiler =====
CC      := clang
CSTD    := -std=c17

# ===== Directories =====
SRC_DIR := src
INC_DIR := include
EXE     := engine

# ===== Sources =====
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:.c=.o)

# ===== Common flags =====
CFLAGS_COMMON := $(CSTD) -I$(INC_DIR) -Wall -Wextra -Wshadow -Wconversion

# ===== Release flags =====
CFLAGS_RELEASE := -O3 -march=native -flto -DNDEBUG

# ===== Debug flags =====
CFLAGS_DEBUG := -O0 -g -DDEBUG

# ===== Profile flags =====
CFLAGS_PROFILE := -O3 -march=native -g -fno-omit-frame-pointer -DNDEBUG

# ===== Link flags =====
LDFLAGS := -flto -fuse-ld=lld

# ===== Default =====
all: release

# ===== Release build =====
release: CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_RELEASE)
release: $(EXE)

# ===== Debug build =====
debug: CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_DEBUG)
debug: LDFLAGS :=
debug: $(EXE)

# ===== Profile build =====
profile: CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_PROFILE)
profile: LDFLAGS := $(LDFLAGS)
profile: $(EXE)

# ===== Build rules =====
$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ===== Clean =====
clean:
	rm -f $(OBJS) $(EXE)

.PHONY: all release debug clean
