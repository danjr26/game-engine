#ifndef KEY_H
#define KEY_H

#include <windows.h>
#include "definitions.h"
#include <vector>

enum class Key : ubyte {
	a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
	quote, colon, plus, minus, period, comma, back_tick, open_bracket, close_bracket, forward_slash, back_slash,
	keypad_1, keypad_2, keypad_3, keypad_4, keypad_5, keypad_6, keypad_7, keypad_8, keypad_9, keypad_0,	
	keypad_plus, keypad_minus, keypad_multiply, keypad_divide, keypad_decimal, keypad_comma,
	f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
	up_arrow, down_arrow, left_arrow, right_arrow,
	space, backspace, _delete, escape, enter, tab,
	left_shift, right_shift, left_ctrl, right_ctrl, left_alt, right_alt, left_windows, right_windows,
	scroll_lock, caps_lock, num_lock,
	home, page_up, page_down, end,
	volume_up, volume_down, volume_mute,
	context_menu, sleep, screenshot,
	unrecognized,
	count
};

Key windowsScanCodeToKey(uint i_vk);

#endif