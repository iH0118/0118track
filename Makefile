CC := gcc
LD := ld
OBJCOPY := objcopy
CFLAGS := -Iinclude -Wall -Wextra -Wno-unused-parameter -c -g
LDFLAGS := -lm -lSDL3

CC_WIN := x86_64-w64-mingw32-gcc
LD_WIN := x86_64-w64-mingw32-ld
OBJCOPY_WIN := x86_64-w64-mingw32-objcopy
CFLAGS_WIN := -Ilib/x86_64-w64-mingw32/include
LDFLAGS_WIN := -Llib/x86_64-w64-mingw32/lib

EXE_NAME := 0118track
BUILD_DIR_LIN := build/x86_64-linux-gnu
BUILD_DIR_WIN := build/x86_64-w64-mingw32

OBJ_CACHE_LIN := build/cache/x86_64-linux-gnu
OBJ_CACHE_WIN := build/cache/x86_64-w64-mingw32
DEP_CACHE := build/cache/deps
SPV_CACHE := build/cache/shaders

SRCS := $(shell find src -name '*.c')
ASSETS := $(wildcard assets/*)
OBJS_SRC_LIN := $(patsubst src/%.c,$(OBJ_CACHE_LIN)/%.o,$(SRCS))
OBJS_SRC_WIN := $(patsubst src/%.c,$(OBJ_CACHE_WIN)/%.o,$(SRCS))
OBJS_ASSETS_LIN :=$(patsubst assets/%,$(OBJ_CACHE_LIN)/asset_%.o,$(ASSETS))
OBJS_ASSETS_WIN :=$(patsubst assets/%,$(OBJ_CACHE_WIN)/asset_%.o,$(ASSETS))
OBJS_LIN := $(OBJS_SRC_LIN) $(OBJS_ASSETS_LIN)
OBJS_WIN := $(OBJS_SRC_WIN) $(OBJS_ASSETS_WIN)
DEPS := $(patsubst src/%.c,$(DEP_CACHE)/%.d,$(SRCS))
SHADERS_SRC := $(shell find shaders -name '*.vert' -or -name '*.frag')
SHADERS_SPV := $(patsubst shaders/%,$(SPV_CACHE)/%.spv,$(SHADERS_SRC))

build_lin: $(BUILD_DIR_LIN)/$(EXE_NAME) $(SHADERS_SPV)

$(BUILD_DIR_LIN)/$(EXE_NAME): $(OBJS_LIN)
	@mkdir -p $(dir $@)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_CACHE_LIN)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_CACHE_LIN)/asset_%.o: assets/%
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf64-x86-64 -B i386 --rename-section .data=.rodata,alloc,load,readonly,data,contents $< $@

build_win: $(BUILD_DIR_WIN)/$(EXE_NAME).exe $(SHADERS_SPV)

$(BUILD_DIR_WIN)/$(EXE_NAME).exe: $(OBJS_WIN)
	@mkdir -p $(dir $@)
	$(CC_WIN) $^ $(LDFLAGS_WIN) $(LDFLAGS) -o $@

$(OBJ_CACHE_WIN)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC_WIN) $(CFLAGS_WIN) $(CFLAGS) $< -o $@

$(OBJ_CACHE_WIN)/asset_%.o: assets/%
	@mkdir -p $(dir $@)
	$(OBJCOPY_WIN) -I binary -O pe-x86-64 -B i386 --rename-section .data=.rodata,alloc,load,readonly,data,contents $< $@

make_deps: $(DEPS)

$(DEP_CACHE)/%.d: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -E -MM $< -MF $@

$(SPV_CACHE)/%.spv: shaders/%
	@mkdir -p $(dir $@)
	glslc -c $< -o $@

clean_all: clean_objs clean_deps

clean_objs: clean_objs_lin clean_objs_win

clean_objs_lin:
	rm -f $(OBJS_LIN)

clean_objs_win:
	rm -f $(OBJS_WIN)

clean_deps:
	rm -f $(DEPS)

.PHONY: build clean_all clean_objs clean_deps

-include $(DEPS)