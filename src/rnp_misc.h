/*
 * GQRM
 * Copyright (C) 2019-2025 Chaofan Ma <chaofanma@hotmail.com>
 *
 * This file is part of GQRM.
 *
 * GQRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GQRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GQRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GQRM_RNP_MISC_H
#define GQRM_RNP_MISC_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "header.h"
#include "node.h"
#include "graph.h"
#include "shortest_path_tree.h"

extern ds_bool         check_feasibility(pt_ALGraph, gqrm_id_t, gqrm_id_t [], size_t);
extern edge_weight_t   check_neighbor(graph_data_t, graph_data_t);
extern ds_bool         is_VertexSN(pt_Vertex);
extern ds_bool         is_VertexCDL(pt_Vertex);
extern ds_bool         is_VertexGW(pt_Vertex);

#endif
