// this is the style you want to emulate.
// This is the canonical layout file for the Quantum project. If you want to add another keyboard,

#include QMK_KEYBOARD_H
#include "tca8418test.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( /* Qwerty */
               KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y,
               KC_U, KC_I, KC_O, KC_P, KC_A, KC_S,
               KC_D, KC_F, KC_G, KC_H, KC_J, KC_K,
               KC_L, KC_Z, KC_X, KC_C, KC_V, KC_B
                ),
};
