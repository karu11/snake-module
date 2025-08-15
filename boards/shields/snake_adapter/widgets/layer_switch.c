/*
 * Copyright (c) 2019 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 *
 * Based on ST7789V sample:
 * Copyright (c) 2019 Marc Reilly
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zmk/display.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>

#include <zmk/display.h>

#include "layer_switch.h"
#include "snake.h"
#include "output_status.h"
#include "battery_status.h"
#include "helpers/display.h"
#include "theme.h"

static uint8_t *buf_frame;

static bool layer_switch_initialized = false;
static struct layer_status_state ls_state;

static bool menu_on = false;
static bool dongle_lock = false;

static const uint8_t base_layer = 0;
static const uint8_t menu_layer = 4;
static const uint8_t theme_layer = 5;

struct layer_status_state {
    uint8_t index;
    const char *label;
};

void print_frames() {
    // board frame
    print_rectangle(buf_frame, 0, 237, 0, 237, get_frame_color(), 2);

    // status frames
    print_rectangle(buf_frame, 2, 119, 2, 158, get_frame_color(), 2);
    print_rectangle(buf_frame, 4, 117, 4, 156, get_frame_color_1(), 2);

    // theme frames
    print_rectangle(buf_frame, 121, 236, 2, 158, get_frame_color(), 2);
    print_rectangle(buf_frame, 123, 234, 4, 156, get_frame_color_1(), 2);

    // battery frames 
    print_rectangle(buf_frame, 2, 119, 160, 236, get_frame_color(), 2);
    print_rectangle(buf_frame, 4, 117, 162, 234, get_frame_color_1(), 2);

    print_rectangle(buf_frame, 121, 236, 160, 236, get_frame_color(), 2);
    print_rectangle(buf_frame, 123, 234, 162, 234, get_frame_color_1(), 2);
}

void print_menu() {
    clear_area();
    print_frames();
    start_battery_status();
    start_output_status();
    set_status_symbol();
    set_battery_symbol();
    print_themes();
}

void toggle_menu() {
    if (menu_on) {
        stop_output_status();
        stop_battery_status();
        restart_snake();
        menu_on = false;
    } else {
        stop_snake();
        print_menu();
        menu_on = true;
    }
}

void change_theme() {
    set_next_theme();
    if (menu_on) {
        print_menu();
    } else {
        stop_snake();
        apply_theme_snake();
        restart_snake();
    }
}

void set_layer_symbol() {
    if (dongle_lock) {
        return;
    }
    dongle_lock = true;
    if (ls_state.index == menu_layer) {
        toggle_menu();
        set_layer(ls_state.index, base_layer);
    }
    if (ls_state.index == theme_layer) {
        change_theme();
        set_layer(ls_state.index, base_layer);
    }
    dongle_lock = false;
}

void set_layer(uint8_t current_layer, uint8_t target_layer) {
    zmk_keymap_layer_deactivate(current_layer);
    zmk_keymap_layer_activate(target_layer);
    // maybe use this? 
    // int zmk_keymap_layer_to(zmk_keymap_layer_id_t layer);
}

static void layer_status_update_cb(struct layer_status_state state) {
    ls_state = state;
    if (layer_switch_initialized) {
        set_layer_symbol();
    }
}

static struct layer_status_state layer_status_get_state(const zmk_event_t *eh) {
    uint8_t index = zmk_keymap_highest_layer_active();
    return (struct layer_status_state) {
        .index = index,
        .label = zmk_keymap_layer_name(index)
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct layer_status_state, layer_status_update_cb,
                            layer_status_get_state)

ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);

void zmk_widget_layer_switch_init() {
	widget_layer_status_init();

    buf_frame = (uint8_t*)k_malloc(320 * 2 * sizeof(uint8_t));
}

void start_layer_switch() {
    layer_switch_initialized = true;
}