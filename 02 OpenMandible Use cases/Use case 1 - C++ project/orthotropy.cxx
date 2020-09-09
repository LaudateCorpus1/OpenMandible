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

#include "bc_directed.h"


int orthotropy(case5_mandible_program_Parameters * program_params, Collection <Mesh_Node> & nodes_col, Collection <Geom_Element> & elements_col)
{
    Info * info = Info::createInfo();

    POS_Printer * pos_printer = POS_Printer::create_POS_Printer();
    VTK_Printer * vtk_printer = VTK_Printer::create_VTK_Printer();


    info->print_info_message("\n\n\n     Section E :: orthotropy \n");

    int matID = 0;

    Collection <Data_Point> picked_material_elements_and_appropriate_vectors;

    for (UINT j = 1; j <= program_params->materials_names_and_types.get_size(); j++)
    if (program_params->emodule_material_name == program_params->materials_names_and_types[j].material_name)
        matID = j;

    Collection <Data_Point> datapoints;

    Data_Point element_with_vectors;

    Data_File_Reader * datafile_reader = Data_File_Reader::create_Data_File_Reader();
    datafile_reader->read_file(datapoints, "input/aOsteonsFile.txt");

    Geom_Node graviticentR;

    Collection <length_referencR> lengths_and_datapointIDs_per_picked_material_elements;

    int mat_ID_max = 0;

    for (UINT i = 1; i <= elements_col.get_size(); i++)
    if (elements_col[i].get_material_ID() > matID)
        mat_ID_max = elements_col[i].get_material_ID();

    length_referencR lengths_and_datapointIDs_per_picked_material_element;

    for (UINT i = 1; i <= elements_col.get_size(); i++)
    if (elements_col[i].get_material_ID() == matID)
    {
        for (int l = 1; l <= 5; l++)
            lengths_and_datapointIDs_per_picked_material_element.lengths[l] = 100000000000.;

        graviticentR = get_gravitycenter(elements_col[i], nodes_col);

        for (UINT j = 1; j <= datapoints.get_size(); j++)
        if (set_lengths_array(get_lenght(graviticentR, datapoints[j]),
        datapoints[j].get_ID(),
        lengths_and_datapointIDs_per_picked_material_element))
            elements_col[i].set_material_ID(
            mat_ID_max + datapoints[j].get_IDzone()); /// new material set, as well as lengths_and_datapointIDs_per_picked_material_element

        /// setting up gravitycenter for visualization

        for (int k = 1; k <= 3; k++)
            element_with_vectors.set_coordinate(graviticentR.get_coordinate(k), k);

        /// setting up ID
        element_with_vectors.set_ID(i);

        /// setting up data concerning vectors for elements of picked material 5 nearest
        /// datapoint to elem grav center vec average

        element_with_vectors.osteon =
        five_vector_average(
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[1]].osteon,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[2]].osteon,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[3]].osteon,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[4]].osteon,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[5]].osteon);
        transform_2_1value_vector(element_with_vectors.osteon);

        element_with_vectors.normal =
        five_vector_average(
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[1]].normal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[2]].normal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[3]].normal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[4]].normal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[5]].normal);
        transform_2_1value_vector(element_with_vectors.normal);

        element_with_vectors.binormal =
        five_vector_average(
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[1]].binormal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[2]].binormal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[3]].binormal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[4]].binormal,
        datapoints[lengths_and_datapointIDs_per_picked_material_element.datapointID[5]].binormal);
        transform_2_1value_vector(element_with_vectors.binormal);


        UINT zone = elements_col[i].get_material_ID() - mat_ID_max;
        element_with_vectors.set_IDzone(zone);

        picked_material_elements_and_appropriate_vectors.insert(element_with_vectors);
    }

    info->print_info_message("\n\nprinting element vectors\n");

    if (program_params->txt)
    {
        if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
            print_plain_text_elems_datapoints_file(
            picked_material_elements_and_appropriate_vectors,
            program_params->emodule_material_name,
            "output/osteon_normal_binormal/plain-text-elems-osteon_normal_binormal-tet.txt");

        if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
            print_plain_text_elems_datapoints_file(
            picked_material_elements_and_appropriate_vectors,
            program_params->emodule_material_name,
            "output/osteon_normal_binormal/plain-text-elems-osteon_normal_binormal-hex.txt");
    }

    if (program_params->pos)
    {
        Collection <Init_Node> osteon_nodes, normal_nodes, binormal_nodes;
        Init_Node init_node;

        for (UINT i = 1; i <= picked_material_elements_and_appropriate_vectors.get_size(); i++)
        {
            for (int j = 1; j <= 3; j++)
                init_node.set_coordinate(
                picked_material_elements_and_appropriate_vectors[i].
                get_coordinate(j), j);

            init_node.initial_velocity = picked_material_elements_and_appropriate_vectors[i].osteon;
            transform_2_1value_vector(init_node.initial_velocity);
            osteon_nodes.insert(init_node);

            init_node.initial_velocity = picked_material_elements_and_appropriate_vectors[i].normal;
            transform_2_1value_vector(init_node.initial_velocity);
            normal_nodes.insert(init_node);

            init_node.initial_velocity = picked_material_elements_and_appropriate_vectors[i].binormal;
            transform_2_1value_vector(init_node.initial_velocity);
            binormal_nodes.insert(init_node);
        }

        if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
        {
            pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
            (osteon_nodes, "output/osteon_normal_binormal/" + program_params->emodule_material_name + "_osteon-hex.pos");

            pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
            (normal_nodes, "output/osteon_normal_binormal/" + program_params->emodule_material_name + "_normal-hex.pos");

            pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
            (binormal_nodes, "output/osteon_normal_binormal/" + program_params->emodule_material_name + "_binormal-hex.pos");
        }

        if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
        {
            pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
            (osteon_nodes, "output/osteon_normal_binormal/" + program_params->emodule_material_name + "_osteon-tet.pos");

            pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
            (normal_nodes, "output/osteon_normal_binormal/" + program_params->emodule_material_name + "_normal-tet.pos");

            pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
            (binormal_nodes, "output/osteon_normal_binormal/" + program_params->emodule_material_name + "_binormal-tet.pos");
        }

    }


    if (program_params->pos)
    {
        if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
            pos_printer->print_elems_with_material_to_pos_file
            (elements_col, constants::POS_BRICK, nodes_col, "output/osteon_normal_binormal/elements_ort_mat-hex.pos");

        if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
            pos_printer->print_elems_with_material_to_pos_file
            (elements_col, constants::POS_TETRA, nodes_col, "output/osteon_normal_binormal/elements_ort_mat-tet.pos");
    }

    if (program_params->vtk)
    {
        if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
            vtk_printer->print_tetrahedron_elems_material_2_vtk_file(
            elements_col, nodes_col, "output/osteon_normal_binormal/elements_ort_mat-tet.vtk");

        if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
            vtk_printer->print_brick_elems_material_2_vtk_file(
            elements_col, nodes_col, "output/osteon_normal_binormal/elements_ort_mat-hex.vtk");
    }

    return mat_ID_max;
}
