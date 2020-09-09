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

#include "aDataPoint.h"

Data_Point &Data_Point::operator =(const Data_Point & otherData_Point)
{
    m_uID =         otherData_Point.m_uID;
    m_uIDzone = otherData_Point.m_uIDzone;

    for (int i = 1; i <= 3; i++)
        m_dCoordinates[i-1] = otherData_Point.get_coordinate(i);

    osteon   =   otherData_Point.osteon;
    normal   =   otherData_Point.normal;
    binormal = otherData_Point.binormal;

    return (*this );
}

UINT Data_Point::get_ID() const
{
    return m_uID;
}

void Data_Point::set_ID(UINT & ID)
{
    m_uID = ID;
}

UINT Data_Point::get_IDzone() const
{
    return m_uIDzone;
}

void Data_Point::set_IDzone(UINT & IDzone)
{
    m_uIDzone = IDzone;
}

Data_Point::Data_Point()
{
    m_uID = 0;
    m_uIDzone = 0;
}

Data_Point::Data_Point(const Data_Point & point)
{
    m_uID =         point.m_uID;
    m_uIDzone = point.m_uIDzone;

    for (int i = 1; i <= 3; i++)
        m_dCoordinates[i-1] = point.get_coordinate(i);

    osteon   =   point.osteon;
    normal   =   point.normal;
    binormal = point.binormal;
}

Data_Point::~Data_Point()
{}
