#include "matrix.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "wait.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "debounce.h"
#include "quantum.h"
#include "action_layer.h"
#include "i2c_master.h"
#include QMK_KEYBOARD_H

// Must shift device address left 1 bit
#define TCA8418_I2C_ADDRESS (0x34<<1)
#define TCA8418_I2C_TIMEOUT 100

#define KEY_EVENT_LIST_SIZE 10

#define REGISTER_CFG 0x01
#define REGISTER_KP_GPIO1 0x1D
#define REGISTER_KP_GPIO2 0x1E
#define REGISTER_KP_GPIO3 0x1F

#define REGISTER_KEY_EVENT_A 0x04
#define REGISTER_KEY_EVENT_B 0x05
#define REGISTER_KEY_EVENT_C 0x06
#define REGISTER_KEY_EVENT_D 0x07
#define REGISTER_KEY_EVENT_E 0x08
#define REGISTER_KEY_EVENT_F 0x09
#define REGISTER_KEY_EVENT_G 0x0A
#define REGISTER_KEY_EVENT_H 0x0B
#define REGISTER_KEY_EVENT_I 0x0C
#define REGISTER_KEY_EVENT_J 0x0D

void tca8418_init(void) {
  print("tca8418_init() start\n");
  i2c_init();

  /*
    | ADDRESS | REGISTER NAME | REGISTER DESCRIPTION | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0 |
    |---------+---------------+----------------------+------+------+------+------+------+------+------+------|
    |    0x1D | KP_GPIO1      | Keypad/GPIO Select 1 | ROW7 | ROW6 | ROW5 | ROW4 | ROW3 | ROW2 | ROW1 | ROW0 |
    |    0x1E | KP_GPIO2      | Keypad/GPIO Select 2 | COL7 | COL6 | COL5 | COL4 | COL3 | COL2 | COL1 | COL0 |
    |    0x1F | KP_GPIO3      | Keypad/GPIO Select 3 | N/A  | N/A  | N/A  | N/A  | N/A  | N/A  | COL9 | COL8 |
  */

  // everything enabled in key scan mode
  uint8_t enabled_rows = 0x0F; // ROWS 3 to 0
  uint8_t enabled_cols = 0x3F; // COLS 5 to 0

  i2c_writeReg(TCA8418_I2C_ADDRESS, REGISTER_KP_GPIO1, &enabled_rows, 1, TCA8418_I2C_TIMEOUT);
  i2c_writeReg(TCA8418_I2C_ADDRESS, REGISTER_KP_GPIO2, &enabled_cols, 1, TCA8418_I2C_TIMEOUT);
  enabled_cols = 0;
  i2c_writeReg(TCA8418_I2C_ADDRESS, REGISTER_KP_GPIO3, &enabled_cols, 1, TCA8418_I2C_TIMEOUT);

  /*
    BIT: NAME

    7: AI
    Auto-increment for read and write operations; See below table for more information
    0 = disabled
    1 = enabled

    6: GPI_E_CFG
    GPI event mode configuration
    0 = GPI events are tracked when keypad is locked
    1 = GPI events are not tracked when keypad is locked

    5: OVR_FLOW_M
    Overflow mode
    0 = disabled; Overflow data is lost
    1 = enabled; Overflow data shifts with last event pushing first event out

    4: INT_CFG
    Interrupt configuration
    0 = processor interrupt remains asserted (or low) if host tries to clear interrupt while there is
    still a pending key press, key release or GPI interrupt
    1 = processor interrupt is deasserted for 50 μs and reassert with pending interrupts

    3: OVR_FLOW_IEN
    Overflow interrupt enable
    0 = disabled; INT is not asserted if the FIFO overflows
    1 = enabled; INT becomes asserted if the FIFO overflows

    2: K_LCK_IEN
    Keypad lock interrupt enable
    0 = disabled; INT is not asserted after a correct unlock key sequence
    1 = enabled; INT becomes asserted after a correct unlock key sequence

    1: GPI_IEN
    GPI interrupt enable to host processor
    0 = disabled; INT is not asserted for a change on a GPI
    1 = enabled; INT becomes asserted for a change on a GPI

    0: KE_IEN
    Key events interrupt enable to host processor
    0 = disabled; INT is not asserted when a key event occurs
    1 = enabled; INT becomes asserted when a key event occurs
   */

  // 10111001 xB9 -- fifo overflow enabled
  // 10011001 x99 -- fifo overflow disabled

  uint8_t cfg = 0x99;
  i2c_writeReg(TCA8418_I2C_ADDRESS, REGISTER_CFG, &cfg, 1, TCA8418_I2C_TIMEOUT);
  _delay_ms(10);
  print("tca8418_init() done\n");
}

