/* Copyright
 *   2026 ryzzer
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

#include QMK_KEYBOARD_H

#ifdef ENCODER_ENABLE
	#include "sofle\encoder.c"
#endif

#ifdef OLED_ENABLE
	//Note that the keyboard animations below take a large amount of space!
	#include "sofle\snakey_minimal.c" //OLED code for Snakey, without WPM/related animations to save space. If not used, do not use OLED_LOGO in config.h.
#endif

#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_LAYERS)
	extern rgblight_config_t rgblight_config; // To pull layer status for RGBLIGHT
#endif


//Variables for custom keycodes
bool lshift_held = false;	// LShift Backspace Delete whole Word Code
bool rshift_held = false;	// RShift Backspace Delete whole Word Code
static uint16_t held_shift = 0;

#ifdef VIA_ENABLE
	enum custom_keycodes { //Use USER 00 instead of SAFE_RANGE for Via. VIA json must include the custom keycode.
	  ATABF = QK_USER_0, 	//Alt tab forwards
	  ATABR, 			//Alt tab reverse
	  NMR, 				//Move window to monitor on right
	  NML, 				//Move window to monitor on left
	  SBS 				//Shift backspace to delete whole word (Swap KC_BPSC with this)
	};
#else
	enum custom_keycodes { //Use USER 00 instead of SAFE_RANGE for Via. VIA json must include the custom keycode.
	  ATABF = SAFE_RANGE, //Alt tab forwards
	  ATABR, 			//Alt tab reverse
	  NMR, 				//Move window to monitor on right
	  NML, 				//Move window to monitor on left
	  SBS				//Shift backspace to delete whole word (Swap KC_BPSC with this)
	};
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {  //Can skip these hard-coded layouts to save space when using only VIA (+700).
/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |-------.            |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------| Trkbl |            |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|  Enc  |    |  Enc  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
*/
/*[0] = LAYOUT(
  KC_GRV,	KC_1,	KC_2,	KC_3,	KC_4,	KC_5,						KC_6,	KC_7,	KC_8,	KC_9,	KC_0,		KC_MINS,
  KC_ESC,	KC_Q,	KC_W,	KC_E,	KC_R,	KC_T,	KC__VOLUP,	KC_PGUP,KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,		SBS,
  KC_TAB,	KC_A,	KC_S,	KC_D,	KC_F,	KC_G,	KC_MUTE,	KC_NO,	KC_H,	KC_J,	KC_K,	KC_L,	KC_SCLN,	KC_QUOT,
  KC_LSFT,	KC_Z,	KC_X,	KC_C,	KC_V,	KC_B,	KC__VOLDOWN,KC_PGDN,KC_N,	KC_M,	KC_COMM,KC_DOT,	KC_SLSH,	KC_RSFT,
					KC_LGUI,KC_LALT,KC_LCTRL,MO(2),	KC_ENT,		KC_SPC,	MO(3),	KC_RCTRL,KC_RALT,KC_RGUI
),
[1] = LAYOUT(
  KC_PSCR,	KC_5,		KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_F11,		KC_F4,
  KC_T,		KC_ESC,		KC_Q,		KC_W,		KC_E,		KC_R,		KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_F12,
  KC_G,		KC_TAB,		KC_A,		KC_S,		KC_D,		KC_F,		KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
  KC_LSFT,	KC_LSFT,	KC_Z,		KC_X,		KC_C,		KC_V,		KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_RALT,
						KC_ENT,		KC_B,		KC_TRNS,	KC_PAUS,	KC_SPACE,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS
),
[2] = LAYOUT(
  KC_TRNS,	KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,							KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,		KC_F11,
  KC_GRV,	KC_1,	KC_2,	KC_3,	KC_4,	KC_5,	KC_RGHT,		KC_TRNS,KC_6,	KC_7,	KC_8,	KC_9,	KC_0,		KC_F12,
  KC_WREF,	KC_EXLM,KC_AT,	KC_HASH,KC_DLR,	KC_PERC,KC_TRNS,		KC_TRNS,KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,	KC_PIPE,
  KC_TRNS,	KC_EQL,	KC_MINS,KC_PLUS,KC_LCBR,KC_LCBR,KC_LEFT,		KC_TRNS,KC_LBRC,KC_RBRC,KC_DOT,	KC_COLN,KC_BSLS,	KC_TRNS,
					KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,		KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS
),
[3] = LAYOUT(
  NML,		KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	NMR,
  KC_NLCK,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	ATABF,		NMR,		KC_WFWD,	KC_HOME,	KC_UP,		KC_END,		KC_INS,		KC_DEL,
  KC_SLCK,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_WBAK,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	KC_TRNS,	KC_TRNS,
  KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	ATABR,		NML,		KC_CAPS,	KC_PGUP,	KC_TRNS,	KC_PGDN,	KC_TRNS,	KC_TRNS,
						KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS
),
[4] = LAYOUT(
  KC_TRNS,		KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
  KC_NLCK,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,		KC_TRNS,		KC_TRNS,	KC_TRNS,	KC_TRNS,		KC_TRNS,		KC_TRNS,		KC_TRNS,
  KC_SLCK,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
  KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,		KC_TRNS,		KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
						KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS
)*/
};

