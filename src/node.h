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

#ifndef GQRM_NODE_H
#define GQRM_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "coordinate.h"
#include "prr.h"
#include "single_linked_list.h"

typedef enum NODE_T     node_t; 
typedef enum STATUS     cdl_status;
typedef struct NODE     Node;
typedef Node*           pt_Node;
typedef struct NODES    Nodes;
typedef Nodes*          pt_Nodes;

extern pt_Node      Node_CreateSN(pt_Coordinate, gqrm_id_t, gqrm_power_t, gqrm_hop_t);
extern pt_Node      Node_CreateRandomSN(gqrm_id_t, gqrm_power_t, gqrm_hop_t);
extern pt_Node      Node_CreateCDL(pt_Coordinate, gqrm_id_t, gqrm_power_t, gqrm_hop_t, cdl_status);
extern pt_Node      Node_CreateRandomCDL(gqrm_id_t, gqrm_power_t, gqrm_hop_t);
extern pt_Node      Node_CreateGW(pt_Coordinate, gqrm_id_t, gqrm_power_t, gqrm_hop_t);
extern pt_Node      Node_CreateRandomGW(gqrm_id_t, gqrm_power_t, gqrm_hop_t);
extern void         Node_Free(pt_Node*);
extern ds_stat      Node_GetCoordinate(pt_Node, pt_Coordinate*);
extern ds_stat      Node_SetCoordinate(pt_Node, pt_Coordinate);
extern ds_stat      Node_GetID(pt_Node, gqrm_id_t*);
extern ds_stat      Node_GetPower(pt_Node, gqrm_power_t*);
extern ds_stat      Node_GetHop(pt_Node, gqrm_hop_t*);
extern ds_stat      Node_GetStatus(pt_Node, cdl_status*);
extern ds_stat      Node_SetStatus(pt_Node, cdl_status);
extern ds_stat      Node_SetHop(pt_Node, gqrm_hop_t);
extern ds_stat      Node_SetPower(pt_Node, gqrm_power_t);
extern ds_bool      Node_IsSN(pt_Node);
extern ds_bool      Node_IsCDL(pt_Node);
extern ds_bool      Node_IsGW(pt_Node);
extern ds_bool      Node_IsSelected(pt_Node);
extern ds_bool      Node_IsSame(pt_Node, pt_Node);
extern coordinate_t Node_Distance(pt_Node, pt_Node);
extern ds_stat      Node_2DPrint(pt_Node, FILE*);
extern ds_stat      Node_3DPrint(pt_Node, FILE*);
extern ds_bool      Node_IsNeighbor(pt_Node, pt_Node, double*);


extern pt_Nodes     Nodes_Create(void);
extern ds_stat      Nodes_PushNode(pt_Nodes, pt_Node);
extern ds_stat      Nodes_PopNode(pt_Nodes);
extern void         Nodes_Clear(pt_Nodes);
extern void         Nodes_Free(pt_Nodes*);
extern size_t       Nodes_Size(pt_Nodes);
extern ds_bool      Nodes_Empty(pt_Nodes);
extern ds_stat      Nodes_GetNode(pt_Nodes, size_t, pt_Node*);
#endif
