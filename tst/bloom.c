#include <stdio.h>
#include <stdlib.h>

#include <bitagg.h>

signed
main (void) {

    signed status = EXIT_SUCCESS;
    #define max_members 5
    #define acceptable 0.01
    bitbloom(unsigned) bloom = bitbloom_optimal_init(unsigned, max_members, acceptable);

    bloom_insert(bloom, "testing", 7);
    bloom_insert(bloom, "testing", 7); // no-op
    bloom_insert(bloom, "1, 2, 3", 7);

    double rate = 0;
    bloom_approximate_fp_rate(bloom, rate);
    if ( rate > acceptable ) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    bool res;
    bloom_elem(bloom, "testing", 7, res);
    if ( !res ) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    bloom_elem(bloom, "1, 2, 3", 7, res);
    if ( !res ) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    bloom_elem(bloom, "pants", 5, res);
    if ( res ) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    bloom_elem(bloom, "stuf", 4, res);
    if ( res ) {
        status = EXIT_FAILURE;
        goto cleanup;
    }

    cleanup:
        bitbloom_free(bloom);
        return status;
}

