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

#ifndef GQRM_SHORTEST_PATH_TREE_H
#define GQRM_SHORTEST_PATH_TREE_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "header.h"
#include "graph.h"
#include "single_linked_list.h"

pt_ALGraph ALGraph_ShortestPathTree(pt_ALGraph, gqrm_id_t, gqrm_id_t [], size_t);
#endif
