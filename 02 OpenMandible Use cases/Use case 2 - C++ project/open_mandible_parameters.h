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

#ifndef _CASE5_MANDIBLE_PROGRAM_PARAMETERS_H
#define	_CASE5_MANDIBLE_PROGRAM_PARAMETERS_H

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dfemtoolz_dMyLib.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dConstants.h"
#include "../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dCollection.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dInfo.h"

#include "../dfemtoolz-master/dfemtoolz_dMyLib/dRead/dReading.h"
#include "open_mandible_classes.h"

class case5_mandible_program_Parameters
{
private:
    static case5_mandible_program_Parameters * m_pcase5_mandible_program_ParametersSingleton;
    string m_sPath_and_FileName;

protected:
    case5_mandible_program_Parameters();

public:

    Collection <material_name_and_type> materials_names_and_types;
    Collection <name_and_orientation> prescribed_surfaces_filenames;
    Collection <name_and_orientation> prescribed_vectors_filenames;
    Collection <names_pair> names_directed;

    void set_params_filename(string filename);

    int submodule;

    bool vtk;
    bool pos;
    bool txt;

    bool emodule;
    string emodule_material_name;

    bool read_file();

    UINT read_the_materials_names(string filename);
    UINT read_the_prescribed_surfaces_filenames(string filename);
    UINT read_the_prescribed_surface_directed_filenames(string filename);

    static case5_mandible_program_Parameters * create_case5_mandible_program_Parameters();
    ~case5_mandible_program_Parameters();
};



#endif	/* _CASE5_MANDIBLE_PROGRAM_PARAMETERS_H */


