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

#ifndef GQRM_HEADER_H
#define GQRM_HEADER_H

#include <stdlib.h>
#include <sys/types.h>

/*
 * These two macros identifying a square deployment field:
 *
 *                          O-------------O    <--- upper right corner
 *                          |             |
 *                          |             |
 *                          |             |
 *                          |             |
 *                          |             |
 * lower left corner --->   O-------------O
 */
#define   UPPER_RIGHT     100
#define   LOWER_LEFT      0

typedef enum {
    DS_OK, DS_ERROR
} ds_stat;

typedef enum {
    DS_FALSE, DS_TRUE
} ds_bool;

typedef double     coordinate_t;
typedef double     gqrm_power_t;
typedef ssize_t    gqrm_id_t;
typedef ssize_t    gqrm_hop_t;

#endif
