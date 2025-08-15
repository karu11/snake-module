/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include "custom_status_screen.h"
#include "widgets/battery_status.h"
#include "widgets/output_status.h"
#include "widgets/splash.h"
#include "widgets/snake.h"
#include "widgets/helpers/display.h"
#include "widgets/layer_switch.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static const uint8_t SPLASH_DURATION = 50;
static const uint8_t SPLASH_FINAL_COUNT = 10;
static uint8_t splash_count = 0;
static bool splash_finished = false;

// static uint32_t theme_color1 = 0x222323u;
// static uint32_t theme_color2 = 0xff4adcu;
// static uint32_t theme_color3 = 0x3dff98u;
// static uint32_t theme_color4 = 0xf0f6f0u;

// static uint32_t theme_black = 0x000000u;
// static uint32_t theme_black_1 = 0xddddddu;

void timer_splash(lv_timer_t * timer) {
    if (splash_finished) {
        return;
    }
    if (splash_count >= SPLASH_FINAL_COUNT) {
        print_background();
        start_snake();
        // start_battery_status();
        // start_output_status();
        start_layer_switch();

        lv_timer_pause(timer);
        splash_finished = true;
        return;
    }
    print_splash();
    splash_count++;
}

lv_obj_t* zmk_display_status_screen() {
    init_display();
    // default color scheme 
    // https://lospec.com/palette-list/b4sement best one so far
    set_colors(0x222323u, 0xff4adcu, 0x3dff98u, 0xf0f6f0u, 0x000000u, 0xddddddu);
    
    zmk_widget_splash_init();

    zmk_widget_snake_init();
    zmk_widget_output_status_init();
    zmk_widget_peripheral_battery_status_init();

    zmk_widget_layer_switch_init();
    theme_init();

    lv_timer_create(timer_splash, SPLASH_DURATION, NULL);

    return lv_obj_create(NULL);
}