/*
 * Copyright (C) 2012 Carolina Aguilar <caroagse@gmail.com>
 * Copyright (C) 2012 Carlos Jenkins <carlos@jenkins.co.cr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mwis.h"

mwis_context* mwis_context_new(int keys)
{
    /* Check input is correct */
    if(keys < 1) {
        return NULL;
    }

    /* Allocate structures */
    mwis_context* c = (mwis_context*) malloc(sizeof(mwis_context));
    if(c == NULL) {
        return NULL;
    }

    /* Try to allocate keys' x */
    c->keys_x = (float*) malloc(keys * sizeof(float));
    if(c->keys_x == NULL) {
        return NULL;
    }

    /* Try to allocate keys' y */
    c->keys_y = (float*) malloc(keys * sizeof(float));
    if(c->keys_y == NULL) {
        return NULL;
    }

    /* Try to allocate keys' diameter */
    c->keys_diameter = (float*) malloc(keys * sizeof(float));
    if(c->keys_diameter == NULL) {
        return NULL;
    }

    /* Try to allocate keys' weight */
    c->keys_weight = (float*) malloc(keys * sizeof(float));
    if(c->keys_weight == NULL) {
        return NULL;
    }

    /* Try to allocate matrices */
    int size = keys + 1;
    c->table_a = matrix_new(size, size, PLUS_INF);
    if(c->table_a == NULL) {
        free(c->keys_x);
        free(c->keys_y);
        free(c->keys_diameter);
        free(c->keys_weight);
        return NULL;
    }
    c->table_r = matrix_new(size, size, 0.0);
    if(c->table_r == NULL) {
        free(c->keys_x);
        free(c->keys_y);
        free(c->keys_diameter);        
        free(c->keys_weight);
        matrix_free(c->table_a);
        return NULL;
    }

    /* Try to allocate names array */
    c->names = (char**) malloc(keys * sizeof(char*));
    if(c->names == NULL) {
        matrix_free(c->table_a);
        matrix_free(c->table_r);
        free(c->keys_x);
        free(c->keys_y);
        free(c->keys_diameter);
        free(c->keys_weight);
        free(c);
        return NULL;
    }
    c->keys = keys;

    /* Initialize values */
    for(int i = 0; i < size; i++) {
        c->table_a->data[i][i] = 0.0;
    }
    for(int i = 0; i < keys; i++) {
        c->names[i] = "";
    }

    c->status = -1;
    c->execution_time = 0;
    c->memory_required = matrix_sizeof(c->table_a) +
                         matrix_sizeof(c->table_r) +
                         (keys * sizeof(float)) +
                         (keys * sizeof(char*)) +
                         sizeof(mwis_context);
    c->report_buffer = tmpfile();
    if(c->report_buffer == NULL) {
        matrix_free(c->table_a);
        matrix_free(c->table_r);
        free(c->keys_x);
        free(c->keys_y);
        free(c->keys_diameter);
        free(c->keys_weight);
        free(c);
        return NULL;
    }

    return c;
}

void mwis_context_free(mwis_context* c)
{
    matrix_free(c->table_a);
    matrix_free(c->table_r);
    fclose(c->report_buffer);
    free(c->keys_x);
    free(c->keys_y);
    free(c->keys_diameter);
    free(c->keys_weight);
    free(c->names);
    free(c);
    return;
}

bool mwis(mwis_context *c)
{
    /* Start counting time */
    GTimer* timer = g_timer_new();

//just a draft code to generate de subsets
        int *S;
        int i;
        int k, N;    
        N = c->keys;
        S = (int *) calloc((N+1), (N+1) * sizeof(int));
        S[0] = -1;
        k = 0;
        while (true) 
        {    // very interesting form to generate all subsets
            if (S[k] < (N-1)) {
              S[k+1] = S[k] + 1;
              k += 1;
            } else {
              S[k-1] += 1;
              k -= 1;
            }
            if (k == 0) break;
    //        printf("%d: {", k);
            for(i = 1; i <= k; i++){
    //         printf ("%d, ", S[i]);
            } 
    //        printf("}\n");
        }


//just begining with the disks x, y, diameter and weight
    for (i=0;i<N;i++){
        printf("%d %f %f %f %f\n",i, c->keys_x[i], c->keys_y[i], c->keys_diameter[i], c->keys_weight[i]);
    }

    /* Stop counting time */
    g_timer_stop(timer);
    c->execution_time = g_timer_elapsed(timer, NULL);
    g_timer_destroy(timer);
    return true;
}
