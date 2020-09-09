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



    /// Section D: directed bc

int bc_directed(case5_mandible_program_Parameters * program_params, Collection <Mesh_Node> & nodes_col, Collection <Geom_Element> & elements_col)
{

    Info * info = Info::createInfo();

    for (UINT i = 1; i <= program_params->names_directed.get_size(); i++)
    {

        info->print_info_message("Section D :: bc directed " + utos(i) +
        "/" + utos(program_params->names_directed.get_size()));


        Collection <Geom_Node> stl_input_nodes1, stl_input_nodes2;
        Collection <Geom_Element>  stl_input_edges1, stl_input_edges2;

        int matID = 0;

        for (UINT j = 1; j <= program_params->materials_names_and_types.get_size(); j++)
        if (program_params->names_directed[i].material == program_params->materials_names_and_types[j].material_name)
            matID = j;


        /// almost all from openR

        /// reading stl inputs
        {
            STL_Reader * stl_reader = STL_Reader::create_STL_Reader();

            if (!stl_reader->read_STL_edges(stl_input_nodes1, stl_input_edges1, "input/bc_directed/" + program_params->names_directed[i].name1))
                return 1;

            if (!stl_reader->read_STL_edges(stl_input_nodes2, stl_input_edges2, "input/bc_directed/" + program_params->names_directed[i].name2))
                return 1;
        }

        /// stl operations
        {
            stl_input_nodes1.sort_collection();
            stl_input_nodes1.no_overlapping();
            stl_input_nodes2.sort_collection();
            stl_input_nodes2.no_overlapping();
        }

        Collection <Mesh_Node> surface_nodez;


        for (UINT j = 1; j <= nodes_col.get_size(); j++)
        {
            nodes_col[j].set_flag(false);

            if (nodes_col[j].is_velocity_boundary())
            {
                surface_nodez.insert(nodes_col[j]);
                surface_nodez[surface_nodez.get_size()].set_ID(j);
            }
        }


        surface_nodez.sort_collection();

        UINT surface_to_mesh_Xmin_node = 1;

        /// stl procedure 1
        {
            UINT one = 1;

            for (UINT j = 1; j <= surface_nodez.get_size(); j++)
            if (these_nodes_are_the_same(stl_input_nodes1[one], surface_nodez[j], constants::same_node_tol))
            {
                surface_to_mesh_Xmin_node = j - 1;
                if (surface_to_mesh_Xmin_node == 0)
                    surface_to_mesh_Xmin_node = 1;
                break;
            }
        }

        Collection <UINT> input_nodez_IDs;
        Collection <UINT> input_elementz_IDs;

        Collection <Node_Candidate> candidates;


        /// stl procedure 2
        for (UINT x = 1; x <= stl_input_nodes1.get_size(); x++)
        for (UINT j = surface_to_mesh_Xmin_node; j <= surface_nodez.get_size(); j++)
        if (!surface_nodez[j].get_flag())
        if(these_nodes_are_the_same(stl_input_nodes1[x], surface_nodez[j], constants::same_node_tol))
        {
            surface_nodez[j].set_flag(true);
            input_nodez_IDs.insert(surface_nodez[j].get_ID());

            break;
        }

        input_nodez_IDs.sort_collection();
        input_nodez_IDs.no_overlapping();

        Collection <Geom_Element> surface_facetz, BC_facetz;
        tmeshing::put_surface_facetz(matID, surface_facetz, elements_col, nodes_col);

        /// new blocks from openR
        {


    Collection <Geom_Element>  inlet_or_outlet_edges;

    {
        Geom_Element edge[4];

        for (UINT q = 1; q <= surface_facetz.get_size(); q++)
        {
            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
            for (UINT j = 1; j <= input_nodez_IDs.get_size(); j++)
            {
                if (
                (surface_facetz[q].get_node(1) != input_nodez_IDs[j]) &&
                (surface_facetz[q].get_node(2) != input_nodez_IDs[j]) &&
                (surface_facetz[q].get_node(3) != input_nodez_IDs[j]) &&
                (surface_facetz[q].get_node(4) != input_nodez_IDs[j]))
                    continue;

                edge[1].set_ID(q);
                edge[1].set_node(surface_facetz[q].get_node(1), 1);
                edge[1].set_node(surface_facetz[q].get_node(2), 2);

                edge[2].set_ID(q);
                edge[2].set_node(surface_facetz[q].get_node(2), 1);
                edge[2].set_node(surface_facetz[q].get_node(3), 2);

                edge[3].set_ID(q);
                edge[3].set_node(surface_facetz[q].get_node(3), 1);
                edge[3].set_node(surface_facetz[q].get_node(4), 2);

                edge[0].set_ID(q);
                edge[0].set_node(surface_facetz[q].get_node(4), 1);
                edge[0].set_node(surface_facetz[q].get_node(1), 2);

                inlet_or_outlet_edges.insert(edge[1]);
                inlet_or_outlet_edges.insert(edge[2]);
                inlet_or_outlet_edges.insert(edge[3]);
                inlet_or_outlet_edges.insert(edge[0]);

                surface_facetz[q].set_flag(true);
            }

            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
            {
                int triple_check = 0;

                for (UINT j = 1; j <= input_nodez_IDs.get_size(); j++)
                {
                    if (surface_facetz[q].get_node(1) == input_nodez_IDs[j])
                        triple_check++;
                    if (surface_facetz[q].get_node(2) == input_nodez_IDs[j])
                        triple_check++;
                    if (surface_facetz[q].get_node(3) == input_nodez_IDs[j])
                        triple_check++;
                }

                if (triple_check == 3)
                {
                    edge[1].set_ID(q);
                    edge[1].set_node(surface_facetz[q].get_node(1), 1);
                    edge[1].set_node(surface_facetz[q].get_node(2), 2);

                    edge[2].set_ID(q);
                    edge[2].set_node(surface_facetz[q].get_node(2), 1);
                    edge[2].set_node(surface_facetz[q].get_node(3), 2);

                    edge[3].set_ID(q);
                    edge[3].set_node(surface_facetz[q].get_node(3), 1);
                    edge[3].set_node(surface_facetz[q].get_node(1), 2);

                    inlet_or_outlet_edges.insert(edge[1]);
                    inlet_or_outlet_edges.insert(edge[2]);
                    inlet_or_outlet_edges.insert(edge[3]);

                    surface_facetz[q].set_flag(true);
                }

                triple_check = 0;
            }
        }
    }

    {
        int no_of_inlet_or_outlet_edges_on_a_face_should_be_twice_no_of_faces = 0;
        Geom_Element edge[4];

        for (UINT q = 1; q <= surface_facetz.get_size(); q++)
        if (surface_facetz[q].get_flag())
        {

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
            {
                for  (int k = 1; k <= 4; k++)
                {
                    int m = k + 1;
                    if (m == 5) m = 1;

                    edge[k - 1].set_ID(q);
                    edge[k - 1].set_node(surface_facetz[q].get_node(k), 1);
                    edge[k - 1].set_node(surface_facetz[q].get_node(m), 2);
                }

                for (UINT j = 1; j <= inlet_or_outlet_edges.get_size(); j++)
                for  (int k = 0; k < 4; k++)
                if (these_edges_are_same(edge[k], inlet_or_outlet_edges[j]))
                    no_of_inlet_or_outlet_edges_on_a_face_should_be_twice_no_of_faces++;

                if (no_of_inlet_or_outlet_edges_on_a_face_should_be_twice_no_of_faces < 8)
                    surface_facetz[q].set_flag(false);
            }

            no_of_inlet_or_outlet_edges_on_a_face_should_be_twice_no_of_faces = 0;
        }
    }


    UINT no_of_faces_lets = 1;

    for (UINT q = 1; q <= surface_facetz.get_size(); q++)
    if (surface_facetz[q].get_flag())
    {
        BC_facetz.insert(surface_facetz[q]);

        if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
        {
            for (int  k = 1; k <= 4; k++)
                input_nodez_IDs.insert(surface_facetz[q].get_node(k));

            surface_facetz[q].set_node(no_of_faces_lets, 5);
        }
        if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
        {
            for (int  k = 1; k <= 3; k++)
                input_nodez_IDs.insert(surface_facetz[q].get_node(k));

            surface_facetz[q].set_node(no_of_faces_lets, 4);
        }
    }

        input_nodez_IDs.sort_collection();
        input_nodez_IDs.no_overlapping();

        }


        Collection <Node_Candidate> jaw_nodes_for_two_stl_bc, head_nodes_for_two_stl_bc;


        for (UINT m = 1; m <= input_nodez_IDs.get_size(); m++)
        {
            jaw_nodes_for_two_stl_bc.insert(nodes_col[input_nodez_IDs[m]]);
            jaw_nodes_for_two_stl_bc[jaw_nodes_for_two_stl_bc.get_size()].set_ID(input_nodez_IDs[m]);
        }

        for (UINT m = 1; m <= stl_input_nodes2.get_size(); m++)
            head_nodes_for_two_stl_bc.insert(stl_input_nodes2[m]);

        Geom_Node gravitycenter_jaw, gravitycenter_head;

        gravitycenter_jaw  = get_gravitycenter(jaw_nodes_for_two_stl_bc);
        gravitycenter_head = get_gravitycenter(stl_input_nodes2);

        Geom_Node direction_vector =
            get_vector_difference_vecA_minus_vecB(gravitycenter_head, gravitycenter_jaw);

        transform_2_1value_vector(direction_vector);

        {
            double max_dist = -1.;

            for (UINT m = 1; m <= jaw_nodes_for_two_stl_bc.get_size(); m++)
            {
                jaw_nodes_for_two_stl_bc[m].distance_to_gravitycenter =
                get_lenght(gravitycenter_jaw, jaw_nodes_for_two_stl_bc[m]);

                if (jaw_nodes_for_two_stl_bc[m].distance_to_gravitycenter > max_dist)
                    max_dist = jaw_nodes_for_two_stl_bc[m].distance_to_gravitycenter;
            }

            for (UINT m = 1; m <= jaw_nodes_for_two_stl_bc.get_size(); m++)
                jaw_nodes_for_two_stl_bc[m].distance_to_gravitycenter =
                jaw_nodes_for_two_stl_bc[m].distance_to_gravitycenter / max_dist;

            max_dist = -1.;

            for (UINT m = 1; m <= head_nodes_for_two_stl_bc.get_size(); m++)
            {
                head_nodes_for_two_stl_bc[m].distance_to_gravitycenter =
                get_lenght(gravitycenter_head, head_nodes_for_two_stl_bc[m]);

                if (head_nodes_for_two_stl_bc[m].distance_to_gravitycenter > max_dist)
                    max_dist = head_nodes_for_two_stl_bc[m].distance_to_gravitycenter;
            }

            for (UINT m = 1; m <= head_nodes_for_two_stl_bc.get_size(); m++)
                head_nodes_for_two_stl_bc[m].distance_to_gravitycenter =
                head_nodes_for_two_stl_bc[m].distance_to_gravitycenter / max_dist;

        }

        SuperCollection <Node_Candidate> jaw_candidates_zones, head_candidates_zones;
        Collection <Node_Candidate> one_zone;

        UINT minimal_node_number_per_zone = 10, number_of_zones;

        if (head_nodes_for_two_stl_bc.get_size() <= jaw_nodes_for_two_stl_bc.get_size())
        if (head_nodes_for_two_stl_bc.get_size() < minimal_node_number_per_zone)
            minimal_node_number_per_zone = head_nodes_for_two_stl_bc.get_size();

        if (jaw_nodes_for_two_stl_bc.get_size() <= head_nodes_for_two_stl_bc.get_size())
        if (jaw_nodes_for_two_stl_bc.get_size() < minimal_node_number_per_zone)
            minimal_node_number_per_zone = jaw_nodes_for_two_stl_bc.get_size();

        if (head_nodes_for_two_stl_bc.get_size() <= jaw_nodes_for_two_stl_bc.get_size())
            number_of_zones =
            head_nodes_for_two_stl_bc.get_size() / minimal_node_number_per_zone;
        else
            number_of_zones =
            jaw_nodes_for_two_stl_bc.get_size() / minimal_node_number_per_zone;

        for (UINT zone = 1; zone <= number_of_zones; zone++)
        {
            jaw_candidates_zones.insert(one_zone);
            head_candidates_zones.insert(one_zone);
        }

        double one_zone_range = 1. / number_of_zones;

        for (UINT headn = 1; headn <= head_nodes_for_two_stl_bc.get_size(); headn++)
        {
            UINT number_of_ranges_per_node = ceil(
            head_nodes_for_two_stl_bc[headn].distance_to_gravitycenter / one_zone_range);

            head_candidates_zones[number_of_ranges_per_node].
            insert(head_nodes_for_two_stl_bc[headn]);
        }

        for (UINT jawn = 1; jawn <= jaw_nodes_for_two_stl_bc.get_size(); jawn++)
        {
            UINT number_of_ranges_per_node = ceil(
            jaw_nodes_for_two_stl_bc[jawn].distance_to_gravitycenter / one_zone_range);

            jaw_candidates_zones[number_of_ranges_per_node].
            insert(jaw_nodes_for_two_stl_bc[jawn]);
        }

        /// shto je manji apsolutni kosinus to je vecji ugao

        for (UINT no_of_zn = 1; no_of_zn <= jaw_candidates_zones.get_size(); no_of_zn++)
        for (UINT jaw_nd_of_this_zn = 1;
                jaw_nd_of_this_zn <= jaw_candidates_zones[no_of_zn].get_size();
                jaw_nd_of_this_zn++)
        {

            Collection <angle_referencR> angle_ref_collection;
            angle_referencR ang_ref;


            for (UINT head_nd_of_this_zn = 1;
            head_nd_of_this_zn <= head_candidates_zones[no_of_zn].get_size();
            head_nd_of_this_zn++)
            {
                Geom_Node current_direction = get_vector_difference_vecA_minus_vecB(
                head_candidates_zones[no_of_zn][head_nd_of_this_zn],
                jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn]);

                ang_ref.abs_angle_cosine =
                fabs(get_cos_of_angle_4_vectors(direction_vector, current_direction));
                ang_ref.ID = head_nd_of_this_zn;

                angle_ref_collection.insert(ang_ref);
            }

            angle_ref_collection.sort_collection();

            Geom_Node vec1, vec2;

           /// resulting BC direction of jaw node is average from two best matching head nodes
            if (angle_ref_collection.get_size() > 1)
            {
                vec1 =
                    get_vector_difference_vecA_minus_vecB(
                    head_candidates_zones[no_of_zn][angle_ref_collection[angle_ref_collection.get_size()].ID],
                    jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn]);

                vec2 =
                    get_vector_difference_vecA_minus_vecB(
                    head_candidates_zones[no_of_zn][angle_ref_collection[angle_ref_collection.get_size() - 1].ID],
                    jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn]);

                jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn].BC_direction =
                get_average_vector_of_vecA_and_vecB(vec1, vec2);

                transform_2_1value_vector(jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn].BC_direction);

                jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn].BC_target =
                get_gravitycenter(
                head_candidates_zones[no_of_zn][angle_ref_collection[angle_ref_collection.get_size()].ID],
                head_candidates_zones[no_of_zn][angle_ref_collection[angle_ref_collection.get_size() - 1].ID]);
            }

            if (angle_ref_collection.get_size() == 1)
            {
                vec1 =
                    get_vector_difference_vecA_minus_vecB(
                    head_candidates_zones[no_of_zn][angle_ref_collection[angle_ref_collection.get_size()].ID],
                    jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn]);


                jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn].BC_direction = vec1;

                transform_2_1value_vector(jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn].BC_direction);

                jaw_candidates_zones[no_of_zn][jaw_nd_of_this_zn].BC_target =
                head_candidates_zones[no_of_zn][angle_ref_collection[angle_ref_collection.get_size()].ID];
            }
        }

        Collection <Init_Node> BC_pair_nodes;
        Collection <Geom_Node> BC_pair_nodes_targets;
        Init_Node BC_pair_node;

        for (UINT t = 1; t <= jaw_candidates_zones.get_size(); t++)
        for (UINT a = 1; a <= jaw_candidates_zones[t].get_size(); a++)
        if (get_the_vector_intensity(jaw_candidates_zones[t][a].BC_direction) < 1.5)
        if (get_the_vector_intensity(jaw_candidates_zones[t][a].BC_direction) > 0.5)
        {
            BC_pair_node = jaw_candidates_zones[t][a];
            BC_pair_node.initial_velocity = jaw_candidates_zones[t][a].BC_direction;

            BC_pair_node.initial_velocity = get_product_scalar_vector(program_params->names_directed[i].scalar, BC_pair_node.initial_velocity);

            BC_pair_nodes.insert(BC_pair_node);
            BC_pair_nodes_targets.insert(jaw_candidates_zones[t][a].BC_target);
        }

        SuperCollection <Geom_Node> bc_lines;
        Collection <Geom_Node> bc_line;

        for (UINT lin = 1; lin <= BC_pair_nodes.get_size(); lin++)
        {
            bc_line.insert(BC_pair_nodes[lin]);
            bc_line.insert(BC_pair_nodes_targets[lin]);
            bc_lines.insert(bc_line);
        }

        if (program_params->pos)
        {
            POS_Printer * pos_printer = POS_Printer::create_POS_Printer();

            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
                (BC_pair_nodes, "output/bc_directed/BC_directed_" + utos(i) + "_nodes_and_vectors_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-tet.pos");

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                pos_printer->print_initial_nodes_velocity_vectors_to_pos_file
                (BC_pair_nodes, "output/bc_directed/BC_directed_" + utos(i) + "_nodes_and_vectors_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-hex.pos");

            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                pos_printer->print_triangles_from_triangle_elements_as_lines_into_pos(true, BC_facetz, nodes_col, "output/bc_directed/BC_directed_" + utos(i) + "_faces_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-tet.pos");

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                pos_printer->print_quads_from_quadrilateral_elements_as_lines_into_pos(true, BC_facetz, nodes_col, "output/bc_directed/BC_directed_" + utos(i) + "_faces_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-hex.pos");

            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                pos_printer->print_lines_with_length_absolute_to_pos_file(bc_lines,
                "output/bc_directed/BC_directed_" + utos(i) + "_lines_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-tet.pos");

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                pos_printer->print_lines_with_length_absolute_to_pos_file(bc_lines,
                "output/bc_directed/BC_directed_" + utos(i) + "_lines_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-hex.pos");
        }



        if (program_params->vtk)
        {
            VTK_Printer * vtk_printer = VTK_Printer::create_VTK_Printer();


            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                vtk_printer->print_triangles_to_vtk_file(BC_facetz, nodes_col, "output/bc_directed/BC_directed_" + utos(i) + "_faces_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-tet.vtk");

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                vtk_printer->print_quads_to_vtk_file(BC_facetz, nodes_col, "output/bc_directed/BC_directed_" + utos(i) + "_faces_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-hex.vtk");

            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                vtk_printer->print_lines_to_vtk_file(bc_lines,
                "output/bc_directed/BC_directed_" + utos(i) + "_lines_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-tet.vtk");

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                vtk_printer->print_lines_to_vtk_file(bc_lines,
                "output/bc_directed/BC_directed_" + utos(i) + "_lines_"   + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-hex.vtk");
        }


        if (program_params->txt)
        {
            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                print_plain_text_BC_directed_nodes_file(
                program_params->names_directed[i].name1,
                program_params->names_directed[i].name2,
                program_params->names_directed[i].material,
                BC_pair_nodes,
                BC_pair_nodes_targets,
                "output/bc_directed/BC_directed_" + utos(i) + "_nodes_and_vectors_"  + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-tet.txt");

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                print_plain_text_BC_directed_nodes_file(
                program_params->names_directed[i].name1,
                program_params->names_directed[i].name2,
                program_params->names_directed[i].material,
                BC_pair_nodes,
                BC_pair_nodes_targets,
                "output/bc_directed/BC_directed_" + utos(i) + "_nodes_and_vectors_"  + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-hex.txt");

            if (elements_col[1].how_many_nodes_per_element() == constants::TETRA)
                print_plain_text_BC_directed_triangles_file(
                program_params->names_directed[i].name1,
                program_params->names_directed[i].name2,
                program_params->names_directed[i].material,
                BC_facetz,
                "output/bc_directed/BC_directed_" + utos(i) + "_faces_" + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-tet.txt");

            if (elements_col[1].how_many_nodes_per_element() == constants::BRICK)
                print_plain_text_BC_directed_quads_file(
                program_params->names_directed[i].name1,
                program_params->names_directed[i].name2,
                program_params->names_directed[i].material,
                BC_facetz,
                "output/bc_directed/BC_directed_" + utos(i) + "_faces_" + program_params->names_directed[i].name1 + "_" + program_params->names_directed[i].name2 + "-hex.txt");
        }

    }

    return 0;
}
