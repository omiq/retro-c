#include "screens.h"
#include "../display/display.h"
#include "../input/input.h"

#if defined (__CC65__)
#include <conio.h>
#include <stdio.h>
#include <peekpoke.h>
#include <string.h>
#else
#include "../include/notconio.h"
#endif

#ifdef __C64__
// Custom character set data - 8x8 pixel patterns for each character
static const unsigned char chars[] = {
    0x5C,0x57,0xE9,0x59,0x39,0x1E,0x14,0x36,
    0x10,0x28,0x28,0x44,0x7C,0x44,0xEE,0x00,
    0xFC,0x42,0x42,0x7C,0x42,0x42,0xFC,0x00,
    0x38,0x44,0x82,0x80,0x82,0x44,0x38,0x00,
    0xFC,0x42,0x42,0x42,0x42,0x42,0xFC,0x00,
    0xFE,0x42,0x48,0x78,0x48,0x42,0xFE,0x00,
    0xFE,0x42,0x48,0x78,0x48,0x40,0xE0,0x00,
    0x3C,0x42,0x80,0x8E,0x82,0x42,0x3C,0x00,
    0xEE,0x44,0x7C,0x44,0x44,0x44,0xEE,0x00,
    0xFE,0x10,0x10,0x10,0x10,0x10,0xFE,0x00,
    0x7C,0x10,0x10,0x10,0x90,0x90,0x60,0x00,
    0xEE,0x44,0x48,0x70,0x48,0x44,0xEE,0x00,
    0xE0,0x40,0x40,0x40,0x42,0x42,0xFE,0x00,
    0xEE,0x54,0x54,0x54,0x44,0x44,0xEE,0x00,
    0xEE,0x64,0x54,0x54,0x4C,0x4C,0xE4,0x00,
    0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00,
    0xFC,0x42,0x42,0x42,0x7C,0x40,0xE0,0x00,
    0x38,0x44,0x82,0x82,0x92,0x4C,0x3A,0x00,
    0xFC,0x42,0x42,0x42,0x7C,0x48,0xEE,0x00,
    0x7C,0x82,0x80,0x7C,0x02,0x82,0x7C,0x00,
    0xFE,0x92,0x10,0x10,0x10,0x10,0x38,0x00,
    0xEE,0x44,0x44,0x44,0x44,0x44,0x38,0x00,
    0xEE,0x44,0x44,0x28,0x28,0x28,0x10,0x00,
    0xEE,0x44,0x44,0x54,0x54,0x6C,0x44,0x00,
    0xEE,0x44,0x28,0x10,0x28,0x44,0xEE,0x00,
    0xEE,0x44,0x28,0x28,0x10,0x10,0x38,0x00,
    0xFE,0x84,0x08,0x10,0x20,0x42,0xFE,0x00,
    0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,
    0x0C,0x12,0x30,0x7C,0x30,0x62,0xFC,0x00,
    0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,
    0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,
    0x00,0x10,0x30,0x7F,0x7F,0x30,0x10,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x18,0x18,0x18,0x18,0x00,0x00,0x18,0x00,
    0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,
    0xFE,0xD4,0xAA,0xD4,0xAA,0xD4,0xAA,0x00,
    0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00,
    0x00,0x00,0x70,0x5E,0x72,0x00,0x00,0x00,
    0x08,0x94,0x48,0x2E,0x1A,0x24,0x6C,0x00,
    0x06,0x0C,0x18,0x00,0x00,0x00,0x00,0x00,
    0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00,
    0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00,
    0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,
    0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30,
    0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,
    0x00,0x03,0x06,0x0C,0x18,0x30,0x60,0x00,
    0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00,
    0x30,0x50,0x90,0x10,0x10,0x10,0xFE,0x00,
    0x7C,0x82,0x02,0x7C,0x80,0x82,0xFE,0x00,
    0xFE,0x84,0x08,0x1C,0x82,0x82,0x7C,0x00,
    0x0C,0x18,0x28,0x4A,0xFE,0x0A,0x1C,0x00,
    0xFE,0x82,0xFC,0x02,0x82,0x82,0x7C,0x00,
    0x7C,0x82,0x80,0xFC,0x82,0x82,0x7C,0x00,
    0xFE,0x84,0x08,0x08,0x10,0x10,0x38,0x00,
    0x7C,0x82,0x82,0x7C,0x82,0x82,0x7C,0x00,
    0x7C,0x82,0x82,0x82,0x7C,0x04,0x78,0x00,
    0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,
    0x00,0x00,0x18,0x00,0x00,0x18,0x18,0x30,
    0x00,0x7E,0x00,0x3C,0x00,0x18,0x00,0x00,
    0xEF,0x8A,0x0C,0x00,0xFE,0xAA,0x80,0x00,
    0x00,0x00,0x18,0x00,0x3C,0x00,0x7E,0x00,
    0x00,0x3C,0x7E,0xFE,0x7A,0x7E,0xFE,0x7E,
    0x00,0x3C,0x7E,0xFE,0x7A,0x7E,0xFE,0x7E,
    0x08,0x1C,0x3E,0x7F,0x7F,0x1C,0x3E,0x00,
    0x00,0x7E,0xDE,0xFE,0xFE,0xFE,0x7E,0x14,
    0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,
    0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,
    0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,
    0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,
    0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,
    0x00,0x00,0x00,0xE0,0xF0,0x38,0x18,0x18,
    0x18,0x18,0x1C,0x0F,0x07,0x00,0x00,0x00,
    0x18,0x18,0x38,0xF0,0xE0,0x00,0x00,0x00,
    0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,
    0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x07,0x03,
    0x03,0x07,0x0E,0x1C,0x38,0x70,0xE0,0xC0,
    0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
    0xFF,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,
    0x08,0x18,0x99,0x7E,0x3C,0x3C,0x66,0x81,
    0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,
    0x36,0x7F,0x6B,0x63,0x2A,0x1C,0x08,0x00,
    0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
    0x00,0x00,0x00,0x07,0x0F,0x1C,0x18,0x18,
    0xC3,0xE7,0x7E,0x3C,0x3C,0x7E,0xE7,0xC3,
    0x00,0x3C,0x7E,0x66,0x66,0x7E,0x3C,0x00,
    0x18,0x3C,0x72,0x62,0x24,0x18,0x3C,0x00,
    0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
    0x08,0x1C,0x3E,0x7F,0x3E,0x1C,0x08,0x00,
    0x18,0x18,0x18,0xFF,0xFF,0x18,0x18,0x18,
    0xC0,0xC0,0x30,0x30,0xC0,0xC0,0x30,0x30,
    0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
    0x00,0x0C,0x02,0x02,0x2C,0x7C,0xEC,0x00,
    0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
    0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
    0xCC,0x87,0x30,0x66,0x08,0x93,0xC7,0x46,
    0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
    0x00,0x00,0x00,0x00,0xCC,0xCC,0x33,0x33,
    0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,
    0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
    0x18,0x18,0x18,0x1F,0x1F,0x18,0x18,0x18,
    0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,
    0x18,0x18,0x18,0x1F,0x1F,0x00,0x00,0x00,
    0x00,0x00,0x00,0xF8,0xF8,0x18,0x18,0x18,
    0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
    0x00,0x00,0x00,0x1F,0x1F,0x18,0x18,0x18,
    0x18,0x18,0x18,0xFF,0xFF,0x00,0x00,0x00,
    0x00,0x00,0x00,0xFF,0xFF,0x18,0x18,0x18,
    0x18,0x18,0x18,0xF8,0xF8,0x18,0x18,0x18,
    0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
    0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
    0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
    0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
    0x03,0x03,0x03,0x03,0x03,0x03,0xFF,0xFF,
    0x00,0x00,0x00,0x00,0xF0,0xF0,0xF0,0xF0,
    0x0F,0x0F,0x0F,0x0F,0x00,0x00,0x00,0x00,
    0x18,0x18,0x18,0xF8,0xF8,0x00,0x00,0x00,
    0xF0,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,
    0xF0,0xF0,0xF0,0xF0,0x0F,0x0F,0x0F,0x0F,
    0xC3,0x99,0x91,0x91,0x9F,0x99,0xC3,0xFF,
    0xE7,0xC3,0x99,0x81,0x99,0x99,0x99,0xFF,
    0x83,0x99,0x99,0x83,0x99,0x99,0x83,0xFF,
    0xC3,0x99,0x9F,0x9F,0x9F,0x99,0xC3,0xFF,
    0x87,0x93,0x99,0x99,0x99,0x93,0x87,0xFF,
    0x81,0x9F,0x9F,0x87,0x9F,0x9F,0x81,0xFF,
    0x81,0x9F,0x9F,0x87,0x9F,0x9F,0x9F,0xFF,
    0xC3,0x99,0x9F,0x91,0x99,0x99,0xC3,0xFF,
    0x99,0x99,0x99,0x81,0x99,0x99,0x99,0xFF,
    0xC3,0xE7,0xE7,0xE7,0xE7,0xE7,0xC3,0xFF,
    0xE1,0xF3,0xF3,0xF3,0xF3,0x93,0xC7,0xFF,
    0x99,0x93,0x87,0x8F,0x87,0x93,0x99,0xFF,
    0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x81,0xFF,
    0x9C,0x88,0x80,0x94,0x9C,0x9C,0x9C,0xFF,
    0x99,0x89,0x81,0x81,0x91,0x99,0x99,0xFF,
    0xC3,0x99,0x99,0x99,0x99,0x99,0xC3,0xFF,
    0x83,0x99,0x99,0x83,0x9F,0x9F,0x9F,0xFF,
    0xC3,0x99,0x99,0x99,0x99,0xC3,0xF1,0xFF,
    0x83,0x99,0x99,0x83,0x87,0x93,0x99,0xFF,
    0xC3,0x99,0x9F,0xC3,0xF9,0x99,0xC3,0xFF,
    0x81,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xFF,
    0x99,0x99,0x99,0x99,0x99,0x99,0xC3,0xFF,
    0x99,0x99,0x99,0x99,0x99,0xC3,0xE7,0xFF,
    0x9C,0x9C,0x9C,0x94,0x80,0x88,0x9C,0xFF,
    0x99,0x99,0xC3,0xE7,0xC3,0x99,0x99,0xFF,
    0x99,0x99,0x99,0xC3,0xE7,0xE7,0xE7,0xFF,
    0x81,0xF9,0xF3,0xE7,0xCF,0x9F,0x81,0xFF,
    0xC3,0xCF,0xCF,0xCF,0xCF,0xCF,0xC3,0xFF,
    0xF3,0xED,0xCF,0x83,0xCF,0x9D,0x03,0xFF,
    0xC3,0xF3,0xF3,0xF3,0xF3,0xF3,0xC3,0xFF,
    0xFF,0xE7,0xC3,0x81,0xE7,0xE7,0xE7,0xE7,
    0xFF,0xEF,0xCF,0x80,0x80,0xCF,0xEF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xE7,0xE7,0xE7,0xE7,0xFF,0xFF,0xE7,0xFF,
    0x99,0x99,0x99,0xFF,0xFF,0xFF,0xFF,0xFF,
    0x99,0x99,0x00,0x99,0x00,0x99,0x99,0xFF,
    0xE7,0xC1,0x9F,0xC3,0xF9,0x83,0xE7,0xFF,
    0x9D,0x99,0xF3,0xE7,0xCF,0x99,0xB9,0xFF,
    0xC3,0x99,0xC3,0xC7,0x98,0x99,0xC0,0xFF,
    0xF9,0xF3,0xE7,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xF3,0xE7,0xCF,0xCF,0xCF,0xE7,0xF3,0xFF,
    0xCF,0xE7,0xF3,0xF3,0xF3,0xE7,0xCF,0xFF,
    0xFF,0x99,0xC3,0x00,0xC3,0x99,0xFF,0xFF,
    0xFF,0xE7,0xE7,0x81,0xE7,0xE7,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xE7,0xCF,
    0xFF,0xFF,0xFF,0x81,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xE7,0xFF,
    0xFF,0xFC,0xF9,0xF3,0xE7,0xCF,0x9F,0xFF,
    0xC3,0x99,0x91,0x89,0x99,0x99,0xC3,0xFF,
    0xE7,0xE7,0xC7,0xE7,0xE7,0xE7,0x81,0xFF,
    0xC3,0x99,0xF9,0xF3,0xCF,0x9F,0x81,0xFF,
    0xC3,0x99,0xF9,0xE3,0xF9,0x99,0xC3,0xFF,
    0xF9,0xF1,0xE1,0x99,0x80,0xF9,0xF9,0xFF,
    0x81,0x9F,0x83,0xF9,0xF9,0x99,0xC3,0xFF,
    0xC3,0x99,0x9F,0x83,0x99,0x99,0xC3,0xFF,
    0x81,0x99,0xF3,0xE7,0xE7,0xE7,0xE7,0xFF,
    0xC3,0x99,0x99,0xC3,0x99,0x99,0xC3,0xFF,
    0xC3,0x99,0x99,0xC1,0xF9,0x99,0xC3,0xFF,
    0xFF,0xFF,0xE7,0xFF,0xFF,0xE7,0xFF,0xFF,
    0xFF,0xFF,0xE7,0xFF,0xFF,0xE7,0xE7,0xCF,
    0xF1,0xE7,0xCF,0x9F,0xCF,0xE7,0xF1,0xFF,
    0xFF,0xFF,0x81,0xFF,0x81,0xFF,0xFF,0xFF,
    0x8F,0xE7,0xF3,0xF9,0xF3,0xE7,0x8F,0xFF,
    0xC3,0x99,0xF9,0xF3,0xE7,0xFF,0xE7,0xFF,
    0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,
    0xF7,0xE3,0xC1,0x80,0x80,0xE3,0xC1,0xFF,
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,
    0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,
    0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,
    0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,
    0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,
    0xFF,0xFF,0xFF,0x1F,0x0F,0xC7,0xE7,0xE7,
    0xE7,0xE7,0xE3,0xF0,0xF8,0xFF,0xFF,0xFF,
    0xE7,0xE7,0xC7,0x0F,0x1F,0xFF,0xFF,0xFF,
    0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x00,0x00,
    0x3F,0x1F,0x8F,0xC7,0xE3,0xF1,0xF8,0xFC,
    0xFC,0xF8,0xF1,0xE3,0xC7,0x8F,0x1F,0x3F,
    0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
    0x00,0x00,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
    0xFF,0xC3,0x81,0x81,0x81,0x81,0xC3,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,
    0xC9,0x80,0x80,0x80,0xC1,0xE3,0xF7,0xFF,
    0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,0x9F,
    0xFF,0xFF,0xFF,0xF8,0xF0,0xE3,0xE7,0xE7,
    0x3C,0x18,0x81,0xC3,0xC3,0x81,0x18,0x3C,
    0xFF,0xC3,0x81,0x99,0x99,0x81,0xC3,0xFF,
    0xE7,0xE7,0x99,0x99,0xE7,0xE7,0xC3,0xFF,
    0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,0xF9,
    0xF7,0xE3,0xC1,0x80,0xC1,0xE3,0xF7,0xFF,
    0xE7,0xE7,0xE7,0x00,0x00,0xE7,0xE7,0xE7,
    0x3F,0x3F,0xCF,0xCF,0x3F,0x3F,0xCF,0xCF,
    0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,
    0xFF,0xFF,0xFC,0xC1,0x89,0xC9,0xC9,0xFF,
    0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
    0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
    0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
    0x33,0x33,0xCC,0xCC,0x33,0x33,0xCC,0xCC,
    0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
    0xFF,0xFF,0xFF,0xFF,0x33,0x33,0xCC,0xCC,
    0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,
    0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,
    0xE7,0xE7,0xE7,0xE0,0xE0,0xE7,0xE7,0xE7,
    0xFF,0xFF,0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,
    0xE7,0xE7,0xE7,0xE0,0xE0,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x07,0x07,0xE7,0xE7,0xE7,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
    0xFF,0xFF,0xFF,0xE0,0xE0,0xE7,0xE7,0xE7,
    0xE7,0xE7,0xE7,0x00,0x00,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0x00,0x00,0xE7,0xE7,0xE7,
    0xE7,0xE7,0xE7,0x07,0x07,0xE7,0xE7,0xE7,
    0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
    0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
    0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
    0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,
    0xFC,0xFC,0xFC,0xFC,0xFC,0xFC,0x00,0x00,
    0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,
    0xF0,0xF0,0xF0,0xF0,0xFF,0xFF,0xFF,0xFF,
    0xE7,0xE7,0xE7,0x07,0x07,0xFF,0xFF,0xFF,
    0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,
    0x0F,0x0F,0x0F,0x0F,0xF0,0xF0,0xF0,0xF0
};

