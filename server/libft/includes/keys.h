/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/12 12:07:29 by edrowzee          #+#    #+#             */
/*   Updated: 2021/02/07 00:22:57 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
# define KEYS_H

# define ESC 53
# define SPACE 49
# define LEFT 123
# define RIGHT 124
# define DOWN 125
# define UP 126
# define NUMBER_1 18
# define NUMBER_2 19
# define NUMBER_3 20
# define NUMBER_4 21
# define NUMBER_5 22
# define NUMBER_6 23
# define KEY_H 4
# define KEY_T 17
# define MOUSE_SCRLUP 4
# define MOUSE_SCRLDOWN 5
# define MOUSE_LEFT 1
# define MOUSE_RIGHT 2
# define MOUSE_MID 3
# define BUTTON_W 13
# define BUTTON_A 0
# define BUTTON_S 1
# define BUTTON_D 2
# define L_CTRL 256

enum			e_keys
{
	KVK_ANSI_A = 0x00,
	KVK_ANSI_S = 0x01,
	KVK_ANSI_D = 0x02,
	KVK_ANSI_F = 0x03,
	KVK_ANSI_H = 0x04,
	KVK_ANSI_G = 0x05,
	KVK_ANSI_Z = 0x06,
	KVK_ANSI_X = 0x07,
	KVK_ANSI_C = 0x08,
	KVK_ANSI_V = 0x09,
	KVK_ANSI_B = 0x0B,
	KVK_ANSI_Q = 0x0C,
	KVK_ANSI_W = 0x0D,
	KVK_ANSI_E = 0x0E,
	KVK_ANSI_R = 0x0F,
	KVK_ANSI_Y = 0x10,
	KVK_ANSI_T = 0x11,
	KVK_ANSI_1 = 0x12,
	KVK_ANSI_2 = 0x13,
	KVK_ANSI_3 = 0x14,
	KVK_ANSI_4 = 0x15,
	KVK_ANSI_6 = 0x16,
	KVK_ANSI_5 = 0x17,
	KVK_ANSI_Equal = 0x18,
	KVK_ANSI_9 = 0x19,
	KVK_ANSI_7 = 0x1A,
	KVK_ANSI_Minus = 0x1B,
	KVK_ANSI_8 = 0x1C,
	KVK_ANSI_0 = 0x1D,
	KVK_ANSI_RightBracket = 0x1E,
	KVK_ANSI_O = 0x1F,
	KVK_ANSI_U = 0x20,
	KVK_ANSI_LeftBracket = 0x21,
	KVK_ANSI_I = 0x22,
	KVK_ANSI_P = 0x23,
	KVK_ANSI_L = 0x25,
	KVK_ANSI_J = 0x26,
	KVK_ANSI_Quote = 0x27,
	KVK_ANSI_K = 0x28,
	KVK_ANSI_Semicolon = 0x29,
	KVK_ANSI_Backslash = 0x2A,
	KVK_ANSI_Comma = 0x2B,
	KVK_ANSI_Slash = 0x2C,
	KVK_ANSI_N = 0x2D,
	KVK_ANSI_M = 0x2E,
	KVK_ANSI_Period = 0x2F,
	KVK_ANSI_Grave = 0x32,
	KVK_ANSI_KeypadDecimal = 0x41,
	KVK_ANSI_KeypadMultiply = 0x43,
	KVK_ANSI_KeypadPlus = 0x45,
	KVK_ANSI_KeypadClear = 0x47,
	KVK_ANSI_KeypadDivide = 0x4B,
	KVK_ANSI_KeypadEnter = 0x4C,
	KVK_ANSI_KeypadMinus = 0x4E,
	KVK_ANSI_KeypadEquals = 0x51,
	KVK_ANSI_Keypad0 = 0x52,
	KVK_ANSI_Keypad1 = 0x53,
	KVK_ANSI_Keypad2 = 0x54,
	KVK_ANSI_Keypad3 = 0x55,
	KVK_ANSI_Keypad4 = 0x56,
	KVK_ANSI_Keypad5 = 0x57,
	KVK_ANSI_Keypad6 = 0x58,
	KVK_ANSI_Keypad7 = 0x59,
	KVK_ANSI_Keypad8 = 0x5B,
	KVK_ANSI_Keypad9 = 0x5C,
	KVK_Return = 0x24,
	KVK_Tab = 0x30,
	KVK_Space = 0x31,
	KVK_Delete = 0x33,
	KVK_Escape = 0x35,
	KVK_Command = 0x37,
	KVK_Shift = 0x38,
	KVK_CapsLock = 0x39,
	KVK_Option = 0x3A,
	KVK_Control = 0x3B,
	KVK_RightShift = 0x3C,
	KVK_RightOption = 0x3D,
	KVK_RightControl = 0x3E,
	KVK_Function = 0x3F,
	KVK_F17 = 0x40,
	KVK_VolumeUp = 0x48,
	KVK_VolumeDown = 0x49,
	KVK_Mute = 0x4A,
	KVK_F18 = 0x4F,
	KVK_F19 = 0x50,
	KVK_F20 = 0x5A,
	KVK_F5 = 0x60,
	KVK_F6 = 0x61,
	KVK_F7 = 0x62,
	KVK_F3 = 0x63,
	KVK_F8 = 0x64,
	KVK_F9 = 0x65,
	KVK_F11 = 0x67,
	KVK_F13 = 0x69,
	KVK_F16 = 0x6A,
	KVK_F14 = 0x6B,
	KVK_F10 = 0x6D,
	KVK_F12 = 0x6F,
	KVK_F15 = 0x71,
	KVK_Help = 0x72,
	KVK_Home = 0x73,
	KVK_PageUp = 0x74,
	KVK_ForwardDelete = 0x75,
	KVK_F4 = 0x76,
	KVK_End = 0x77,
	KVK_F2 = 0x78,
	KVK_PageDown = 0x79,
	KVK_F1 = 0x7A,
	KVK_LeftArrow = 0x7B,
	KVK_RightArrow = 0x7C,
	KVK_DownArrow = 0x7D,
	KVK_UpArrow = 0x7E,
	kVK_Backspace = 0x7F
};

#endif
