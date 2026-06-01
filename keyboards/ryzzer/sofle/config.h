/* Copyright
 *   2026 Ryzzer
 *   2021 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// USB Device descriptor parameters
#define VENDOR_ID       0xFC32
#define PRODUCT_ID      0x1287 //Original is 0x0287 but we do not want to load default VIA keymap as it has errors for the bottom row
#define DEVICE_VER      0x0002
#define MANUFACTURER    "Keyhive"
#define PRODUCT         "Ryzzer_Sofle_v2_RGB" //New unique name

// Set which side is master
#define MASTER_LEFT

#define DYNAMIC_KEYMAP_LAYER_COUNT 5

// Key Matrix Settings
// Rows are doubled-up for split keyboards. Added extra column for rotary encoder VIA mapping.
#define MATRIX_ROWS 10
#define MATRIX_COLS 7
#define MATRIX_ROW_PINS { C6, D7, E6, B4, B5 }
#define MATRIX_COL_PINS { B6, B2, B3, B1, F7, F6, NO_PIN } //NO_PIN is used for the encoder key matrix in via.
#define MATRIX_ROW_PINS_RIGHT { C6, D7, E6, B4, B5 }
#define MATRIX_COL_PINS_RIGHT { F6, F7, B1, B3, B2, B6, NO_PIN } //NO_PIN is used for the encoder key matrix in via.
#define DIODE_DIRECTION COL2ROW
#define DEBOUNCE 6 //Default is 5

// NKRO Settings
#ifdef NKRO_ENABLE
	#define FORCE_NKRO //Force NKRO if enabled.
#endif

// Tapping settings
#define TAP_CODE_DELAY 10
#define TAPPING_TOGGLE 2 //Tap TT twice to toggle layer
#define TAPPING_TERM 160 //Tapping duration in ms

// Combo settings
#ifdef COMBO_ENABLE
	#define COMBO_COUNT 1 //Number of tap combos used
	#define COMBO_TERM 400 //Default combo time is 200ms. This is the time delay allowed between deleting each whole word with shift-backspace.
#endif

// Disabled to save space
#define NO_ACTION_ONESHOT		//Save 244 bytes (-244).
#define NO_RESET				//Save 40 bytes (-40).
#define LAYER_STATE_8BIT		//For less than 8 bits worth of layers.
#undef LOCKING_SUPPORT_ENABLE	//For MX lock keys only.
#undef LOCKING_RESYNC_ENABLE	//For MX lock keys only.

// Encoder support
// Change from ENCODERS_PAD_A to ENCODER_A_PINS
#define ENCODER_A_PINS { F5 }
#define ENCODER_B_PINS { F4 }

// Change from ENCODERS_PAD_A_RIGHT to ENCODER_A_PINS_RIGHT
#define ENCODER_A_PINS_RIGHT { F4 }
#define ENCODER_B_PINS_RIGHT { F5 }
// Individual resolution adjustments for each side
#define ENCODER_RESOLUTIONS_LEFT  { 4 }
#define ENCODER_RESOLUTIONS_RIGHT { 2 }

// Communication between sides
#define SOFT_SERIAL_PIN D2

// OLED settings
#ifdef OLED_ENABLE
	#define OLED_TIMEOUT 80000			//80000 = 80secs, 120000 = 2mins in ms.
	#define OLED_BRIGHTNESS 90			//Default is 100.
	#define SPLIT_OLED_ENABLE			//Synx on/off OLED state between halves (+100).
	#ifdef WPM_ENABLE
		#define SPLIT_WPM_ENABLE			//Enable WPM across split keyboards (+106-268).
	#endif
	#define OLED_LOGO					//Enable to print snakey custom logo on slave side (+108).
	//#define SNEAK_DISABLE				//Disable snakey keyboard pet sneak animation to save space (-132).
	//#define OLED_NO_SLAVE				//Disable snakey minimal keyboard pet slave OLED rendering (-160).
	//#define OLED_NO_MASTER				//Disable snakey minimal keyboard pet master OLED rendering and render status on the slave (+96).
	#ifdef OLED_NO_MASTER
		#define SPLIT_LAYER_STATE_ENABLE	//Keep on master to save space (+142).
		#define SPLIT_LED_STATE_ENABLE		//Keep on master to save space (+112).
		#undef SPLIT_OLED_ENABLE			//Do not sync OLED state with one OLED only (+100).
	#endif
	//#define SPLIT_MODS_ENABLE			//Keep on master to save space (+138).
#endif

// RGB settings
#ifdef RGBLIGHT_ENABLE
	#define RGB_DI_PIN D3
	#define WS2812_DI_PIN D3
	#define RGBLED_NUM 74
	#define RGBLED_SPLIT {37,37}
	//#define RGBLIGHT_ANIMATIONS // Very memory intensive (+2604)
	#define RGBLIGHT_EFFECT_STATIC_GRADIENT //Preferred RGB effect (+262)
	#define RGBLIGHT_LIMIT_VAL 160 //Power draw may still exceed the USB limitations of 0.6A at max brightness with white colour with this setting.
	//#define RGBLIGHT_LAYERS //Enable layer light indicators. Not required as updates are done in layer_state_set_user and led_update_user (+588).
	#define RGBLIGHT_SLEEP //Turn off LEDs when computer sleeping (+72)

	//RGB LED Conversion macro from physical array to electric array (+146). This results in better looking animated effects.
	//First section is the LED matrix, second section is the electrical wiring order, and the third section is the desired mapping
	#define LED_LAYOUT( \
		L00,	L01,	L02,	L03,	L04,	L05,					L08,	L09,	L010,	L011,	L012,	L013, \
		L10,			L12,			L14,									L19,			L111,			L113, \
		L20,	L21,	L22,	L23,	L24,	L25,					L28,	L29,	L210,	L211,	L212,	L213, \
		L30,	L31,	L32,	L33,	L34,	L35,					L38,	L39,	L310,	L311,	L312,	L313, \
		L40,			L42,			L44,									L49,			L411,			L413, \
		L50,	L51,	L52,	L53,	L54,	L55,					L58,	L59,	L510,	L511,	L512,	L513, \
						L62,	L63,	L64,	L65,	L66,	L67,	L68,	L69,	L610,	L611,				  \
				L71,									L76,	L77,									L712		 )\
		{ \
			L14,L12,L10,L40,L42,L44,L71,L76,L66,L65,L55,L35,L25,L05,L04,L24,L34,L54,L64,L63,L53,L33,L23,L03,L02,L22,L32,L52,L62,L51,L31,L21,L01,L00,L20,L30,L50,L19,L111,L113,L413,L411,L49,L712,L77,L67,L68,L58,L38,L28,L08,L09,L29,L39,L59,L69,L610,L510,L310,L210,L010,L011,L211,L311,L511,L611,L512,L312,L212,L012,L013,L213,L313,L513, \
		}
	//RGB LED logical order map
	#define RGBLIGHT_LED_MAP LED_LAYOUT( \
		5,	6,	17,	18,	29,	30,			43,	44,	55,	56,	67,	68, \
		4,		16,		28,					45,		57,		69, \
		3,	7,	15,	19,	27,	31,			42,	46,	54,	58,	66,	70, \
		2,	8,	14,	20,	26,	32,			41,	47,	53,	59,	65,	71, \
		1,		13,		25,					48,		60,		72, \
		0,	9,	12,	21,	24,	33,			40,	49,	52,	61,	64,	73, \
				11,	22,	23,	34,	35,	38,	39,	50,	51,	62,		    \
				10,				36,	37,				63		    )
#endif
