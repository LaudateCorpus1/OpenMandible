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
 * File:   dData_File_Reader.h
 *
 *
 *
 *
 *
 */

#ifndef _ADATAFILE_READER_H
#define	_ADATAFILE_READER_H

#include "../../dfemtoolz-master/dfemtoolz_dMyLib/dfemtoolz_dMyLib.h"

#include "../../dfemtoolz-master/dfemtoolz_dMyLib/dBase/dCollection.h"

#include "../../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Node.h"

#include "../../dfemtoolz-master/dfemtoolz_dMyLib/dPrin/dInfo.h"

#include "../../dfemtoolz-master/dfemtoolz_dMyLib/dRead/dReading.h"

#include "aDataPoint.h"


class Data_File_Reader
{
private:
    static Data_File_Reader * m_pData_File_ReaderSingleton;

    string m_sPath_and_FileName;

    bool negative_reading_condition(char * buffer);

protected:
    Data_File_Reader();

    void read_data
    (Collection <Data_Point> & datapoints_c, ifstream & data_file);

public:

    bool read_file
    (Collection <Data_Point> & datapoints_col, string path_and_fileName);

    static Data_File_Reader * create_Data_File_Reader();
    ~Data_File_Reader();
};

#endif	/* _ADATAFILE_READER_H */
