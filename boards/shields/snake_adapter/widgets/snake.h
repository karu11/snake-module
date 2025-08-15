/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */
 
 #pragma once

#include <zephyr/kernel.h>
#include <lvgl.h>

void start_snake(void);
void restart_snake(void);
void stop_snake(void);
void display_setup(void);
void zmk_widget_snake_init(void);

void apply_theme_snake(void);