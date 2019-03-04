#ifndef INNER_FUNCTION__H
#define INNER_FUNCTION__H

#include <stdio.h>
#include <stdlib.h>

int expand(uint64_t* expand, uint32_t R);
int process_S_box(uint32_t* result, SUB_KEY elem);
int permutation_inner_function(uint32_t* output, uint32_t input);
int permutation_inv_inner_function(uint32_t* output, uint32_t input);
int inner_function(SUB_KEY sub_key, uint32_t* R);

#endif
