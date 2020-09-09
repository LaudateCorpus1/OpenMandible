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




#include "open_mandible_functions.h"

bool set_lengths_array(double len, UINT ID, length_referencR & referenceR)
{

    if (len < referenceR.lengths[1])
    {
        referenceR.lengths[5] = referenceR.lengths[4];
        referenceR.datapointID[5] = referenceR.datapointID[4];
        referenceR.lengths[4] = referenceR.lengths[3];
        referenceR.datapointID[4] = referenceR.datapointID[3];
        referenceR.lengths[3] = referenceR.lengths[2];
        referenceR.datapointID[3] = referenceR.datapointID[2];
        referenceR.lengths[2] = referenceR.lengths[1];
        referenceR.datapointID[2] = referenceR.datapointID[1];
        referenceR.lengths[1] = len;
        referenceR.datapointID[1] = ID;
        return true;
    }

    if (len < referenceR.lengths[2])
    {
        referenceR.lengths[5] = referenceR.lengths[4];
        referenceR.datapointID[5] = referenceR.datapointID[4];
        referenceR.lengths[4] = referenceR.lengths[3];
        referenceR.datapointID[4] = referenceR.datapointID[3];
        referenceR.lengths[3] = referenceR.lengths[2];
        referenceR.datapointID[3] = referenceR.datapointID[2];
        referenceR.lengths[2] = len;
        referenceR.datapointID[2] = ID;
        return false;
    }

    if (len < referenceR.lengths[3])
    {
        referenceR.lengths[5] = referenceR.lengths[4];
        referenceR.datapointID[5] = referenceR.datapointID[4];
        referenceR.lengths[4] = referenceR.lengths[3];
        referenceR.datapointID[4] = referenceR.datapointID[3];
        referenceR.lengths[3] = len;
        referenceR.datapointID[3] = ID;
        return false;
    }

    if (len < referenceR.lengths[4])
    {
        referenceR.lengths[5] = referenceR.lengths[4];
        referenceR.datapointID[5] = referenceR.datapointID[4];
        referenceR.lengths[4] = len;
        referenceR.datapointID[4] = ID;
        return false;
    }

    if (len < referenceR.lengths[5])
    {
        referenceR.lengths[5] = len;
        referenceR.datapointID[5] = ID;
        return false;
    }

    return false;
}

Geom_Node five_vector_average(Geom_Node & vec1, Geom_Node & vec2, Geom_Node & vec3, Geom_Node & vec4, Geom_Node & vec5)
{
    Geom_Node result;

    result = get_vector_sum_vecA_plus_vecB(vec1,   vec2);
    result = get_vector_sum_vecA_plus_vecB(result, vec3);
    result = get_vector_sum_vecA_plus_vecB(result, vec4);
    result = get_vector_sum_vecA_plus_vecB(result, vec5);

    for (int i = 1; i <= 3; i++)
        result.set_coordinate(result.get_coordinate(i)/5, i);

    return result;
}

void print_software_title(string title, UINT year, UINT build)
{

    cout << endl << endl << endl <<
    "       ..:: " << title << " ::.." << endl << "            ";

    if (title.length() >  9)
    for (UINT i = 0; i <= title.length() - 10; i++)
        cout << ' ';

    cout << "                 v" << build << endl << endl
    << "    by danko milasinovic and arso vukicevic (c) " << year << endl
    << endl << endl
    << "  this software is under GNU GPLv3 licence" << endl << endl;

}

