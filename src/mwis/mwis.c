/*
 * Copyleft ()) 2014 Manassés Ferreira Neto <sudolshw@gmail.com>
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

mwis_context* mwis_context_new(int nodes)
{
    /* Check input is correct */
    if(nodes < 2) {
        return NULL;
    }

    /* Allocate structure */
    mwis_context* c = (mwis_context*) malloc(sizeof(mwis_context));
    if(c == NULL) {
        return NULL;
    }

    /* Try to allocate matrices */
    c->table_d = matrix_new(nodes, nodes, PLUS_INF);
    if(c->table_d == NULL) {
        free(c);
        return NULL;
    }
    c->table_p = matrix_new(nodes, nodes, 0.0);
    if(c->table_p == NULL) {
        matrix_free(c->table_d);
        free(c);
        return NULL;
    }

    /* Try to allocate items array */
    c->names = (char**) malloc(nodes * sizeof(char*));
    if(c->names == NULL) {
        matrix_free(c->table_d);
        matrix_free(c->table_p);
        free(c);
        return NULL;
    }

    /* Initialize values */
    for(int i = 0; i < nodes; i++) {
        c->table_d->data[i][i] = 0.0;
        c->names[i] = "";
    }

    c->nodes = nodes;

    c->status = -1;
    c->execution_time = 0.0;
    c->memory_required = (matrix_sizeof(c->table_d) * 2) +
                         (nodes * sizeof(char*)) +
                         sizeof(mwis_context);
    c->report_buffer = tmpfile();
    if(c->report_buffer == NULL) {
        matrix_free(c->table_d);
        matrix_free(c->table_p);
        free(c->names);
        free(c);
        return NULL;
    }

    return c;
}

void mwis_context_free(mwis_context* c)
{
    matrix_free(c->table_d);
    matrix_free(c->table_p);
    fclose(c->report_buffer);
    free(c->names);
    free(c);
    return;
}

bool mwis(mwis_context *c)
{
    /* Create graph and first iteration */
    mwis_graph(c->table_d, c->names);
    mwis_execution(c, 0);

    /* Start counting time */
    GTimer* timer = g_timer_new();

    /* Run the MWIS PTAS */
    matrix* d = c->table_d;
    matrix* p = c->table_p;
    int nodes = d->rows;


    /* First we need to do the
      FIG 2.4 Computing the auxiliary table AT_S,I (S_g,h, ∗). 
      http://goo.gl/XvNkg4
    */

    int k = 5;
    double d_min = 0.001; 
    int l = (int)(log(1/d_min)/log(k+1));
    printf("%d\n",l);



    /* And then, we do
      FIG 2.7 Computing the auxiliary table AT_S,I (S_g1··g3,h1··h2, ∗).
      http://goo.gl/hmi9tI
    */

    for(int k = 0; k < nodes; k++) {
        for(int i = 0; i < nodes; i++) {
            for(int j = 0; j < nodes; j++) {
                float minimum = fminf(d->data[i][j],
                                      d->data[i][k] + d->data[k][j]);
                if(minimum < d->data[i][j]) {
                    p->data[i][j] = k + 1;
                    d->data[i][j] = minimum;
                }
            }
        }
        /* Log execution */
        mwis_execution(c, k + 1);
    }

    /* Stop counting time */
    g_timer_stop(timer);
    c->execution_time = g_timer_elapsed(timer, NULL);
    g_timer_destroy(timer);
    return true;
}