bool tca8418_update(matrix_row_t current_matrix[]) {
  bool matrix_changed = false;
  uint8_t key_event = 0;
  uint8_t key_code, key_down, key_row, key_col;

  i2c_readReg(TCA8418_I2C_ADDRESS, REGISTER_KEY_EVENT_A, &key_event, 1, TCA8418_I2C_TIMEOUT);

  // if there is a new event
  if (key_event > 0) {
    matrix_changed = true;

    key_code = key_event & 0x7F;
    key_down = (key_event & 0x80) >> 7;
    key_row = key_code / MATRIX_COLS_PER_SIDE;
    key_col = key_code % MATRIX_COLS_PER_SIDE;

    xprintf("event: %d, code: %d, down: %d, row: %d, col: %d\n", key_event, key_code, key_down, key_row, key_col);

    if (key_down) {
    }
    else {
    }
  }

  return matrix_changed;
}

#if (MATRIX_COLS <= 8)
#    define print_matrix_header() print("\nr/c 01234567\n")
#    define print_matrix_row(row) print_bin_reverse8(matrix_get_row(row))
#    define matrix_bitpop(i) bitpop(matrix[i])
#    define ROW_SHIFTER ((uint8_t)1)
#elif (MATRIX_COLS <= 16)
#    define print_matrix_header() print("\nr/c 0123456789ABCDEF\n")
#    define print_matrix_row(row) print_bin_reverse16(matrix_get_row(row))
#    define matrix_bitpop(i) bitpop16(matrix[i])
#    define ROW_SHIFTER ((uint16_t)1)
#elif (MATRIX_COLS <= 32)
#    define print_matrix_header() print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
#    define print_matrix_row(row) print_bin_reverse32(matrix_get_row(row))
#    define matrix_bitpop(i) bitpop32(matrix[i])
#    define ROW_SHIFTER ((uint32_t)1)
#endif

#ifdef MATRIX_MASKED
extern const matrix_row_t matrix_mask[];
#endif

#ifdef DIRECT_PINS
static pin_t direct_pins[MATRIX_ROWS][MATRIX_COLS] = DIRECT_PINS;
#elif (DIODE_DIRECTION == ROW2COL) || (DIODE_DIRECTION == COL2ROW)
static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
#endif

/* matrix state(1:on, 0:off) */
static matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
static matrix_row_t matrix[MATRIX_ROWS];      // debounced values

__attribute__((weak)) void matrix_init_kb(void) { matrix_init_user(); }

