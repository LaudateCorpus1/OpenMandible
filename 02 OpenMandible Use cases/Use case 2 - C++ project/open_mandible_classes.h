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
 * File:   open_mandible_classes.h
 *
 *
 *
 *
 *
 */



#ifndef CASE5_EXAMPLE_CLASSES_H
#define	CASE5_EXAMPLE_CLASSES_H


#include "../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dConstants.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Node.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Plane.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Element.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Functions.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_TFunctions.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMesh/dMesh_Node.h"


class length_referencR
{
public:
    UINT datapointID[6];
    double lengths[6];
};

class angle_referencR
{
public:
    UINT ID;
    double abs_angle_cosine;

angle_referencR & operator =(const angle_referencR & otherangle_referencR);
bool operator       ==(const angle_referencR & otherangle_referencR);
bool operator        <(const angle_referencR & a) const;

angle_referencR();
~angle_referencR();
};

class Node_Candidate : public Mesh_Node
{
public:
    double distance_to_gravitycenter;
    Geom_Node BC_direction;
    Geom_Node BC_target;

Node_Candidate & operator =(const Node_Candidate & otherNode_Candidate);
bool operator       ==(const Node_Candidate & otherNode_Candidate);
bool operator        <(const Node_Candidate & a) const;

Node_Candidate();
Node_Candidate(const Node_Candidate & node);
Node_Candidate(const Mesh_Node & node);
Node_Candidate(const Geom_Node & node);
~Node_Candidate();
};


class name_and_orientation
{
public:
    string material;
    string name;
    double orijentation;
};

class names_pair
{
public:
    string material;
    string name1;
    string name2;
    double scalar;
};

class material_name_and_type
{
public:
    string material_name;
    string material_type;
};


#endif	/* CASE5_EXAMPLE_CLASSES_H */

