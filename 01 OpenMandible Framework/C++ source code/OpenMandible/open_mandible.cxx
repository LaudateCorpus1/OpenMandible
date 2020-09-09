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

#include "open_mandible.h"

int main()
{
    /// program start


    print_software_title("Open-Mandible", 2020, 1);


    /// info object created
    Info * info = Info::createInfo();

    /// POS and VTK printers created

    POS_Printer * pos_printer = POS_Printer::create_POS_Printer();
    VTK_Printer * vtk_printer = VTK_Printer::create_VTK_Printer();

    /// super-collections and collections for storing data created (for each material)

    SuperCollection <Mesh_Node> nodez;
    SuperCollection <Geom_Element> elements;
    Collection <Geom_Element> contact_faces, surface_faces;


    case5_mandible_program_Parameters * params = case5_mandible_program_Parameters::create_case5_mandible_program_Parameters();
    params->read_file();

    /// Section A: re-meshing the materials
    /// dfemtoolz_remesh

    {
        Collection <Mesh_Node> nodes_collection;
        Collection <Geom_Element>  elements_collection;

        /// dfemtoolz_remesh for each material
        for (UINT i = 1; i <= params->materials_names_and_types.get_size(); i++)
        {
            info->print_info_message("Section A :: dfemtoolz_remesh " + utos(i) +
            "/" + utos(params->materials_names_and_types.get_size()));

            nodez.insert(nodes_collection);
            elements.insert(elements_collection);

            dremesh_get_elemsF(
            params->submodule,
            nodez[i],
            elements[i],
            "input/materials/tetgen-files/" + params->materials_names_and_types[i].material_name + ".1.node",
            "input/materials/tetgen-files/" + params->materials_names_and_types[i].material_name + ".1.ele", "");
        }
    }

    info->m_bInfoHeader = false;

    /// Section B: merging materials
    /// dfemtoolz_multimaterial

    SuperCollection <Geom_Element> all_contacts;


    info->print_info_message("");
    info->print_info_message("");

    merg_multimaterial(params, nodez, elements, surface_faces, contact_faces, all_contacts);


    info->m_bInfoHeader = false;


    info->print_info_message("");
    info->print_info_message("");

    /// Section C: bc

    bc_openR(params, nodez[1], elements[1], surface_faces, contact_faces);



    info->print_info_message("");
    info->print_info_message("");

    /// Section D: bc directed

    bc_directed(params, nodez[1], elements[1]);



    info->print_info_message("");
    info->print_info_message("");


    /// printing

    info->print_info_message("\n\nprinting nodes and elements (materials)\n");

    if (params->txt)
    {
        if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
        {
            print_plain_text_nodes_file(nodez[1], "output/plain-text-nodes-hex.txt");
            print_plain_text_elems_file(nodez[1], elements[1], params->materials_names_and_types, params->emodule_material_name, params->emodule, "output/plain-text-elems-hex.txt");

        }

        if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
        {
            print_plain_text_nodes_file(nodez[1], "output/plain-text-nodes-tet.txt");
            print_plain_text_elems_file(nodez[1], elements[1], params->materials_names_and_types, params->emodule_material_name, params->emodule, "output/plain-text-elems-tet.txt");
        }
    }

    if (params->pos)
    {
        if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
        {
            pos_printer->print_nodes_boundary_to_pos_file
            (nodez[1], "output/nodes_boundary-hex.pos");

            pos_printer->print_nodes_using_nodeID_to_pos_file
            (nodez[1], "output/nodes_IDs-hex.pos");

            pos_printer->print_elems_with_material_to_pos_file
            (elements[1], constants::POS_BRICK, nodez[1], "output/elements_mat-hex.pos");
        }

        if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
        {
            pos_printer->print_nodes_boundary_to_pos_file
            (nodez[1], "output/nodes_boundary-tet.pos");

            pos_printer->print_nodes_using_nodeID_to_pos_file
            (nodez[1], "output/nodes_IDs-tet.pos");

            pos_printer->print_elems_with_material_to_pos_file
            (elements[1], constants::POS_TETRA, nodez[1], "output/elements_mat-tet.pos");
        }
    }

    if (params->vtk)
    {
        if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
            vtk_printer->print_tetrahedron_elems_material_2_vtk_file(
            elements[1], nodez[1], "output/elements_mat-tet.vtk");

        if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
            vtk_printer->print_brick_elems_material_2_vtk_file(
            elements[1], nodez[1], "output/elements_mat-hex.vtk");
    }


    info->print_info_message("");
    info->print_info_message("");


    /// Section E: orthotropy

    info->m_bInfoHeader = false;

    int material_ID_max = 0;

    if (params->emodule)
        material_ID_max = orthotropy(params, nodez[1], elements[1]);

    info->print_info_message("");
    info->print_info_message("");

    /// printing

    /// stl output print block
    {
        SuperCollection <Geom_Element> elements_supercol_for_stl_print;

        bool checkR;

        for (UINT i = 1; i <= elements[1].get_size(); i++)
        if (elements[1][i].get_material_ID() > material_ID_max)
        {
            checkR = false;

            for (UINT j = 1; j <= elements_supercol_for_stl_print.get_size(); j++)
            if (elements[1][i].get_material_ID() ==
            elements_supercol_for_stl_print[j][1].get_material_ID())
            {
                elements_supercol_for_stl_print[j].insert(elements[1][i]);
                checkR = true;
                break;
            }

            if (!checkR)
            {
                Collection <Geom_Element> col;
                col.insert(elements[1][i]);
                elements_supercol_for_stl_print.insert(col);
            }
        }

        STL_Printer * stl_printer = STL_Printer::create_STL_Printer();

        for (UINT i = 1; i <= elements_supercol_for_stl_print.get_size(); i++)
        {
            Collection <Geom_Element> surface_faces_collection;

            Collection <Mesh_Node> nodez_4_stl_print;
            nodez_4_stl_print = nodez[1];

            dnolimitF(nodez_4_stl_print, elements_supercol_for_stl_print[i], surface_faces_collection);

            if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
                stl_printer->print_stl_file(constants::QUAD, surface_faces_collection, nodez[1], "output/ansys_import_materials/orthotropy-zone-" + itos(i) + "-hex.stl");

            if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
                stl_printer->print_stl_file(constants::TRIANGLE, surface_faces_collection, nodez[1], "output/ansys_import_materials/orthotropy-zone-" + itos(i) + "-tet.stl");

            if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
                vtk_printer->print_triangles_to_vtk_file(surface_faces_collection, nodez[1], "output/ansys_import_materials/orthotropy-zone-" + itos(i) + "-tet.vtk");
        }
    }

    info->print_info_message("\n\ncontacts printing\n");

    {
        char buffer[10];

        for (UINT i = 1; i <= all_contacts.get_size(); i++)
        if (all_contacts[i].get_size() >= 1)
        {
            string  contact_name_pos, contact_name_vtk, contact_name_txt;

            snprintf(buffer, 10, "%d", i);

            contact_name_pos = "output/contacts/contact-";
            contact_name_pos += buffer;

            contact_name_vtk = contact_name_pos;
            contact_name_txt = contact_name_pos;

            if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
            {
                contact_name_vtk += "-hex.vtk";
                contact_name_pos += "-hex.pos";
                contact_name_txt += "-hex.txt";
            }

            if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
            {
                contact_name_vtk += "-tet.vtk";
                contact_name_pos += "-tet.pos";
                contact_name_txt += "-tet.txt";
            }

            if (params->txt)
                print_plain_text_BC_contacts_file
                (i, elements[1][1].how_many_nodes_per_element(), params->materials_names_and_types, all_contacts[i], contact_name_txt);

            if (params->pos)
            {
                if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
                    pos_printer->print_triangles_from_triangle_elements_as_lines_into_pos
                    (false, all_contacts[i], nodez[1], contact_name_pos);

                if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
                    pos_printer->print_quads_from_quadrilateral_elements_as_lines_into_pos
                    (false, all_contacts[i], nodez[1], contact_name_pos);
            }

            if (params->vtk)
            {
                if (elements[1][1].how_many_nodes_per_element() == constants::TETRA)
                    vtk_printer->print_triangles_to_vtk_file
                    (i, all_contacts[i], nodez[1], contact_name_vtk);

                if (elements[1][1].how_many_nodes_per_element() == constants::BRICK)
                    vtk_printer->print_quads_to_vtk_file
                    (i, all_contacts[i], nodez[1], contact_name_vtk);
            }
        }
    }

    return 0;
}
