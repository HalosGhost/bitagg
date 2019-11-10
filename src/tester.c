#include <stdio.h>
#include <stdlib.h>

#include "bitbloom.h"

signed
main (void) {

    bitbloom(unsigned) bloom = bitbloom_optimal_init(unsigned, 50, 0.01);

    printf("bloom: %zu bits (%zu bytes), using %zu hashes\n", bloom.bits, bytes_needed(unsigned, bloom.bits), bloom.num_hashes);

    bloom_insert(bloom, "test", 4);
    bloom_insert(bloom, "pants", 4);

    bool res;
    bloom_elem(bloom, "test", 4, res);
    printf("%s may be in set: %d\n", "test", res);

    bloom_elem(bloom, "pants", 4, res);
    printf("%s may be in set: %d\n", "pant", res);

    bloom_elem(bloom, "pants", 5, res);
    printf("%s may be in set: %d\n", "pants", res);

    bloom_elem(bloom, "stuf", 4, res);
    printf("%s may be in set: %d\n", "stuf", res);

    size_t pop = 0;
    bitpopcount(bloom.data, bloom.bits, pop);
    printf("popcount: %zu\n", pop);

    double elements = 0;
    bloom_approximate_count(bloom, elements);
    printf("elements: %f\n", elements);

    if ( bloom.data ) { free(bloom.data); }

    return EXIT_SUCCESS;
}

