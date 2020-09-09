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

#include "case5-mandible_plain_text_print_functions.h"

bool print_plain_text_nodes_file(Collection <Mesh_Node> & node_col, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << "nodes of the model (" <<  node_col.get_size() << " total)" << endl;
    output_file << "ID x y z is_on_the_surface_of_the_material" << endl;

    char buffR[200];

    int is_on_the_surface;

    for (UINT i = 1; i <= node_col.get_size(); i++)
    {
        is_on_the_surface = (int)((node_col[i].get_boundary() & constants::IS_BOUNDARY_V) == constants::IS_BOUNDARY_V);

        sprintf(buffR, "%10d %8.5E %8.5E %8.5E %5d",
        i,
        node_col[i].get_coordinate_X(),
        node_col[i].get_coordinate_Y(),
        node_col[i].get_coordinate_Z(),
        is_on_the_surface);

        output_file << buffR << endl;
    }

    output_file.close();
    return true;
}

bool print_plain_text_elems_datapoints_file(Collection <Data_Point> & datapoints_elem_col, string material_name, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << "orthrotopy vectors for " << material_name << " material of the model model (" <<  datapoints_elem_col.get_size() << " total)" << endl;
    output_file << "elementID osteonX osteonY osteonZ normalX normalY normalZ binormalX binormalY binormalZ zoneID" << endl;

    char buffR[400];

    for (UINT i = 1; i <= datapoints_elem_col.get_size(); i++)
    {
        sprintf(buffR, "%10d", datapoints_elem_col[i].get_ID());
        output_file << buffR;

        for (int j = 1; j <= 3; j++)
        {
            sprintf(buffR, " %8.5E", datapoints_elem_col[i].osteon.get_coordinate(j));
            output_file << buffR;
        }

        for (int j = 1; j <= 3; j++)
        {
            sprintf(buffR, " %8.5E", datapoints_elem_col[i].normal.get_coordinate(j));
            output_file << buffR;
        }

        for (int j = 1; j <= 3; j++)
        {
            sprintf(buffR, " %8.5E", datapoints_elem_col[i].binormal.get_coordinate(j));
            output_file << buffR;
        }

        sprintf(buffR, "%10d", datapoints_elem_col[i].get_IDzone());
        output_file << buffR;

        output_file << endl;
    }

    output_file.close();
    return true;
}

bool print_plain_text_elems_file(Collection <Mesh_Node> & node_col, Collection <Geom_Element> & elem_col, Collection <material_name_and_type> & materials_names, string & special_material_name, bool emodule_off_or_on, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << "elements of the model (" <<  elem_col.get_size() << " total)" << endl;
    output_file << "materials of the model (materialID  materialName):" << endl;

    for (UINT i = 1; i <= materials_names.get_size(); i++)
        output_file << i << " " << materials_names[i].material_name << " " << materials_names[i].material_type << endl;

    if (emodule_off_or_on)
    {
        output_file << "material " << special_material_name << " is divided into zones," << endl;
        output_file << "please see additional details in output/osteon_normal_binormal/" << endl;
    }

    output_file << "ID node1ID node2ID node3ID .... is_on_the_surface materialID" << endl;

    int surface_element;
    char buffR[11];

    for (UINT i = 1; i <= elem_col.get_size(); i++)
    {
        sprintf(buffR, "%10d", i);
        output_file << buffR;

        for (int j = 1; j <= elem_col[i].how_many_nodes_per_element(); j++)
        {
            sprintf(buffR, "%10d", elem_col[i].get_node(j));

            if ((node_col[elem_col[i].get_node(j)].get_boundary() & constants::IS_BOUNDARY_V) == constants::IS_BOUNDARY_V)
                surface_element = 1;

            output_file << buffR;
        }

        sprintf(buffR, "%5d", surface_element);
        output_file << buffR;

        sprintf(buffR, "%5d", elem_col[i].get_material_ID());
        output_file << buffR << endl;

        surface_element = 0;
    }

    output_file.close();
    return true;
}


