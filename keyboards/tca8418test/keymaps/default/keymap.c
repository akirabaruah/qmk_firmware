// this is the style you want to emulate.
// This is the canonical layout file for the Quantum project. If you want to add another keyboard,

#include QMK_KEYBOARD_H
#include "tca8418test.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

               /* RESET, KC_W, KC_E, KC_R, KC_T, KC_Y,        RESET, KC_W, KC_E, KC_R, KC_T, KC_Y, */
               /* KC_U, KC_I, KC_O, KC_P, KC_A, KC_S,         KC_U, KC_I, KC_O, KC_P, KC_A, KC_S, */
               /* KC_LSFT, KC_RSFT, KC_H, KC_D, KC_X, KC_K,   KC_LSFT, KC_RSFT, KC_H, KC_D, KC_X, KC_K, */
               /* KC_L, KC_Z, KC_X, KC_C, KC_V, RESET,        KC_L, KC_Z, KC_X, KC_C, KC_V, RESET */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( /* Qwerty */
               KC_LSFT,    KC_RSFT,     KC_H,    KC_X,    KC_D,    RESET,   RESET,   KC_J,    KC_L,    KC_U,     KC_Y,     KC_SCLN, \
               KC_A,    KC_R,     KC_S,    KC_T,    KC_D,    RESET,   RESET,   KC_H,    KC_N,    KC_E,     KC_I,     KC_O, \
               KC_Z,    KC_X,     KC_C,    KC_V,    KC_B,    RESET,   RESET,   KC_K,    KC_M,    KC_COMM,  KC_DOT,   KC_SLSH, \
               KC_FN7,  KC_LGUI,  KC_TAB,  KC_FN1,  KC_FN6,  KC_FN3,  KC_FN4,  KC_FN5,  KC_FN0,  KC_MINS,  KC_QUOT,  KC_FN8),
};
