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




#include "open_mandible_classes.h"

angle_referencR &angle_referencR::operator =(const angle_referencR &otherangle_referencR)
{
    this->ID = otherangle_referencR.ID;
    this->abs_angle_cosine = otherangle_referencR.abs_angle_cosine;

    return (*this );
}

bool angle_referencR::operator ==(const angle_referencR &otherangle_referencR)
{
    if (this->ID == otherangle_referencR.ID)
    if (this->abs_angle_cosine == otherangle_referencR.abs_angle_cosine)

        return true;

    return false;
}

bool angle_referencR::operator < (const angle_referencR & a) const
{
    return (this->abs_angle_cosine < a.abs_angle_cosine);
}

angle_referencR::angle_referencR()
{
    this->ID = 0;
    this->abs_angle_cosine = 0.;
}

angle_referencR::~angle_referencR()
{}

////

Node_Candidate &Node_Candidate::operator =(const Node_Candidate &otherNode_Candidate)
{
    for (int i = 1; i <= 3; i++)
        this->set_coordinate(otherNode_Candidate.get_coordinate(i), i);

    this->distance_to_gravitycenter = otherNode_Candidate.distance_to_gravitycenter;
    this->BC_direction = otherNode_Candidate.BC_direction;
    this->BC_target = otherNode_Candidate.BC_target;

    this->m_uID = otherNode_Candidate.m_uID;

    return (*this );
}

bool Node_Candidate::operator ==(const Node_Candidate &otherNode_Candidate)
{
    if (this->get_coordinate_X() == otherNode_Candidate.get_coordinate_X())
    if (this->get_coordinate_Y() == otherNode_Candidate.get_coordinate_Y())
    if (this->get_coordinate_Z() == otherNode_Candidate.get_coordinate_Z())
    if (this->distance_to_gravitycenter == otherNode_Candidate.distance_to_gravitycenter)
    if (this->BC_direction == otherNode_Candidate.BC_direction)
    if (this->m_uID == otherNode_Candidate.m_uID)
    if (this->BC_target == otherNode_Candidate.BC_target)

        return true;

    return false;
}

bool Node_Candidate::operator < (const Node_Candidate & a) const
{
    return (this->distance_to_gravitycenter < a.distance_to_gravitycenter);
}

Node_Candidate::Node_Candidate()
{
    this->distance_to_gravitycenter = -1.;
    this->m_uID = 0;
}

Node_Candidate::Node_Candidate(const Node_Candidate & node)
{
    for (int i = 1; i <= 3; i++)
        this->set_coordinate(node.get_coordinate(i), i);

    this->distance_to_gravitycenter = node.distance_to_gravitycenter;
    this->BC_direction  = node.BC_direction;
    this->m_uID = node.m_uID;
    this->BC_target = node.BC_target;
}

Node_Candidate::Node_Candidate(const Mesh_Node & node)
{
    for (int i = 1; i <= 3; i++)
        this->set_coordinate(node.get_coordinate(i), i);

    m_uID = node.get_ID();
}

Node_Candidate::Node_Candidate(const Geom_Node & node)
{
    for (int i = 1; i <= 3; i++)
        this->set_coordinate(node.get_coordinate(i), i);
}

Node_Candidate::~Node_Candidate()
{}
