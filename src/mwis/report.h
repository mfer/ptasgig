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

#ifndef H_MWIS_REPORT
#define H_MWIS_REPORT

#include "mwis.h"
#include "graphviz.h"

/**
 * Write a report about the execution of the algorithm.
 *
 * @param mwis_context, the Optimal Binary Search Tree's context data
 *        structure after successful execution.
 * @return if report creation was successful.
 */
bool mwis_report(mwis_context* c);
void mwis_nodes(mwis_context* c, FILE* stream);
void mwis_execution(mwis_context* c, FILE* stream);
void mwis_table(matrix* m, bool a, FILE* stream);
int mwis_graph(mwis_context* c);
int find_nodes(matrix* r, int i, int j, FILE* stream);
int find_lnodes(matrix* r, int i, int j, FILE* stream, int level);
int find_rnodes(matrix* r, int i, int j, FILE* stream, int level);

#endif
