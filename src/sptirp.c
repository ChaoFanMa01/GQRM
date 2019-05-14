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

#include "sptirp.h"

pt_ALGraph
SPTiRP(p_sll nodes)
{
    pt_ALGraph      tmp;
	gqrm_id_t       src;
	gqrm_id_t       dsts[400];
	size_t          n, i, size;

	size = SingleLinkedList_Size(nodes);
	for (i = 0, n = 0; i < size; i++) {
	    
	}
}

