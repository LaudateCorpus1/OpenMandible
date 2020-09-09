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
 * File:   aData_Point.h
 *
 * This file is made to set structure to accommodate
 * input/aDataList.txt file list, that is exported from the software
 * made by Arso Vukicevic (arso_kg@yahoo.com).
 *
 */

#ifndef _ADATA_POINT_H
#define	_ADATA_POINT_H

#include "../../dfemtoolz-master/dfemtoolz_dMyLib/dGeom/dGeom_Node.h"


class Data_Point : public Geom_Node
{
protected:

    UINT m_uID;
    UINT m_uIDzone;

public:

    void set_ID(UINT & ID);
    UINT get_ID() const;

    void set_IDzone(UINT & IDzone);
    UINT get_IDzone() const;

    Geom_Node osteon;
    Geom_Node normal;
    Geom_Node binormal;

    Data_Point & operator =(const Data_Point & otherData_Point);

    Data_Point();
    Data_Point(const Data_Point & point);
    ~Data_Point();
};



#endif	/* _ADATA_POINT_H */



