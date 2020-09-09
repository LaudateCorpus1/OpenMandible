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
 * File:   orthrotopy.h
 *
 *
 */

#ifndef _ORTHROTOPY_H
#define	_ORTHROTOPY_H


#include <math.h>

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dfemtoolz_dMyLib.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dSuperCollection.h"
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

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dInit/dInit_Node.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMisc/dTimer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMisc/dMisc_Functionz.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dPrinting.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dPOS_Printer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dVTK_Printer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dSLI_Printer.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dInfo.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dMisc/dDirectory_Miner.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dRead/dSLI_Reader.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dRead/dSTL_Reader.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dRead/dFAL_Reader.h"

#include "../dfemtoolz-master/dfemtoolz_remesh/dremesh_parameters.h"
#include "../dfemtoolz-master/dfemtoolz_remesh/dremeshF.h"

#include "../dfemtoolz-master/dfemtoolz_openR/dopenRf.h"

#include "../dfemtoolz-master/dfemtoolz_multimaterial/dmultimaterialF.h"
#include "../dfemtoolz-master/dfemtoolz_multimaterial/dmultimaterial_functions.h"

#include "open_mandible_parameters.h"
#include "case5-mandible_plain_text_print_functions.h"
#include "open_mandible_functions.h"
#include "open_mandible_classes.h"

#include "aModule/aDataFile_Reader.h"
#include "aModule/aDataPoint.h"


int orthrotopy(case5_mandible_program_Parameters * program_params, Collection <Mesh_Node> & nodes_col, Collection <Geom_Element> & elements_col);


#endif	/* _ORTHROTOPY_H */



