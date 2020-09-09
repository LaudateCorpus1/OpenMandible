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

#include "bc_openR.h"


int bc_openR(case5_mandible_program_Parameters * program_params, Collection <Mesh_Node> & nodes_col, Collection <Geom_Element> & elements_col,
Collection <Geom_Element> & surface_facets_col, Collection <Geom_Element> & contact_faces_col)
{

    Info * info = Info::createInfo();

    POS_Printer * pos_printer = POS_Printer::create_POS_Printer();
    VTK_Printer * vtk_printer = VTK_Printer::create_VTK_Printer();


    Collection <Geom_Element> initial_faces, input_faces_from_fal;

    /// dfemtoolz_openR
    {


        /// reading of the file-names needed to prescribe face loads
        for (UINT i = 1; i <= program_params->prescribed_surfaces_filenames.get_size(); i++)
        {
            info->print_info_message("Section C :: dfemtoolz_openR " + utos(i) +
            "/" + utos(program_params->prescribed_surfaces_filenames.get_size()));

            UINT matID = 0;

            Collection <Init_Node> BCnodes;
            Collection <Geom_Node> BCnode_normals;


            for (UINT j = 1; j <= program_params->materials_names_and_types.get_size(); j++)
            if (program_params->prescribed_surfaces_filenames[i].material == program_params->materials_names_and_types[j].material_name)
                matID = j;

            surface_facets_col.clear_collection();

            dopenR(
            nodes_col,
            elements_col,
            initial_faces,
            contact_faces_col,
            surface_facets_col,
            input_faces_from_fal,
            "input/bc/" + program_params->prescribed_surfaces_filenames[i].name,
            matID);

            /// bound 15 are prescribed
            /// now will be reset to 7

            Geom_Node normal_vector;

            normal_vector.set_coordinate_X(0);
            normal_vector.set_coordinate_Y(0);
            normal_vector.set_coordinate_Z(0);

            for (UINT res = 1; res <= nodes_col.get_size(); res++)
            if (nodes_col[res].get_boundary() == 15)
            {
                nodes_col[res].clear_boundary();
                nodes_col[res].set_boundary(7);

                BCnodes.insert(nodes_col[res]);
                BCnodes[BCnodes.get_size()].set_ID(res);
            }

            for (UINT no_of_n = 1; no_of_n <= nodes_col.get_size(); no_of_n++)
                BCnode_normals.insert(normal_vector);

            {
                Collection <STL_Triangle>  stl_triangles;

                {
                    STL_Reader * stl_reader = STL_Reader::create_STL_Reader();
                    if (!stl_reader->read_STL_triangles(stl_triangles, "input/bc/" + program_params->prescribed_surfaces_filenames[i].name))
                        return 1;
                }

                normal_vector = get_normal_vector_coordinates(stl_triangles);
                transform_2_1value_vector(normal_vector);
            }

            normal_vector = get_product_scalar_vector(program_params->prescribed_surfaces_filenames[i].orijentation, normal_vector);

            Collection <Geom_Element> BC;

            {

                for (UINT xxx = 1; xxx <= surface_facets_col.get_size(); xxx++)
                if (surface_facets_col[xxx].get_flag())
                {
                    BC.insert(surface_facets_col[xxx]);
                    surface_facets_col.set_flag(false);
                }

                /// normals for each BC node
                {
                    UINT node[5];
                    Geom_Node norm_vec_per_node;

                    for (UINT h = 1; h <= BC.get_size(); h++)
                    {
                        if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                        {
                            node[1] = BC[h].get_node(1);
                            node[2] = BC[h].get_node(2);
                            node[3] = BC[h].get_node(3);

                            norm_vec_per_node = get_normal_vector_coordinates(
                            nodes_col[node[1]],
                            nodes_col[node[2]],
                            nodes_col[node[3]]);

                            transform_2_1value_vector(norm_vec_per_node);
                            norm_vec_per_node = get_product_scalar_vector(program_params->prescribed_surfaces_filenames[i].orijentation, norm_vec_per_node);

                            BCnode_normals[node[1]] = norm_vec_per_node;
                            BCnode_normals[node[2]] = norm_vec_per_node;
                            BCnode_normals[node[3]] = norm_vec_per_node;
                        }

                        if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                        {
                            node[1] = BC[h].get_node(1);
                            node[2] = BC[h].get_node(2);
                            node[3] = BC[h].get_node(3);
                            node[4] = BC[h].get_node(4);

                            norm_vec_per_node = get_normal_vector_coordinates(
                            nodes_col[node[1]],
                            nodes_col[node[2]],
                            nodes_col[node[3]]);

                            transform_2_1value_vector(norm_vec_per_node);

                            BCnode_normals[node[1]] = norm_vec_per_node;
                            BCnode_normals[node[2]] = norm_vec_per_node;
                            BCnode_normals[node[3]] = norm_vec_per_node;
                            BCnode_normals[node[4]] = norm_vec_per_node;
                        }
                    }
                }

                for (UINT zzr = 1; zzr <= BCnodes.get_size(); zzr++)
                for (int ort = 1; ort <= 3; ort++)
                    BCnodes[zzr].initial_velocity.set_coordinate
                    (BCnode_normals[BCnodes[zzr].get_ID()].get_coordinate(ort), ort);

                for (UINT zzr = 1; zzr <= BCnodes.get_size(); zzr++)
                    transform_2_1value_vector(BCnodes[zzr].initial_velocity);

                info->print_info_message("\n\nprinting BC-"+ utos(i) +
                "/" + utos(program_params->prescribed_surfaces_filenames.get_size()));

                if (program_params->pos)
                {
                    if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                        pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
                        (BCnodes, "output/bc/BC_" + utos(i) + "_vectors_"  + program_params->prescribed_surfaces_filenames[i].name + "-tet.pos");

                    if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                        pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
                        (BCnodes, "output/bc/BC_" + utos(i) + "_vectors_"  + program_params->prescribed_surfaces_filenames[i].name + "-hex.pos");
                }

                if (program_params->vtk)
                {
                    if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                        vtk_printer->print_triangles_to_vtk_file(BC, nodes_col, "output/bc/BC_" + utos(i) + "_faces_"  + program_params->prescribed_surfaces_filenames[i].name + "-tet.vtk");

                    if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                        vtk_printer->print_quads_to_vtk_file(BC, nodes_col, "output/bc/BC_" + utos(i) + "_faces_"  + program_params->prescribed_surfaces_filenames[i].name + "-hex.vtk");
                }

                if (program_params->txt)
                {
                    if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                        print_plain_text_BC_nodes_file(
                        program_params->prescribed_surfaces_filenames[i].name,
                        program_params->prescribed_surfaces_filenames[i].material,
                        normal_vector,
                        BCnodes,
                        "output/bc/BC_" + utos(i) + "_nodes_"  + program_params->prescribed_surfaces_filenames[i].name + "-tet.txt");

                    if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                        print_plain_text_BC_nodes_file(
                        program_params->prescribed_surfaces_filenames[i].name,
                        program_params->prescribed_surfaces_filenames[i].material,
                        normal_vector,
                        BCnodes,
                        "output/bc/BC_" + utos(i) + "_nodes_"  + program_params->prescribed_surfaces_filenames[i].name + "-hex.txt");

                    if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                        print_plain_text_BC_faces_file(
                        constants::TRIANGLE,
                        program_params->prescribed_surfaces_filenames[i].name,
                        program_params->prescribed_surfaces_filenames[i].material,
                        normal_vector,
                        BC, "output/bc/BC_" + utos(i) + "_faces_"  + program_params->prescribed_surfaces_filenames[i].name + "-tet.txt");

                    if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                        print_plain_text_BC_faces_file(
                        constants::QUAD,
                        program_params->prescribed_surfaces_filenames[i].name,
                        program_params->prescribed_surfaces_filenames[i].material,
                        normal_vector,
                        BC, "output/bc/BC_" + utos(i) + "_faces_"  + program_params->prescribed_surfaces_filenames[i].name + "-hex.txt");
                }
            }

            if (program_params->pos)
            {
                if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                    pos_printer->print_triangles_from_triangle_elements_as_lines_into_pos(true, BC, nodes_col, "output/bc/BC_" + utos(i) + "_faces_" + program_params->prescribed_surfaces_filenames[i].name + "-tet.pos");

                if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                    pos_printer->print_quads_from_quadrilateral_elements_as_lines_into_pos(true, BC, nodes_col, "output/bc/BC_" + utos(i) + "_faces_" + program_params->prescribed_surfaces_filenames[i].name + "-hex.pos");
            }

        }

    }



    return 0;
}
