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


#include "open_mandible_parameters.h"

void case5_mandible_program_Parameters::set_params_filename(string filename)
{
    m_sPath_and_FileName = filename;
}

case5_mandible_program_Parameters::case5_mandible_program_Parameters()
{
    m_sPath_and_FileName = "input/open_mandible_program_parameters.cfg";

    submodule = 1;

    vtk = true;
    pos = true;
    txt = true;

    emodule = false;


}

case5_mandible_program_Parameters * case5_mandible_program_Parameters::m_pcase5_mandible_program_ParametersSingleton = 0;
case5_mandible_program_Parameters * case5_mandible_program_Parameters::create_case5_mandible_program_Parameters()
{
    if (m_pcase5_mandible_program_ParametersSingleton == 0)
        m_pcase5_mandible_program_ParametersSingleton = new case5_mandible_program_Parameters();
    return m_pcase5_mandible_program_ParametersSingleton;
}

case5_mandible_program_Parameters::~case5_mandible_program_Parameters()
{}

bool case5_mandible_program_Parameters::read_file()
{
    Info * info = Info::createInfo();
    info->print_info_message("reading parameters... ");

    char line[constants::no_of_line_characters];
    char name[200];

    int tmp;

    read_the_materials_names(m_sPath_and_FileName);
    read_the_prescribed_surfaces_filenames(m_sPath_and_FileName);
    read_the_prescribed_surface_directed_filenames(m_sPath_and_FileName);

    reading::get_line_that_contains_this_string_and_close_the_file(line, "Section_A_choice", m_sPath_and_FileName);
    sscanf(line, "%*s%d", &tmp);
    submodule = tmp;

    reading::get_line_that_contains_this_string_and_close_the_file(line, "Section_F_choice_POS", m_sPath_and_FileName);
    sscanf(line, "%*s%d", &tmp);
    pos = (bool) tmp;

    reading::get_line_that_contains_this_string_and_close_the_file(line, "Section_F_choice_VTK", m_sPath_and_FileName);
    sscanf(line, "%*s%d", &tmp);
    vtk = (bool) tmp;

    reading::get_line_that_contains_this_string_and_close_the_file(line, "Section_F_choice_TXT", m_sPath_and_FileName);
    sscanf(line, "%*s%d", &tmp);
    txt = (bool) tmp;

    reading::get_line_that_contains_this_string_and_close_the_file(line, "Section_E_choice_run_module", m_sPath_and_FileName);
    sscanf(line, "%*s%d", &tmp);
    emodule = (bool) tmp;

    reading::get_line_that_contains_this_string_and_close_the_file(line, "Section_E_pick_material_name", m_sPath_and_FileName);
    sscanf(line, "%*s %s", name);
    emodule_material_name = name;

    return true;
}

UINT case5_mandible_program_Parameters::read_the_materials_names(string filename)
{
    ifstream ifile;
    ifile.open(filename.c_str());

    if (reading::check_if_file_is_not_open_wo_using_cout(ifile, filename.c_str()))
        return false;

    char the_line[constants::no_of_line_characters];

    string line_str,
    line_str_start = "Section_B_list_start",
    line_str_end   = "Section_B_list_end";

    char name1[200], name2[200];

    material_name_and_type m_name_and_type;

    bool
    start_bool = false;

    for (;!ifile.eof();)
    {
        ifile.getline(the_line, constants::no_of_line_characters);
        line_str = the_line;

        if (strstr(line_str.c_str(), line_str_end.c_str()))
            return materials_names_and_types.get_size();


        if (start_bool)
        if (line_str[0] != '-')
        {
            sscanf(line_str.c_str(), "%s %s", name1, name2);
            m_name_and_type.material_name = name1;
            m_name_and_type.material_type = name2;
            materials_names_and_types.insert(m_name_and_type);
        }


        if (strstr(line_str.c_str(), line_str_start.c_str()))
            start_bool = true;
    }

    ifile.close();
    return materials_names_and_types.get_size();
}

UINT case5_mandible_program_Parameters::read_the_prescribed_surfaces_filenames(string filename)
{
    ifstream ifile;
    ifile.open(filename.c_str());

    if (reading::check_if_file_is_not_open_wo_using_cout(ifile, filename.c_str()))
        return false;

    char the_line[constants::no_of_line_characters];

    string line_str, surface_filename,
    line_str_start = "Section_C_list_start",
    line_str_end   = "Section_C_list_end";

    bool
    start_bool = false;

    name_and_orientation n_and_o;
    char name1[200], name2[200];
    double d_tmp;


    for (;!ifile.eof();)
    {
        ifile.getline(the_line, constants::no_of_line_characters);
        line_str = the_line;

        if (strstr(line_str.c_str(), line_str_end.c_str()))
            return prescribed_surfaces_filenames.get_size();

        if (start_bool)
        if (line_str[0] != '-')
        {
            sscanf(line_str.c_str(), "%s %s %lf", name1, name2, &d_tmp);
            n_and_o.name =      name2;
            n_and_o.material =  name1;
            n_and_o.orijentation = d_tmp;
            prescribed_surfaces_filenames.insert(n_and_o);
        }

        if (strstr(line_str.c_str(), line_str_start.c_str()))
            start_bool = true;
    }

    ifile.close();
    return prescribed_surfaces_filenames.get_size();
}

UINT case5_mandible_program_Parameters::read_the_prescribed_surface_directed_filenames(string filename)
{
    ifstream ifile;
    ifile.open(filename.c_str());

    if (reading::check_if_file_is_not_open_wo_using_cout(ifile, filename.c_str()))
        return false;

    char the_line[constants::no_of_line_characters];

    string line_str, surface_filename,
    line_str_start = "Section_D_list_start",
    line_str_end   = "Section_D_list_end";

    bool
    start_bool = false;

    double d_tmp;

    names_pair namespair;
    char name1[200], name2[200], mat[200];

    for (;!ifile.eof();)
    {
        ifile.getline(the_line, constants::no_of_line_characters);
        line_str = the_line;

        if (strstr(line_str.c_str(), line_str_end.c_str()))
            return names_directed.get_size();

        if (start_bool)
        if (line_str[0] != '-')
        {

            sscanf(line_str.c_str(), "%s %s %s %lf", mat, name1, name2, &d_tmp);
            namespair.material =  mat;
            namespair.name1 =  name1;
            namespair.name2 =  name2;
            namespair.scalar = d_tmp;
            names_directed.insert(namespair);
        }

        if (strstr(line_str.c_str(), line_str_start.c_str()))
            start_bool = true;
    }

    ifile.close();
    return names_directed.get_size();
}


