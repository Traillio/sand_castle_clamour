BUILD_DIR=build
include $(N64_INST)/include/n64.mk
N64_CXXFLAGS = -std=c++17 -fpermissive
src = $(wildcard *.cpp) $(wildcard */*.cpp)

assets_png = $(wildcard assets/*.png)
assets_gltf = $(wildcard assets/*.gltf)
assets_ttf = $(wildcard assets/*.ttf)
assets_txt = $(wildcard assets/*.txt)
assets_wav = $(wildcard assets/*.wav)

assets_conv = 	$(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite))) \
				$(addprefix filesystem/,$(notdir $(assets_gltf:%.gltf=%.model64))) \
				$(addprefix filesystem/,$(notdir $(assets_ttf:%.ttf=%.font64))) \
				$(addprefix filesystem/,$(notdir $(assets_txt:%.txt=%.lvl))) \
				$(addprefix filesystem/,$(notdir $(assets_wav:%.wav=%.wav64)))

MKSPRITE_FLAGS ?= --optimize --compress
MKMODEL_FLAGS ?=
MKFONT_FLAGS ?= --size 18
MKASSET_FLAGS ?= --compress 1
all: beach.z64

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) -f CI4 --compress -o "$(dir $@)" "$<"

filesystem/%.model64: assets/%.gltf
	@mkdir -p $(dir $@)
	@echo "    [MODEL] $@"
	@$(N64_MKMODEL) $(MKMODEL_FLAGS) -o filesystem "$<"

filesystem/%.font64: assets/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem "$<"

filesystem/%.lvl: assets/%.txt
	@mkdir -p $(dir $@)
	@echo "    [LVL] $@"
	@$(N64_MKASSET) $(MKASSET_FLAGS) -o "$(dir $@)" "$<"

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) -o filesystem $<

$(BUILD_DIR)/beach.dfs: $(assets_conv)
$(BUILD_DIR)/beach.elf: $(src:%.cpp=$(BUILD_DIR)/%.o)

beach.z64: N64_ROM_TITLE="The beach game"
beach.z64: $(BUILD_DIR)/beach.dfs

clean:
	rm -rf $(BUILD_DIR) filesystem/ beach.z64 &

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean

