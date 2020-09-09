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

#include "merg_multimaterial.h"


int merg_multimaterial(case5_mandible_program_Parameters * program_params, SuperCollection <Mesh_Node> & nodes_supercol, SuperCollection <Geom_Element> & elements_supercol,
Collection <Geom_Element> & surface_facets_col, Collection <Geom_Element> & contact_faces_col, SuperCollection <Geom_Element> & all_contacts_supercol)
{

    Info * info = Info::createInfo();

    Collection <Geom_Element> initial_faces;
    Collection <UINT> all_contacts_checksums;

    for (UINT i = 2; i <= program_params->materials_names_and_types.get_size(); i++)
    {

        info->print_info_message("Section B :: dfemtoolz_multimaterial " + utos(i-1) +
        "/" + utos(program_params->materials_names_and_types.get_size()-1));

        dmultimaterialF(nodes_supercol[1], nodes_supercol[i], elements_supercol[1], elements_supercol[i], initial_faces, all_contacts_supercol, all_contacts_checksums);

        for (UINT i = 1; i <= nodes_supercol[1].get_size(); i++)
        {
            nodes_supercol[1][i].clear_all_neighbours();
            nodes_supercol[1][i].set_flag(false);
        }
    }

    if (elements_supercol[1][1].how_many_nodes_per_element() == constants::TETRA)
        meshing::clear_all_neighbours_lists(nodes_supercol[1]);

    return 0;
}
