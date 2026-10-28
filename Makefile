CC := gcc
LD := ld
OBJCOPY := objcopy
CFLAGS := -Iinclude -Wall -Wextra -Wno-unused-parameter -c -g
LDFLAGS := -lm -lSDL3 -lSDL3_ttf

CC_WIN := x86_64-w64-mingw32-gcc
LD_WIN := x86_64-w64-mingw32-ld
OBJCOPY_WIN := x86_64-w64-mingw32-objcopy
CFLAGS_WIN := -Ilib/x86_64-w64-mingw32/include $(CFLAGS)
LDFLAGS_WIN := -Llib/x86_64-w64-mingw32/bin $(LDFLAGS)

EXE_NAME := 0118track
BUILD_DIR_LIN := build/x86_64-linux-gnu
BUILD_DIR_WIN := build/x86_64-w64-mingw32

OBJ_CACHE_LIN := build/cache/x86_64-linux-gnu
OBJ_CACHE_WIN := build/cache/x86_64-w64-mingw32
DEP_CACHE := build/cache/deps
SPV_CACHE := build/cache/shaders

SRCS := $(shell find src -name '*.c')
ASSETS := $(wildcard assets/*)
DEPS := $(patsubst src/%.c,$(DEP_CACHE)/%.d,$(SRCS))
SHADERS_SRC := $(shell find shaders -name '*.vert' -or -name '*.frag')
SHADERS_SPV := $(patsubst shaders/%,$(SPV_CACHE)/%.spv,$(SHADERS_SRC))
ASSET_HEADERS := $(patsubst assets/%,include/assets/%.h,$(ASSETS))
SHADER_HEADERS := $(patsubst $(SPV_CACHE)/%.spv,include/shaders/%.h,$(SHADERS_SPV))

OBJS_SRC_LIN := $(patsubst src/%.c,$(OBJ_CACHE_LIN)/%.o,$(SRCS))
OBJS_SRC_WIN := $(patsubst src/%.c,$(OBJ_CACHE_WIN)/%.o,$(SRCS))
OBJS_ASSETS_LIN := $(patsubst assets/%,$(OBJ_CACHE_LIN)/asset_%.o,$(ASSETS))
OBJS_ASSETS_WIN := $(patsubst assets/%,$(OBJ_CACHE_WIN)/asset_%.o,$(ASSETS))
OBJS_SHADER_LIN := $(patsubst $(SPV_CACHE)/%.spv,$(OBJ_CACHE_LIN)/shader_%.o,$(SHADERS_SPV))
OBJS_SHADER_WIN := $(patsubst $(SPV_CACHE)/%.spv,$(OBJ_CACHE_WIN)/shader_%.o,$(SHADERS_SPV))
OBJS_LIN := $(OBJS_SRC_LIN) $(OBJS_ASSETS_LIN) $(OBJS_SHADER_LIN)
OBJS_WIN := $(OBJS_SRC_WIN) $(OBJS_ASSETS_WIN) $(OBJS_SHADER_WIN)

build_lin: $(BUILD_DIR_LIN)/$(EXE_NAME)

$(BUILD_DIR_LIN)/$(EXE_NAME): $(OBJS_LIN)
	@mkdir -p $(dir $@)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_CACHE_LIN)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_CACHE_LIN)/asset_%.o: assets/%
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf64-x86-64 -B i386 --binary-symbol-prefix=_asset_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g') --rename-section .data=.rodata,alloc,load,readonly,data,contents $< $@

$(OBJ_CACHE_LIN)/shader_%.o: $(SPV_CACHE)/%.spv
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf64-x86-64 -B i386 --binary-symbol-prefix=_shader_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g') --rename-section .data=.rodata,alloc,load,readonly,data,contents $< $@

build_win: $(BUILD_DIR_WIN)/$(EXE_NAME).exe
	cp lib/x86_64-w64-mingw32/bin/* $(BUILD_DIR_WIN)

$(BUILD_DIR_WIN)/$(EXE_NAME).exe: $(OBJS_WIN)
	@mkdir -p $(dir $@)
	$(CC_WIN) $^ $(LDFLAGS_WIN) -o $@

$(OBJ_CACHE_WIN)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC_WIN) $(CFLAGS_WIN) $< -o $@

$(OBJ_CACHE_WIN)/asset_%.o: assets/%
	@mkdir -p $(dir $@)
	$(OBJCOPY_WIN) -I binary -O pe-x86-64 -B i386 --binary-symbol-prefix=_asset_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g') --rename-section .data=.rodata,alloc,load,readonly,data,contents $< $@

$(OBJ_CACHE_WIN)/shader_%.o: $(SPV_CACHE)/%.spv
	@mkdir -p $(dir $@)
	$(OBJCOPY_WIN) -I binary -O pe-x86-64 -B i386 --binary-symbol-prefix=_shader_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g') --rename-section .data=.rodata,alloc,load,readonly,data,contents $< $@

make_deps: $(DEPS)

$(DEP_CACHE)/%.d: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -E -MM $< -MF $@

asset_headers: $(ASSET_HEADERS) $(SHADER_HEADERS)

include/assets/%.h: assets/%
	@mkdir -p $(dir $@)
	@echo "/* automatically generated file -- do not edit */" > $@
	@echo "#ifndef _0118TRACK_$(shell echo $< | tr [:lower:] [:upper:] | sed 's/[[:punct:]]/_/g')_H" >> $@
	@echo "#define _0118TRACK_$(shell echo $< | tr [:lower:] [:upper:] | sed 's/[[:punct:]]/_/g')_H" >> $@
	@echo "    extern const unsigned char _asset_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g')_start[];" >> $@
	@echo "    extern const unsigned char _asset_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g')_end[];" >> $@
	@echo "#endif" >> $@

include/shaders/%.h: $(SPV_CACHE)/%.spv
	@mkdir -p $(dir $@)
	@echo "/* automatically generated file -- do not edit */" > $@
	@echo "#ifndef _0118TRACK_$(shell echo $< | tr [:lower:] [:upper:] | sed 's/[[:punct:]]/_/g')_H" >> $@
	@echo "#define _0118TRACK_$(shell echo $< | tr [:lower:] [:upper:] | sed 's/[[:punct:]]/_/g')_H" >> $@
	@echo "    extern const unsigned char _shader_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g')_start[];" >> $@
	@echo "    extern const unsigned char _shader_$(shell echo $(notdir $<) | sed 's/[[:punct:]]/_/g')_end[];" >> $@
	@echo "#endif" >> $@

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

.PHONY: build_lin build_win clean_all clean_objs clean_objs_lin clean_objs_win make_deps clean_deps asset_headers

-include $(DEPS)