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

#ifndef GQRM_MYSQL_H
#define GQRM_MYSQL_H

#include <mysql/mysql.h>    // MySQL adaptor.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header.h"
#include "coordinate.h"
#include "node.h"
#include "graph.h"

typedef struct MYSQL_API    Mysql;
typedef Mysql*              pt_Mysql;

extern pt_Mysql    Mysql_Create(void);
extern pt_Mysql    Mysql_CreateInitConnect(const char*, const char*, const char*);
extern ds_stat     Mysql_Init(pt_Mysql);
extern ds_stat     Mysql_Connect(pt_Mysql, const char*, const char*, const char*);
extern void        Mysql_Free(pt_Mysql*);
extern ds_stat     Mysql_Query(pt_Mysql, const char*);
extern ds_stat     Mysql_GetResult(pt_Mysql);
extern ds_stat     Mysql_WriteGraph(pt_ALGraph, pt_Mysql, const char*);
#endif
