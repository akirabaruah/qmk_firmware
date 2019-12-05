// this is the style you want to emulate.
// This is the canonical layout file for the Quantum project. If you want to add another keyboard,

#include QMK_KEYBOARD_H
#include "atreus_split_featherble.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QW 0

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QW] = LAYOUT( /* Qwerty */
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_ESC,  RESET,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P    ,
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    RESET,   KC_DEL,  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN ,
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_LSFT, KC_ENT,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH ,
    KC_TAB,  KC_ESC,  KC_LGUI, KC_LSFT, KC_BSPC, KC_LCTL, RESET,   KC_SPC,  KC_DOWN, KC_UP,   KC_MINS, KC_QUOT
  ),
};