bool print_plain_text_BC_contacts_file(UINT contactID, int elem_type, Collection <material_name_and_type> & materials_names, Collection <Geom_Element> & contact_face_col, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());

    output_file << "this is contact ID " << contactID << " between these materials of the model:" << endl;

    if (elem_type == constants::TETRA)
    {
        output_file << materials_names[contact_face_col[1].get_node(5)].material_name << " is name of material1_ID (this is reference material)" << endl;
        output_file << materials_names[contact_face_col[1].get_node(6)].material_name << " is name of material2_ID" << endl;
    }

    if (elem_type == constants::BRICK)
    {
        output_file << materials_names[contact_face_col[1].get_node(6)].material_name << " is material1_ID name (this is reference material)" << endl;
        output_file << materials_names[contact_face_col[1].get_node(7)].material_name << " is material2_ID name" << endl;
    }

    output_file << "(" <<  contact_face_col.get_size() << " is number of contact faces)" << endl;

    output_file << "ID elementID nodesID.. material1_ID material2_ID" << endl;

    char buffR[200];

    for (UINT i = 1; i <= contact_face_col.get_size(); i++)
    {
        if (elem_type == constants::TETRA)
            sprintf(buffR, "%10d%10d%10d%10d%10d%5d%5d",
            i,
            contact_face_col[i].get_ID(),
            contact_face_col[i].get_node(1),
            contact_face_col[i].get_node(2),
            contact_face_col[i].get_node(3),
            contact_face_col[i].get_node(5),
            contact_face_col[i].get_node(6));
//            (int) contact_face_col[i].get_boundary());
/// software could print side 0 or 1 of the same contact
/// for the sake of simplicity this feature is not printed but fully supported

        if (elem_type == constants::BRICK)
            sprintf(buffR, "%10d%10d%10d%10d%10d%10d%5d%5d",
            i,
            contact_face_col[i].get_ID(),
            contact_face_col[i].get_node(1),
            contact_face_col[i].get_node(2),
            contact_face_col[i].get_node(3),
            contact_face_col[i].get_node(4),
            contact_face_col[i].get_node(6),
            contact_face_col[i].get_node(7));
//            (int) contact_face_col[i].get_boundary());
/// software could print side 0 or 1 of the same contact
/// for the sake of simplicity this feature is not printed but fully supported

        output_file << buffR << endl;
    }

    output_file.close();
    return true;
}

bool print_plain_text_BC_faces_file(int face_type, string bc_faces_filename, string bc_material_filename, Geom_Node & normal_vector, Collection <Geom_Element> & faces_col, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << bc_faces_filename << " is the file used to create this BC" << endl;
    output_file << bc_material_filename << " is the material name used for this BC" << endl;
    output_file << "(" <<  faces_col.get_size() << " total number of faces)" << endl;

    output_file << "avg normal vector is: " <<
    normal_vector.get_coordinate_X() << " " <<
    normal_vector.get_coordinate_Y() << " " <<
    normal_vector.get_coordinate_Z() << " " <<
    endl;
    output_file << "ID elementID node1ID node2ID ... " << endl;

    char buffR[200];

    for (UINT i = 1; i <= faces_col.get_size(); i++)
    {
        if (face_type == constants::TRIANGLE)
            sprintf(buffR, "%10d%10d%10d%10d%10d",
            i,
            faces_col[i].get_ID(),
            faces_col[i].get_node(1),
            faces_col[i].get_node(2),
            faces_col[i].get_node(3));

        if (face_type == constants::QUAD)
            sprintf(buffR, "%10d%10d%10d%10d%10d%10d",
            i,
            faces_col[i].get_ID(),
            faces_col[i].get_node(1),
            faces_col[i].get_node(2),
            faces_col[i].get_node(3),
            faces_col[i].get_node(4));

        output_file << buffR << endl;
    }

    output_file.close();
    return true;
}


bool print_plain_text_BC_nodes_file(string bc_faces_filename, string bc_material_filename, Geom_Node & normal_vector, Collection <Init_Node> & nodes_col, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << bc_faces_filename << " is the file used to create this BC" << endl;
    output_file << bc_material_filename << " is the material name used for this BC" << endl;
    output_file << "(" <<  nodes_col.get_size() << " total number of nodes)" << endl;

    output_file << "avg normal vector is: " <<
    normal_vector.get_coordinate_X() << " " <<
    normal_vector.get_coordinate_Y() << " " <<
    normal_vector.get_coordinate_Z() << " " <<
    endl;
    output_file << "ID X Y Z normX normY normZ" << endl;

    char buffR[200];

    for (UINT i = 1; i <= nodes_col.get_size(); i++)
    {
        sprintf(buffR, "%10d  %8.5E  %8.5E  %8.5E  %8.5E  %8.5E  %8.5E ",
        nodes_col[i].get_ID(),
        nodes_col[i].get_coordinate_X(),
        nodes_col[i].get_coordinate_Y(),
        nodes_col[i].get_coordinate_Z(),
        nodes_col[i].initial_velocity.get_coordinate_X(),
        nodes_col[i].initial_velocity.get_coordinate_Y(),
        nodes_col[i].initial_velocity.get_coordinate_Z()
        );

        output_file << buffR << endl;
    }

    output_file.close();
    return true;
}

