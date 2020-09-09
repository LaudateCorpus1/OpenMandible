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
 * File:   open_mandible_parameters.h
 *
 *
 *
 *
 *
 */

#ifndef _CASE5_MANDIBLE_PLAIN_TEXT_PRINT_FUNCTIONS_H
#define	_CASE5_MANDIBLE_PLAIN_TEXT_PRINT_FUNCTIONS_H


#include "../dfemtoolz-master/dfemtoolz_dMyLib/dfemtoolz_dMyLib.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dCollection.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dConstants.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Node.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Plane.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Element.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Functions.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_TFunctions.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMesh/dMesh_Neighbours.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMesh/dTMeshing.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMesh/dMesh_Node.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dInit/dInit_Node.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMisc/dTimer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMisc/dMisc_Functionz.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dPrinting.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dPOS_Printer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dVTK_Printer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dSLI_Printer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dInfo.h"


#include "aModule/aDataPoint.h"
#include "open_mandible_parameters.h"


bool print_plain_text_nodes_file(Collection <Mesh_Node> & node_col, string path_and_fileName);
bool print_plain_text_elems_file(Collection <Mesh_Node> & node_col, Collection <Geom_Element> & elem_col, Collection <material_name_and_type> & materials_names, string & special_material_name, bool emodule_off_or_on, string path_and_fileName);
bool print_plain_text_elems_datapoints_file(Collection <Data_Point> & datapoints_elem_col, string material_name, string path_and_fileName);
bool print_plain_text_BC_contacts_file(UINT contactID, int elem_type, Collection <material_name_and_type> & materials_names, Collection <Geom_Element> & contact_face_col, string path_and_fileName);
bool print_plain_text_BC_faces_file(int face_type, string bc_faces_filename, string bc_material_filename, Geom_Node & normal_vector, Collection <Geom_Element> & faces_col, string path_and_fileName);
bool print_plain_text_BC_nodes_file(string bc_faces_filename, string bc_material_filename, Geom_Node & normal_vector, Collection <Init_Node> & nodes_col, string path_and_fileName);
bool print_plain_text_BC_directed_nodes_file(string bc_faces_filenameJaw, string bc_faces_filenameHead, string bc_material_filename, Collection <Init_Node> & nodes_col, Collection <Geom_Node> & targets_col, string path_and_fileName);
bool print_plain_text_BC_directed_triangles_file(string bc_faces_filenameJaw, string bc_faces_filenameHead, string bc_material_filename, Collection <Geom_Element> & faces_col, string path_and_fileName);
bool print_plain_text_BC_directed_quads_file(string bc_faces_filenameJaw, string bc_faces_filenameHead, string bc_material_filename, Collection <Geom_Element> & faces_col, string path_and_fileName);




#endif	/* _CASE5_MANDIBLE_PLAIN_TEXT_PRINT_FUNCTIONS_H */


