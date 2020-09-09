/*
    This file is part of Open-Mandible software package.
*
    Open-Mandible software package is free software:
*   you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation,
*   either version 3 of the License, or (at your option) any later version.

*   Open-Mandible software package is distributed in the hope that
    it will be useful, but WITHOUT ANY WARRANTY; without even the
*   implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.
*
    You should have received a copy of the GNU General Public License
*   along with Open-Mandible software package.
    If not, see <http://www.gnu.org/licenses/>.
*
    Open-Mandible is created on the top of the dfemtoolz software.
*   github.com/dmilashinovic/dfemtoolz

*   For any additional info contact authors of this software:

*   Danko Milasinovic
    dmilashinovic@gmail.com
*   dmilashinovic@kg.ac.rs

*   Arso Vukicevic
    arso_kg@yahoo.com
*   sites.google.com/site/arsomvukicevic/home

*/

/*
 * File:   open_mandible_functions.h
 *
 *
 *
 *
 *
 */



#ifndef CASE5_EXAMPLE_FUNCTIONS_H
#define	CASE5_EXAMPLE_FUNCTIONS_H


#include "../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dConstants.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Node.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Plane.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Element.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Functions.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_TFunctions.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMesh/dMesh_Node.h"

#include "open_mandible_classes.h"

bool set_lengths_array(double len, UINT ID, length_referencR & referenceR);

Geom_Node five_vector_average(Geom_Node & vec1, Geom_Node & vec2, Geom_Node & vec3, Geom_Node & vec4, Geom_Node & vec5);

void print_software_title(string title, UINT year, UINT build);


#endif	/* CASE5_EXAMPLE_FUNCTIONS_H */

