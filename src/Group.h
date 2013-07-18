/***************************************************************************
 *   Copyright (C) 2013 by Sergey N Chursanov                              *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   jabber: masakra@jabber.ru                                             *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

 /** \class Group
  *
  * \brief 
  */

#ifndef GROUP_H
#define GROUP_H

#include <QVariant>
#include <QVector>

#include "GroupPointer.h"

class Group;
typedef QMap< GroupKey, Group > Groups;	// вложенные группы

class Group : public Groups				// наследует вложенные группы
{
	private:
		QVector< int > m_rows;				///< вложенные строки

		mutable int height_cached;

	public:
		Group();

		int height( int heightGroup, int heightRow ) const;

		void clear();

		void groupAt( int pos, int heightGroup, int heightRow, GroupPointer & gPtr,
				int group_pos = 0 ) const;

		int groupPosition( int heightGroup, int heightRow, const GroupPointer & gPtr ) const;

		GroupKey lastGroupKey() const;

		void addRow( int row_logical );

		Group & operator<<( int row_logical );

		const Group * group( const GroupPointer & gPtr );

		const QVector< int > & rows() const;
};

#endif


