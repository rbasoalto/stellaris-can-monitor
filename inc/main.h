#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

// Data buffer types

typedef struct __attribute__((__packed__)) {
  uint16_t wheel1;
  uint16_t wheel2;
  uint8_t flags;
  uint8_t seq;
  uint16_t _pad0;
} wheel_t;

typedef struct __attribute__((__packed__)) {
  uint16_t rpm;
  uint8_t _pad0;
  uint8_t unk0;
  uint8_t _pad1;
  uint8_t _pad2;
  uint8_t unk1;
  int8_t unk2;
} engine_t;

typedef struct __attribute__((__packed__)) {
  uint16_t fuel;
  uint16_t _pad0;
  uint16_t _pad1;
  uint16_t _pad2;
} fuel_t;

#endif