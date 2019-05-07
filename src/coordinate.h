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

#ifndef GQRM_COORDINATE_H
#define GQRM_COORDINATE_H

typedef struct COORDINATE  Coordinate;
typedef Coordinate*        pt_Coordinate;

extern pt_Coordinate Coordinate_Create2D(const coordinate_t, const coordinate_t);
extern pt_Coordinate Coordinate_Create3D(const coordinate_t, const coordinate_t, const coordinate_t);
extern ds_stat Coordinate_Assign(pt_Coordinate, pt_Coordinate);
extern ds_stat Coordinate_GetX(pt_Coordinate, coordinate_t*);
extern ds_stat Coordinate_GetY(pt_Coordinate, coordinate_t*);
extern ds_stat Coordinate_GetZ(pt_Coordinate, coordinate_t*);
extern ds_stat Coordinate_SetX(pt_Coordinate, const coordinate_t);
extern ds_stat Coordinate_SetY(pt_Coordinate, const coordinate_t);
extern ds_stat Coordinate_SetZ(pt_Coordinate, const coordinate_t);
extern ds_bool Coordinate_Equal(pt_Coordinate, pt_Coordinate);
extern ds_bool Coordinate_NotEqual(pt_Coordinate, pt_Coordinate);
extern ds_bool Coordinate_XEqual(pt_Coordinate, pt_Coordinate);
extern ds_bool Coordinate_XNotEqual(pt_Coordinate, pt_Coordinate);
extern ds_bool Coordinate_YEqual(pt_Coordinate, pt_Coordinate);
extern ds_bool Coordinate_YNotEqual(pt_Coordinate, pt_Coordinate);
extern ds_bool Coordinate_ZEqual(pt_Coordinate, pt_Coordinate);
extern ds_bool Coordinate_ZNotEqual(pt_Coordinate, pt_Coordinate);
extern ds_stat Coordinate_Add(pt_Coordinate, pt_Coordinate, pt_Coordinate);
extern ds_stat Coordinate_Subtract(pt_Coordinate, pt_Coordinate, pt_Coordinate);
extern ds_stat Coordinate_OnbodyAdd(pt_Coordinate, pt_Coordinate);
extern ds_stat Coordinate_OnbodySubtract(pt_Coordinate, pt_Coordinate);
extern coordinate_t Coordinate_Distance(pt_Coordinate, pt_Coordinate);
extern ds_stat Coordinate_2DPrint(pt_Coordinate, FILE*);
extern ds_stat Coordinate_3DPrint(pt_Coordinate, FILE*);
#endif