bool print_plain_text_BC_directed_nodes_file(string bc_faces_filenameJaw, string bc_faces_filenameHead, string bc_material_filename, Collection <Init_Node> & nodes_col, Collection <Geom_Node> & targets_col, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << bc_faces_filenameJaw <<  " and " << bc_faces_filenameHead << " are the files used to create this BC" << endl;
    output_file << bc_material_filename << " is the material name used for this BC" << endl;
    output_file << "(" <<  nodes_col.get_size() << " total number of nodes)" << endl;

   {
        Collection <Geom_Node> average_vecs;
        Geom_Node average_vec;

        for (UINT i = 1; i <= nodes_col.get_size(); i++)
            average_vecs.insert(nodes_col[i].initial_velocity);

        average_vec = get_gravitycenter(average_vecs);
        output_file << "average direction vector is: (" <<
        average_vec.get_coordinate_X() << ", " <<
        average_vec.get_coordinate_Y() << ", " <<
        average_vec.get_coordinate_Z() << ")" <<
        endl;
   }

   {
        Geom_Node average_vec;

        average_vec = get_gravitycenter(targets_col);
        output_file << "average origin is: (" <<
        average_vec.get_coordinate_X() << ", " <<
        average_vec.get_coordinate_Y() << ", " <<
        average_vec.get_coordinate_Z() << ")" <<
        endl;
   }

    output_file << "ID insertionX insertionY insertionZ directionX directionY directionZ originX originY originZ" << endl;

    char buffR[200];

    for (UINT i = 1; i <= nodes_col.get_size(); i++)
    {
        sprintf(buffR, "%10d  %8.5E  %8.5E  %8.5E  %8.5E  %8.5E  %8.5E  %8.5E  %8.5E  %8.5E ",
        nodes_col[i].get_ID(),
        nodes_col[i].get_coordinate_X(),
        nodes_col[i].get_coordinate_Y(),
        nodes_col[i].get_coordinate_Z(),
        nodes_col[i].initial_velocity.get_coordinate_X(),
        nodes_col[i].initial_velocity.get_coordinate_Y(),
        nodes_col[i].initial_velocity.get_coordinate_Z(),
        targets_col[i].get_coordinate_X(),
        targets_col[i].get_coordinate_Y(),
        targets_col[i].get_coordinate_Z());

        output_file << buffR << endl;
    }

    output_file.close();
    return true;
}

bool print_plain_text_BC_directed_triangles_file(string bc_faces_filenameJaw, string bc_faces_filenameHead, string bc_material_filename, Collection <Geom_Element> & faces_col, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << bc_faces_filenameJaw <<  " and " << bc_faces_filenameHead << " are the files used to create this BC" << endl;
    output_file << bc_material_filename << " is the material name used for this BC" << endl;
    output_file << "(" <<  faces_col.get_size() << " total number of faces)" << endl;
    output_file << "ID elementID node1ID node2ID ... " << endl;

    char buffR[200];

    for (UINT i = 1; i <= faces_col.get_size(); i++)
    {
        sprintf(buffR, "%10d%10d%10d%10d%10d",
        i,
        faces_col[i].get_ID(),
        faces_col[i].get_node(1),
        faces_col[i].get_node(2),
        faces_col[i].get_node(3));

        output_file << buffR << endl;
    }

    output_file.close();
    return true;
}

bool print_plain_text_BC_directed_quads_file(string bc_faces_filenameJaw, string bc_faces_filenameHead, string bc_material_filename, Collection <Geom_Element> & faces_col, string path_and_fileName)
{
    Info * info = Info::createInfo();
    info->print_info_message("printing file " + path_and_fileName);

    ofstream output_file;
    output_file.open(path_and_fileName.c_str());
    output_file << bc_faces_filenameJaw <<  " and " << bc_faces_filenameHead << " are the files used to create this BC" << endl;
    output_file << bc_material_filename << " is the material name used for this BC" << endl;
    output_file << "(" <<  faces_col.get_size() << " total number of faces)" << endl;
    output_file << "ID elementID node1ID node2ID ... " << endl;

    char buffR[200];

    for (UINT i = 1; i <= faces_col.get_size(); i++)
    {
        sprintf(buffR, "%10d%10d%10d%10d%10d%10d",
        i,
        faces_col[i].get_ID(),
        faces_col[i].get_node(1),
        faces_col[i].get_node(2),
        faces_col[i].get_node(3),
        faces_col[i].get_node(4));

        output_file << buffR << endl;
    }

    output_file.close();
    return true;
}
