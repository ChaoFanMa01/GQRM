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

 #include "mysql_api.h"

struct MYSQL_API {
    /* handler for MySQL database connection */
    MYSQL*       mysql;
	/* result of a query that returns rows */
	MYSQL_RES*   res;
	/*
	 * A type-safe representation of one row of data.
	 * It is currently implemented as an array of 
	 * counted byte strings.
	 */
     MYSQL_ROW    row;
};

pt_Mysql
Mysql_Create(void)
{
    pt_Mysql pm = malloc(sizeof(Mysql));
	if (!pm)
	    return NULL;
    pm->mysql = NULL;
	pm->res   = NULL;
	return pm;
}

ds_stat
Mysql_Init(pt_Mysql pm)
{
    if (!pm)
        return DS_ERROR;

    if ((pm->mysql = mysql_init(NULL)) == NULL)
        return DS_ERROR;
    return DS_OK;
}

ds_stat
Mysql_Connect(pt_Mysql pm, const char* user, 
              const char* passwd, const char* db)
{
    if ((pm->mysql = mysql_real_connect(pm->mysql, NULL, user,
	                 passwd, db, 0, NULL, 0)) == NULL)
	    return DS_ERROR;
    return DS_OK;
}

pt_Mysql
Mysql_CreateInitConnect(const char* user, 
                        const char* passwd, const char* db)
{
    pt_Mysql pm = Mysql_Create();
	if (!pm)
	    return NULL;
    if (Mysql_Init(pm) == DS_ERROR) {
	    free(pm);
	    return NULL;
	}
	if (Mysql_Connect(pm, user, passwd, db) == DS_ERROR) {
	    mysql_close(pm->mysql);
		free(pm);
		return NULL;
	}
	return pm;
}

void
Mysql_Free(pt_Mysql* pm)
{
    if (!*pm || !(*pm)->mysql)
	    return;
	mysql_close((*pm)->mysql);
	free(*pm);
	*pm = NULL;
}

ds_stat
Mysql_Query(pt_Mysql pm, const char* query)
{
    if (!pm || !pm->mysql)
	    return DS_ERROR;

    assert(pm);
	assert(pm->mysql);

    /* mysql_query() return 0 on success, nonzero otherwise */
    if (mysql_query(pm->mysql, query))
	    return DS_ERROR;
	return DS_OK;
}

/* @fn 
 * Fetch the result of the latest query, and store this 
 * result in the "res" field.
 */
ds_stat
Mysql_GetResult(pt_Mysql pm)
{
    if (!pm ||!pm->mysql)
	    return DS_ERROR;

    /* 
	 * res != NULL denotes a select action
	 * has been successfully done, and the 
	 * result have been stored in res field.
	 */
    if (pm->res = mysql_store_result(pm->mysql)) {
	    return DS_OK;
	} else {
	/*
	 * res == NULL denotes action other than
	 * select has been done.
	 */
	    /*
		 * mysql_field_count() == 0 deontes the 
		 * non-select action has been successfully
		 * done.
		 */
	    if (!mysql_field_count(pm->mysql))
		    return DS_OK;
	    else
		    return DS_ERROR;
	}
}

ds_stat
Mysql_WriteGraph(pt_ALGraph pg, pt_Mysql pm, const char* table)
{
    size_t         size, i, j;
	pt_Vertex      pv;
	pt_Node        pn;
	char           cmd[1000], buf[100];
	gqrm_id_t      id;
	gqrm_power_t   power;
	pt_Coordinate  pcoor;
	coordinate_t   coor;
	pt_Edge        pe;
	p_sll          edges;

    size = ALGraph_Size(pg);
	for (i = 0; i < size; i++) {
        memset(cmd, 0, sizeof(cmd));
        memset(buf, 0, sizeof(buf));
	    sprintf(buf, "INSERT INTO ");
		strcat(cmd, buf);
		strcat(cmd, table);
		sprintf(buf, " VALUES (");
		strcat(cmd, buf);
		if (ALGraph_GetVertex(pg, i, &pv) == DS_ERROR)
		    return DS_ERROR;
		if (Vertex_GetData(pv, (graph_data_t*)&pn) == DS_ERROR)
		    return DS_ERROR;
		if (Node_GetID(pn, &id) == DS_ERROR)
		    return DS_ERROR;
		sprintf(buf, "%ld, ", id);
		strcat(cmd, buf);
		if (Node_IsSN(pn) == DS_TRUE)
		    sprintf(buf, "0, ");
		else if (Node_IsCDL(pn) == DS_TRUE)
		    sprintf(buf, "1, ");
		else
		    sprintf(buf, "2, ");
		strcat(cmd, buf);
		if (Node_GetPower(pn, &power) == DS_ERROR)
		    return DS_ERROR;
		sprintf(buf, "%2.2lf, ", power);
		strcat(cmd, buf);
		if (Node_GetCoordinate(pn, &pcoor) == DS_ERROR)
		    return DS_ERROR;
		if (Coordinate_GetX(pcoor, &coor) == DS_ERROR)
		    return DS_ERROR;
		sprintf(buf, "%4.2lf, ", coor);
		strcat(cmd, buf);
		if (Coordinate_GetY(pcoor, &coor) == DS_ERROR)
		    return DS_ERROR;
		sprintf(buf, "%4.2lf, \"", coor);
		strcat(cmd, buf);
		if (Vertex_GetEdges(pv, &edges) == DS_ERROR)
		    return DS_ERROR;
		for (j = 0; j < SingleLinkedList_Size(edges); j++) {
		    if (SingleLinkedList_GetData(edges, j, (sll_data_t*)&pe) == DS_ERROR)
			    return DS_ERROR;
			if (Edge_GetEndID(pe, &id) == DS_ERROR)
			    return DS_ERROR;
			sprintf(buf, "%ld,", id);
			strcat(cmd, buf);
		}
		sprintf(buf, "\");");
		strcat(cmd, buf);
		if (Mysql_Query(pm, cmd) == DS_ERROR)
		    return DS_ERROR;
	}
	return DS_OK;
}
