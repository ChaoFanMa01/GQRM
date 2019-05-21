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

#include "c1np.h"


/* TODO
 *  Search all feasible neighbors of the nodes to
 * be covered.
 * The feasible neighbor should meet two conditions:
 * 1) "neighbor" is a 1-hop neighbor of "dst"
 * 2) let s1 mean the least hop count from "neighbor"
 *    to "src", s2 mean the hop constraint imposed 
 *    on "dst". Then s1 <= s2 - 1.
 */
static pt_ALGraph
search_feasible_neighbor(pt_ALGraph orgin, pt_ALGraph spt, 
                     gqrm_id_t dsts[], size_t n)
{

}

