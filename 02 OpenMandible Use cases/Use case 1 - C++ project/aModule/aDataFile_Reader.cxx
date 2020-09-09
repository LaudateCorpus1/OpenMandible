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

#include "aDataFile_Reader.h"


Data_File_Reader::Data_File_Reader()
{
    m_sPath_and_FileName = constants::input_directory + "faces.fal";
}

Data_File_Reader * Data_File_Reader::m_pData_File_ReaderSingleton = 0;

Data_File_Reader * Data_File_Reader::create_Data_File_Reader()
{
    if (m_pData_File_ReaderSingleton == 0)
        m_pData_File_ReaderSingleton = new Data_File_Reader();
    return m_pData_File_ReaderSingleton;
}

Data_File_Reader::~Data_File_Reader()
{
    delete m_pData_File_ReaderSingleton;
    m_pData_File_ReaderSingleton = 0;
}

void Data_File_Reader::read_data
(Collection <Data_Point> & datapoints_c, ifstream & data_file)
{
    char line[constants::no_of_line_characters];

    UINT point_ID, zoneID;
    double point_coordinates[4], osteon[4], normal[4], binormal[4];
    Data_Point datapoint;

    string line_str,
    line_str_end = "DataList_STOP";

    for (;!data_file.eof();)
    {
        data_file.getline(line, constants::no_of_line_characters);
        line_str = line;

        if (strstr(line_str.c_str(), line_str_end.c_str()))
            break;

        sscanf(line, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d",
        &point_ID,
        &point_coordinates[1], &point_coordinates[2], &point_coordinates[3],
        &osteon[1], &osteon[2], &osteon[3],
        &normal[1], &normal[2], &normal[3],
        &binormal[1], &binormal[2], &binormal[3],
        &zoneID);

        datapoint.set_ID(point_ID);
        datapoint.set_IDzone(zoneID);

        for (int j = 1; j <= 3; j++)
        {
            datapoint.set_coordinate(point_coordinates[j],j);
            datapoint.osteon.set_coordinate(osteon[j],j);
            datapoint.normal.set_coordinate(normal[j],j);
            datapoint.binormal.set_coordinate(binormal[j],j);
        }

        datapoints_c.insert(datapoint);
    }
}

bool Data_File_Reader::read_file
(Collection <Data_Point> & datapoints_col, string path_and_fileName)
{
    ifstream input_datafile;

    if (path_and_fileName.length() > constants::minimum_characters_in_fileName)
        m_sPath_and_FileName = path_and_fileName;

    input_datafile.open(m_sPath_and_FileName.c_str());

    if (reading::check_if_file_is_not_open(input_datafile, m_sPath_and_FileName))
        return false;

    char line[constants::no_of_line_characters];

    string line_str,
    line_str_start = "DataList_BEGIN";


    for (;!input_datafile.eof();)
    {
        input_datafile.getline(line, constants::no_of_line_characters);
        line_str = line;

        if (strstr(line_str.c_str(), line_str_start.c_str()))
            break;
    }

    this->read_data(datapoints_col, input_datafile);

    input_datafile.close();
    return true;
}