// Function to load and setup the custom character set
static void setup_charset(void) {
    // Load our custom character set into memory at $C000 (49152)
    memcpy(49152, chars, sizeof(chars));

    // Configure the C64's memory layout for custom character set:
    
    // 1. Set up RAM bank for character ROM access
    // CIA2 port A (56576) controls RAM bank selection
    POKE(56576, PEEK(57576) & 252);

    // 2. Configure VIC-II to use our custom character set
    // 53272 is the VIC-II control register for character ROM location
    POKE(53272, 32);

    // 3. Set the cursor character to use our custom character set
    POKE(648, 200);
}
#endif

// This is the default title screen
int title_screen(void) {
    // if c64 then set the background color to black and the text to green
    #ifdef __C64__
        bgcolor(0);
        bordercolor(0);
        textcolor(5);
        setup_charset();
    #endif

    clrscr();
    
    sprintf(output, "pet dungeon");
    cputsxy(11, 10, output);
    
    sprintf(output, "a game by retrogamecoders.com");
    cputsxy(2, 15, output);
    
    sprintf(output, "press a key");
    cputsxy(12, 20, output);

    counter = 0;
    while (!kbhit()) { counter++; }
    in_play = true;
    clrscr();
    return counter;
}

bool game_over(void) {
    clrscr();
    sprintf(output, " game over\n\n");
    cputsxy(15, 10, output);
    refresh();
    timer = dumb_wait(1000);
    sprintf(output, " ah, such a shame,");
    cputsxy(16, 12, output);
    sprintf(output, "you were doing so well!");
    cputsxy(16, 14, output);
    timer = dumb_wait(1000);
    refresh();
    sprintf(output, " score:%03d", score);
    cputsxy(20, 18, output);
    sprintf(output, " play again (y/n)?");
    cputsxy(20, 19, output);
    refresh();
    key = cgetc();
    in_play = false;
    if (key == 'n') {
        cursor(1);
        echo();
        refresh();
        return false;
    } else {
        return true;
    }
} 