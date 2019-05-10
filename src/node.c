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

#include "node.h"

/* @enum
 * Enumerate type for identifying the type of a wireless node:
 * SN  - wireless sensor node
 * CDL - candidate deployment location
 * GW  - gateway
 */
enum NODE_T {
    SN, CDL, GW
};

/* @enum
 * Enumerate type for identifying the status of a CDL:
 * SLCT    - this CDL is selected to place a relay node
 * UNSLCT  - this CDL is not selected to place a relay node
 */
enum STATUS {
    SLCT, UNSLCT
};

/* @struct
 * Structure representing a wireless node:
 * pcoor     - the coordinate of this wireless node
 * id        - the series number of this wireless node
 * power     - the transmit power of this wireless node
 * hop       - the hop constraint imposed on this wireless node
 * type      - the node type, i.e., SN, CDL, or GW.
 * status    - identifying whether a CDL is selected to place relay
 */
struct NODE {
    pt_Coordinate      pcoor;
    gqrm_id_t               id;
    gqrm_power_t            power;
    gqrm_hop_t              hop;
    node_t             type;
    cdl_status         status;
};

static pt_Node create_node(pt_Coordinate, gqrm_id_t, gqrm_power_t, gqrm_hop_t, node_t, cdl_status);
static ds_stat print(pt_Node, FILE*, int);

/* @fn
 * Create a sensor node.
 */
pt_Node
Node_CreateSN(pt_Coordinate c, gqrm_id_t i, gqrm_power_t p, 
              gqrm_hop_t h)
{
    return create_node(c, i, p, h, SN, UNSLCT);
}

pt_Node
Node_CreateRandomSN(gqrm_id_t i, gqrm_power_t p, gqrm_hop_t h)
{
    pt_Coordinate  co = Coordinate_CreateRandom2D(UPPER_RIGHT, LOWER_LEFT);
    return Node_CreateSN(co, i, p, h);
}

/* @fn
 * Create a candidate deployment location.
 */
pt_Node
Node_CreateCDL(pt_Coordinate c, gqrm_id_t i, gqrm_power_t p, 
              gqrm_hop_t h, cdl_status s)
{
    return create_node(c, i, p, h, CDL, s);
}

pt_Node
Node_CreateRandomCDL(gqrm_id_t i, gqrm_power_t p, gqrm_hop_t h)
{
    pt_Coordinate  co = Coordinate_CreateRandom2D(UPPER_RIGHT, LOWER_LEFT);
    return Node_CreateCDL(co, i, p, h, UNSLCT);
}

/* @fn
 * Create a gateway.
 */
pt_Node
Node_CreateGW(pt_Coordinate c, gqrm_id_t i, gqrm_power_t p, 
              gqrm_hop_t h)
{
    return create_node(c, i, p, h, GW, UNSLCT);
}

pt_Node
Node_CreateRandomGW(gqrm_id_t i, gqrm_power_t p, gqrm_hop_t h)
{
    pt_Coordinate  co = Coordinate_CreateRandom2D(UPPER_RIGHT, LOWER_LEFT);
    return Node_CreateGW(co, i, p, h);
}

/*
 * BE AWARE, the power level used in this program
 * is the ABSOLUTE value. So only NON-NEGATIVE value
 * is allowed.
 */
static pt_Node
create_node(pt_Coordinate c, gqrm_id_t i, gqrm_power_t p, 
            gqrm_hop_t h, node_t t, cdl_status s)
{
    pt_Node pn = malloc(sizeof(Node));
    if (!pn)
        return NULL;

    pn->pcoor    = c;
    pn->id       = i;
    pn->power    = p;
    pn->hop      = h;
    pn->type     = t;
    pn->status   = s;

    return pn;
}

/* @fn
 * Free a node. It's worth noting that the coordinate of 
 * this node will BE FREED in this procedure automatically,
 * and this coordinate CANNOT BE USED anymore.
 */
void
Node_Free(pt_Node* nd)
{
    if (!nd)
        return;

    Coordinate_Free(&(*nd)->pcoor);
    free(*nd);
    *nd = NULL;
}

ds_stat
Node_GetStatus(pt_Node nd, cdl_status* re)
{
    if (!nd || !re) 
        return DS_ERROR;

    *re = nd->status;
    return DS_OK;
}

ds_stat
Node_SetStatus(pt_Node nd, cdl_status s)
{
    if (!nd)
        return DS_ERROR;

    nd->status = s;
    return DS_OK;
}

/* @fn
 * Set a new coordinate to a wireless node. It's
 * worth noting that the original coordinate of 
 * this node will BE AUTOMATICALLY FREED in this
 * procedure, so DO NOT USE that coordinate anymore.
 */
ds_stat
Node_SetCoordinate(pt_Node nd, pt_Coordinate co)
{
    if (!nd)
        return DS_ERROR;

    Coordinate_Free(&nd->pcoor);
    nd->pcoor = co;
    return DS_OK;
}

