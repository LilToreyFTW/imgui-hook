#pragma once

#define CHEESE_USE_DEFAULT_FONTS 1

#if CHEESE_USE_DEFAULT_FONTS
#define ICON_MIN_FA 0
#define ICON_MAX_FA 0
#else
extern const uint8_t Rubik[];
extern const unsigned char font_awesome_data[];
extern const unsigned int font_awesome_size;
#define ICON_MIN_FA 0
#define ICON_MAX_FA 0xFFFF
#endif