void matrix_scan_user(void) {
	#ifdef ENCODER_ENABLE
		encoder_action_unregister();
	#endif
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	#if defined(KEYBOARD_PET) || defined(OLED_LOGO)
		if (record->event.pressed) { //OLED timeout code
			oled_timer = timer_read32();
		}
	#endif
	
	switch (keycode) { //For keycode overrides
		case NMR:	//Move window to next monitor on right
		  if (record->event.pressed) {
			register_code(KC_LSFT);
			register_code(KC_LWIN);
			register_code(KC_RIGHT);
			unregister_code(KC_RIGHT);
			unregister_code(KC_LWIN);
			unregister_code(KC_LSFT);
		  }
		  return true;
		case NML:	//Move window to next monitor on left
		  if (record->event.pressed) {
			register_code(KC_LSFT);
			register_code(KC_LWIN);
			register_code(KC_LEFT);
			unregister_code(KC_LEFT);
			unregister_code(KC_LWIN);
			unregister_code(KC_LSFT);
		  }
		  return true;

		case KC_RSFT: //Shift Backspace to Delete Whole Word. Inspired by Hellsingcoder.
			rshift_held = record->event.pressed;
			held_shift = keycode;
			#ifdef KEYBOARD_PET // KEYBOARD PET STATUS
				if (record->event.pressed) {
					isBarking = true;
				} else {
					isBarking = false;
				}
			#endif
			return true;
		case KC_LSFT:
			lshift_held = record->event.pressed;
			held_shift = keycode;
			#ifdef KEYBOARD_PET // KEYBOARD PET STATUS
				if (record->event.pressed) {
					isBarking = true;
				} else {
					isBarking = false;
				}
			#endif
			return true;
		case SBS:
			if (record->event.pressed) { //When left shift is held and backspace pressed, one whole word will be deleted (left).
				if (lshift_held) {
					unregister_code(held_shift);
					register_code(KC_LCTL);
					register_code(KC_BSPC);
				} else if (rshift_held) { //When left shift is held and backspace pressed, one whole word will be deleted (right).
					unregister_code(held_shift);
					register_code(KC_LCTL);
					register_code(KC_DEL);
				} else {
					register_code(KC_BSPC);
				}
			} else {
				unregister_code(KC_BSPC);
				unregister_code(KC_DEL);
				unregister_code(KC_LCTL);
				if (lshift_held || rshift_held) {
					register_code(held_shift);
				}
			}
			return false;
		
		#ifdef KEYBOARD_PET // KEYBOARD PET STATUS
			case KC_LCTL:
			case KC_RCTL:
				#ifndef SNEAK_DISABLE
				if (record->event.pressed) { //Pet sneaks when control held.
					isSneaking = true;
				} else {
					isSneaking = false;
				}
				#endif
				return true;
			case KC_SPC:
				if (record->event.pressed) { //Pet jumps when enter is pressed.
					isJumping = true;
					showedJump = false;
				} else {
					isJumping = false;
				}
				return true;
		#endif
		}
	return true;
}


#ifdef RGBLIGHT_ENABLE
	layer_state_t layer_state_set_user(layer_state_t state)	//Use for layer lighting. This method uses less space than RGBLIGHT_LAYER_SEGMENTS.
	{
		switch (get_highest_layer(state)) { // Change all other LEDs based on layer state as well
			case 0:
				rgblight_sethsv_noeeprom(50,255,80);	//Green → Teal → Blue gradient
				//rgblight_sethsv_noeeprom(115,170,80);	//Pale Teal → Pale Cyan → Pale Blue gradient
				break;
			case 1:
				rgblight_sethsv_noeeprom(252,255,80);	//Red → Orange → Yellow gradient
				break;
			case 2:
				rgblight_sethsv_noeeprom(80,255,80);	//Green → Spring Green → Cyan gradient
				break;
			case 3:
				//rgblight_sethsv_noeeprom(118,255,80);	//Teal → Cyan → Blue gradient
				rgblight_sethsv_noeeprom(160,255,80);	//Blue → Violet → Magenta gradient
				break;
			case 4:
				rgblight_sethsv_noeeprom(218,255,80);	//Magenta → Pink → Red gradient
				break;
		  }
		return state;
	}
	
	bool led_update_user(led_t led_state)	//Lock key status indicators
	{
		if(led_state.caps_lock){
			rgblight_sethsv_range(43,100,170, 4,7); //White-left caps lock indication
		}
		if(!(led_state.num_lock)){
			rgblight_sethsv_range(43,100,170, 28,31); //White-right num lock indication. Since this indicator is inverted, it must be on the master side of the keyboard to shut off properly when the computer is sleeping.
		}
		if(led_state.scroll_lock){
			rgblight_sethsv_range(43,100,170, 16,19); //White-middle scroll lock indication
		}
		return true;
	}
#endif


void keyboard_post_init_user(void)
{
	#ifdef RGBLIGHT_ENABLE
		rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_GRADIENT+8); //Set to static gradient 9
	#endif
	layer_move(0); 						//Start on layer0 by default to set LED colours. Can remove to save a very small amount of space.
}
