/*
 * Copyright (c) 2024 Golioth, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

int app_buzzer_init(void);
void play_beep_once(void);

#if CONFIG_USE_BUZZER
void play_funkytown_once(void);
void play_mario_once(void);
void play_golioth_once(void);
#endif