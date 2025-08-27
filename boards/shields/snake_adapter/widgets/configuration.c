#include <stdlib.h>
#include "helpers/display.h"

#define HEX_PARSE_ERROR ((uint32_t)-1)

uint32_t hex_string_to_uint(const char *hex_str) {
    if (!hex_str) {
        return HEX_PARSE_ERROR;
    }

    uint32_t result = 0;
    uint8_t i = 0;

    // Optional "0x" or "0X" prefix
    if (hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X')) {
        i = 2;
    }

    if (hex_str[i + 6] != '\0') {
        // Not rgb hex
        return HEX_PARSE_ERROR;
    }

    if (hex_str[i] == '\0') {
        // Empty string after "0x"
        return HEX_PARSE_ERROR;
    }

    for (; hex_str[i] != '\0'; ++i) {
        char c = hex_str[i];
        uint32_t digit;

        if (isdigit(c)) {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            digit = 10 + (c - 'A');
        } else {
            // Invalid character for hex
            return HEX_PARSE_ERROR;
        }

        result = (result << 4) | digit;  // Multiply result by 16 and add digit
    }

    return result;
}

void board_size() {
    const char *size_str = CONFIG_SNAKE_BOARD_SIZE;
    if (strcmp(size_str, "XXXL") == 0) {
        set_snake_board_width(48);
        set_snake_board_height(48);
        set_snake_pixel_size(5);
    } else if (strcmp(size_str, "XXL") == 0) {
        set_snake_board_width(24);
        set_snake_board_height(24);
        set_snake_pixel_size(10);
    } else  if (strcmp(size_str, "XL") == 0) {
        set_snake_board_width(20);
        set_snake_board_height(20);
        set_snake_pixel_size(12);
    } else if (strcmp(size_str, "L") == 0) {
        set_snake_board_width(16);
        set_snake_board_height(16);
        set_snake_pixel_size(15);
    } else if (strcmp(size_str, "M") == 0) {
        set_snake_board_width(12);
        set_snake_board_height(12);
        set_snake_pixel_size(20);
    } else if (strcmp(size_str, "S") == 0) {
        set_snake_board_width(10);
        set_snake_board_height(10);
        set_snake_pixel_size(24);
    } else if (strcmp(size_str, "XS") == 0) {
        set_snake_board_width(8);
        set_snake_board_height(8);
        set_snake_pixel_size(30);
    } else {
        set_snake_board_width(24);
        set_snake_board_height(24);
        set_snake_pixel_size(10);
    }
}

void default_screen() {
    const char *default_screen_str = CONFIG_DEFAULT_SCREEN;
    if (strcmp(default_screen_str, "snake") == 0) {
        set_default_screen(SNAKE_SCREEN);
    } else if (strcmp(default_screen_str, "status") == 0) {
        set_default_screen(STATUS_SCREEN);
    } else {
        set_default_screen(SNAKE_SCREEN);
    }
}

void custom_theme() {
    uint32_t color1 = hex_string_to_uint(CONFIG_THEME_COLOR_1);
    uint32_t color2 = hex_string_to_uint(CONFIG_THEME_COLOR_2);
    uint32_t color3 = hex_string_to_uint(CONFIG_THEME_COLOR_3);
    uint32_t color4 = hex_string_to_uint(CONFIG_THEME_COLOR_4);
    uint32_t color5 = hex_string_to_uint(CONFIG_THEME_COLOR_5);
    uint32_t color6 = hex_string_to_uint(CONFIG_THEME_COLOR_6);
    if (color1 == HEX_PARSE_ERROR ||
        color2 == HEX_PARSE_ERROR ||
        color3 == HEX_PARSE_ERROR ||
        color4 == HEX_PARSE_ERROR ||
        color5 == HEX_PARSE_ERROR ||
        color6 == HEX_PARSE_ERROR) {
        set_custom_theme_colors(0x4aedffu, 0xff8acdu, 0xb03e80u, 0xffffffu, 0x000000u, 0xddddddu);
    } else {
        set_custom_theme_colors(color1, color2, color3, color4, color5, color6);
    }
}

void configure(void) {
    custom_theme();
    board_size();
    default_screen();
}