__attribute__((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

inline uint8_t matrix_rows(void) { return MATRIX_ROWS; }

inline uint8_t matrix_cols(void) { return MATRIX_COLS; }

// Deprecated.
bool matrix_is_modified(void) {
    if (debounce_active()) return false;
    return true;
}

inline bool matrix_is_on(uint8_t row, uint8_t col) { return (matrix[row] & ((matrix_row_t)1 << col)); }

inline matrix_row_t matrix_get_row(uint8_t row) {
    // Matrix mask lets you disable switches in the returned matrix data. For example, if you have a
    // switch blocker installed and the switch is always pressed.
#ifdef MATRIX_MASKED
    return matrix[row] & matrix_mask[row];
#else
    return matrix[row];
#endif
}

void matrix_print(void) {
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row);
        print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

uint8_t matrix_key_count(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += matrix_bitpop(i);
    }
    return count;
}

#ifdef DIRECT_PINS

static void init_pins(void) {
    for (int row = 0; row < MATRIX_ROWS; row++) {
        for (int col = 0; col < MATRIX_COLS; col++) {
            pin_t pin = direct_pins[row][col];
            if (pin != NO_PIN) {
                setPinInputHigh(pin);
            }
        }
    }
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    matrix_row_t last_row_value = current_matrix[current_row];
    current_matrix[current_row] = 0;

    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
        pin_t pin = direct_pins[current_row][col_index];
        if (pin != NO_PIN) {
            current_matrix[current_row] |= readPin(pin) ? 0 : (ROW_SHIFTER << col_index);
        }
    }

    return (last_row_value != current_matrix[current_row]);
}

#elif (DIODE_DIRECTION == COL2ROW)

static void select_row(uint8_t row) {
    setPinOutput(row_pins[row]);
    writePinLow(row_pins[row]);
}

static void unselect_row(uint8_t row) { setPinInputHigh(row_pins[row]); }

static void unselect_rows(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        setPinInputHigh(row_pins[x]);
    }
}

static void init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        setPinInputHigh(col_pins[x]);
    }
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Store last value of row prior to reading
    matrix_row_t last_row_value = current_matrix[current_row];

    // Clear data in matrix row
    current_matrix[current_row] = 0;

    // Select row and wait for row selecton to stabilize
    select_row(current_row);
    wait_us(30);

    // For each col...
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {
        // Select the col pin to read (active low)
        uint8_t pin_state = readPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_matrix[current_row] |= pin_state ? 0 : (ROW_SHIFTER << col_index);
    }

    // Unselect row
    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

#elif (DIODE_DIRECTION == ROW2COL)

static void select_col(uint8_t col) {
    setPinOutput(col_pins[col]);
    writePinLow(col_pins[col]);
}

static void unselect_col(uint8_t col) { setPinInputHigh(col_pins[col]); }

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        setPinInputHigh(col_pins[x]);
    }
}

static void init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        setPinInputHigh(row_pins[x]);
    }
}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    bool matrix_changed = false;

    // Select col and wait for col selecton to stabilize
    select_col(current_col);
    wait_us(30);

    // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Store last value of row prior to reading
        matrix_row_t last_row_value = current_matrix[row_index];

        // Check row pin state
        if (readPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= (ROW_SHIFTER << current_col);
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~(ROW_SHIFTER << current_col);
        }

        // Determine if the matrix changed state
        if ((last_row_value != current_matrix[row_index]) && !(matrix_changed)) {
            matrix_changed = true;
        }
    }

    // Unselect col
    unselect_col(current_col);

    return matrix_changed;
}

#endif

void matrix_init(void) {
    print("matrix_init()\n");
    // initialize key pins
    init_pins();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        raw_matrix[i] = 0;
        matrix[i]     = 0;
    }

    debounce_init(MATRIX_ROWS);

    tca8418_init();
    matrix_init_quantum();
}

uint8_t matrix_scan(void) {
    bool changed = false;

#if defined(DIRECT_PINS) || (DIODE_DIRECTION == COL2ROW)
    // Set row, read cols
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        changed |= read_cols_on_row(raw_matrix, current_row);
    }
#elif (DIODE_DIRECTION == ROW2COL)
    // Set col, read rows
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
      if (current_col < MATRIX_COLS_PER_SIDE) {
        // USB/Microcontroller Side
        changed |= read_rows_on_col(raw_matrix, current_col);
      }
      // else {
        // i2c Side
        // TODO read tca8418 i2c fifo
      // }
    }
    changed |= tca8418_update(raw_matrix);

#endif

    debounce(raw_matrix, matrix, MATRIX_ROWS, changed);

    matrix_scan_quantum();
    return (uint8_t)changed;
}
