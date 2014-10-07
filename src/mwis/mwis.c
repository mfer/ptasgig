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
#include "dialogs.h"
GtkWindow* window;

void normalize_diameter(mwis_set* D){
    int i;
    for (i=1; i < D->N; i++) {
        D->diameter[i] = D->diameter[i] / D->dmax;
    }
}

void min_max_diameter(mwis_set* D){
    int i=0;
    float dmin=D->diameter[i], dmax=D->diameter[i];
    for (i=1; i < D->N; i++) {
        if(dmin > D->diameter[i]) dmin=D->diameter[i];
        if(dmax < D->diameter[i]) dmax=D->diameter[i];
    }
    D->dmin=dmin;
    D->dmax=dmax;
}

//receive a context and made it a set
// normalizing the diameters <--- TODO
// calculating the minimum diameter
mwis_set* mwis_set_new(mwis_context* c)
{
    int N = c->keys;

    /* Check input is correct */
    if(N < 1) {
        return NULL;
    }

    /* Allocate structures */
    mwis_set* D = (mwis_set*) malloc(sizeof(mwis_set));
    if(D == NULL) {
        return NULL;
    }

    /* Try to allocate x */
    D->x = (float*) malloc(N * sizeof(float));
    if(D->x == NULL) {
        return NULL;
    }

    /* Try to allocate y */
    D->y = (float*) malloc(N * sizeof(float));
    if(D->y == NULL) {
        return NULL;
    }

    /* Try to allocate diameter */
    D->diameter = (float*) malloc(N * sizeof(float));
    if(D->diameter == NULL) {
        return NULL;
    }

    /* Try to allocate weight */
    D->weight = (float*) malloc(N * sizeof(float));
    if(D->weight == NULL) {
        return NULL;
    }

//TODO: make the normalization of diameters! The biggest has to be made unitary in this new scale.    

    /* Fill set */
    D->N = c->keys;
    D->x = c->keys_x;
    D->y = c->keys_y;
    D->diameter = c->keys_diameter;
    D->weight = c->keys_weight;

    int i;
    for(i=0;i<D->N;i++){
        printf("%d %f %f %f %f\n", i, D->x[i], D->y[i], D->diameter[i], D->weight[i]);
    }

    min_max_diameter(D);
    normalize_diameter(D);

    return D;
}

void mwis_set_free(mwis_set* D)
{
    free(D->x);
    free(D->y);
    free(D->diameter);
    free(D->weight);
    free(D);
    return;
}

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

void generate_subsets(mwis_context *c){
    //just a draft code to generate de subsets
    int *S;
    int i,j=0;
    int k, N;    
    N = c->keys;
    S = (int *) calloc((N+1), (N+1) * sizeof(int));
    S[0] = -1;
    k = 0;
    while (true) {    // very interesting form to generate all subsets
        if (S[k] < (N-1)) {
            S[k+1] = S[k] + 1;
            k += 1;
        } else {
            S[k-1] += 1;
            k -= 1;
        }
        if (k == 0) break;
        j++;
        printf("%d - %d: {", j, k);
        for(i = 1; i <= k; i++){
            printf ("%d, ", S[i]);
        } 
        printf("}\n");
    }
}

bool mwis(mwis_context* c)
{
    /* Start counting time */
    GTimer* timer = g_timer_new();

        //alpha code =^)
        generate_subsets(c);

    //pre-processing
        int k = c->k;

        /* Create and Fill set */
        mwis_set* D = NULL;
        if(D != NULL) {
            mwis_set_free(D);
        }
        D = mwis_set_new(c);
        if(D == NULL) {
            show_error(window, "Unable to allocate enough memory");
            return false;
        }

        
        int l = (int)(log(1/D->dmin)/log(k+1));
        printf("(k -- dmin -- dmax -- l)=(%d -- %f -- %f -- %d)\n", k,D->dmin,D->dmax,l);

        int i;
        for(i=0;i<D->N;i++){
            printf("%d %f %f %f %f\n", i, D->x[i], D->y[i], D->diameter[i], D->weight[i]);
        }


    //implementing 2.4 algorythm
        int j,r,s,partition,p,q,g,h;
        float delta,deltaprime;
        printf("j r s partition p q delta g h deltprime\n");
        for (j=l;j>=0;j--){
            for (r=0;r<k;r++){
                for (s=0;s<k;s++){
                    
                    partition=pow((k+1),j);

                    for (p=0;p<partition;p++){
                        for (q=0;q<partition;q++){

                            delta=1.0/(float)partition;
//                            S = new_square(p*delta,q*delta,(p+1)*delta,(q+1)*delta)

//                            init_sub_set(level(intersects(D,S),"least",j),I)
//                            while (next_sub_set(I)){
//                                if (disjoint(get_sub_set(I))){

                                    for (g=0;g<=k;g++){
                                        for (h=0;h<=k;h++){

                                            deltaprime = delta/(float)(1+k);
//                                            Sprime new_sub_square(    p*delta + g*deltaprime,(p+1)*delta + (g+1)*deltaprime,(q+1)*delta + (h+1)*deltaprime )

//                                            init_aux_set(S,I,Sprime)
//                                            Q = intersects(D,Sprime)

//                                            init_sub_set_with_size(Q, U,C*k^2)
//                                            while (next_sub_set(Q)) {
//                                                if(size(disjoint(I,U))>0){

//                                                    Iprime = intersects(I,Sprime)
//                                                    X = union(get_set(union(Iprime,U)),contained(U,Sprime))
//                                                    J = boundary(U,Sprime)
//                                                    AT = get_aux_set(S,I,Sprime,J)
//                                                    if size(AT) == 0 || weight(X) > weight(AT) {
//                                                        update_aux_set(S,I,Sprime,J,X)
                                                        printf("%d %d %d %d %d %d %f %d %d %f\n",j,r,s,partition,p,q,delta,g,h,deltaprime);
//                                                    }
//                                                }
//                                            }
                                        }
                                    }
//                                }
//                            }
                        }
                    }

                }
            }
        }


    /* Stop counting time */
    g_timer_stop(timer);
    c->execution_time = g_timer_elapsed(timer, NULL);
    g_timer_destroy(timer);
    return true;
}
