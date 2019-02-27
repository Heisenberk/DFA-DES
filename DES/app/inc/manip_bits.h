#ifndef MANIP_BITS__H
#define MANIP_BITS__H

#include <stdint.h>
#include "key_schedule.h"

uint8_t get_bit_uint64_t (uint64_t elem, uint8_t i);
uint8_t get_bit_uint32_t (uint32_t elem, uint8_t i);
uint8_t get_bit_uint8_t (uint8_t elem, uint8_t i);
uint8_t get_bit_uint64_t_most (uint64_t elem, uint8_t i);
uint8_t get_bit_uint32_t_most (uint32_t elem, uint8_t i);
uint8_t get_6bits_uint64_t_most (SUB_KEY elem, uint8_t i);

int set_bit_uint64_t (uint64_t* elem, uint8_t bit, uint8_t pos);
int set_bit_uint32_t (uint32_t* elem, uint8_t bit, uint8_t pos);

void printf_uint64_t_binary(uint64_t key);
void printf_uint32_t_binary(uint32_t key);
void printf_uint8_t_binary(uint8_t key);
void printf_uint64_t_hexa(uint64_t key);
void printf_uint32_t_hexa(uint32_t key);

#endif