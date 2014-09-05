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

#ifndef H_FLOYD_REPORT
#define H_FLOYD_REPORT

#include "mwis.h"
#include "graphviz.h"

/**
 * Write a report about the execution of the algorithm.
 *
 * @param mwis_context, the mwis's context data structure after
 *        successful execution.
 * @return if report creation was successful.
 */
bool mwis_report(mwis_context* c);
void mwis_execution(mwis_context* c, int k);
void mwis_table(matrix* m, bool d, int k, FILE* stream);
void mwis_graph(matrix* m, char** n);

#endif