ds_stat
Node_GetHop(pt_Node nd, gqrm_hop_t* re)
{
    if (!nd)
        return DS_ERROR;

    *re = nd->hop;
    return DS_OK;
}

ds_stat
Node_SetHop(pt_Node nd, gqrm_hop_t h)
{
    if (!nd)
        return DS_ERROR;

    nd->hop = h;
    return DS_OK;
}

ds_stat
Node_GetPower(pt_Node nd, gqrm_power_t* re)
{
    if (!nd || !re)
        return DS_ERROR;

    *re = nd->power;
    return DS_OK;
}

ds_stat
Node_SetPower(pt_Node nd, gqrm_power_t p)
{
    if (!nd)
        return DS_ERROR;

    nd->power = p;
    return DS_OK;
}

/* @fn
 * Return the coordinate of this node through parameter "re".
 */
ds_stat
Node_GetCoordinate(pt_Node nd, pt_Coordinate* re)
{
    if (!nd || !re)
        return DS_ERROR;

    *re = nd->pcoor;
    return DS_OK;
}

/* @fn
 * Return the serier number of this node.
 */
ds_stat
Node_GetID(pt_Node nd, gqrm_id_t* re)
{
    if (!nd || !re)
        return DS_ERROR;

    *re = nd->id;
    return DS_OK;
}

ds_bool
Node_IsSN(pt_Node nd)
{
    if (!nd)
        return DS_FALSE;
    return nd->type == SN ? DS_TRUE : DS_FALSE;
}

ds_bool
Node_IsCDL(pt_Node nd)
{
    if (!nd)
        return DS_FALSE;

    return nd->type == CDL ? DS_TRUE : DS_FALSE;
}

ds_bool
Node_IsGW(pt_Node nd)
{
    if (!nd)
        return DS_FALSE;

    return nd->type == GW ? DS_TRUE : DS_FALSE;
}

ds_bool
Node_IsSelected(pt_Node nd)
{
    if (!nd)
        return DS_FALSE;

    return nd->status == SLCT ? DS_TRUE : DS_FALSE;
}

/* @fn
 * Check whether two pointers point to the same wireless node.
 */
ds_bool
Node_IsSame(pt_Node n1, pt_Node n2)
{
    if (!n1 || !n2)
        return DS_FALSE;

    if (n1->id == n2->id)
        return DS_TRUE;
    return DS_TRUE;
}

/* @fn
 * Compute the Euclidean distance between two wireless nodes.
 * It's worth noting that DO NOT input NULL pointer. If so,
 * program will halt.
 */
coordinate_t
Node_Distance(pt_Node n1, pt_Node n2)
{
    return Coordinate_Distance(n1->pcoor, n2->pcoor);
}

ds_stat
Node_2DPrint(pt_Node nd, FILE* fp)
{
    return print(nd, fp, 1);
}

ds_stat
Node_3DPrint(pt_Node nd, FILE* fp)
{
    return print(nd, fp, 0);
}

static ds_stat
print(pt_Node nd, FILE* fp, int flag)
{
    if (!nd || !fp)
        return DS_ERROR;

    switch (nd->type) {
        case SN:
            fprintf(fp, "sensor   "); break;
        case CDL:
            fprintf(fp, "cdl      "); break;
        case GW:
            fprintf(fp, "gateway  "); break;
        default:
            return DS_ERROR;
    }

    if (flag)
        Coordinate_2DPrint(nd->pcoor, fp);
    else
        Coordinate_3DPrint(nd->pcoor, fp);
    fprintf(fp, " ID: %10ld ", nd->id);
    fprintf(fp, "power: %2.2lf ", nd->power);

    if (Node_IsSN(nd) == DS_TRUE)
        fprintf(fp, "hop constraint: %2ld", nd->hop);
    if (Node_IsCDL(nd) == DS_TRUE)
        if (Node_IsSelected(nd) == DS_TRUE)
            fprintf(fp, "status: selected");
        else
            fprintf(fp, "status: unselected");
    return DS_OK;
}

/* @fn
 * Check whether two wireless nodes are neighbors (i.e., whether
 * they can communicate with each other directly) according to
 * the wireless link quality between them. It's worth noting that
 * the power level is always NON-NEGATIVE, otherwise, the program
 * WILL HALT!
 */
ds_bool
Node_IsNeighbor(pt_Node n1, pt_Node n2)
{
    coordinate_t   p;

    if (!n1 || !n2)
        return DS_FALSE;
    assert(n1->power >= 0.0);
    assert(n2->power >= 0.0);

    p = prr(n1->power, Node_Distance(n1, n2));
    printf("prr: %lf\n", p);
    if (isnan(p) || p < PRR_CONSTRAINT)
        return DS_FALSE;
    return DS_TRUE;
}
