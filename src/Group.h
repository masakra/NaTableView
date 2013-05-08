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

class Group;

typedef QVector< QVariant > GroupPointer;
typedef QHash< QVariant, Group > Groups;	// вложенные группы

class QAbstractItemModel;

class Group : public Groups // наследует вложенные группы
{
	private:
		Group * m_parent;

		QVector< int > rows;				///< вложенные строки

		int m_logical;

	public:
		Group();
		Group( int logical, int row, Group * parent );

		void buildGroupsForColumns( QVector< int > logicals, const QAbstractItemModel * model );

		int height( int heightGroup, int heightRow ) const;

		void clear();

		void operator<<( int i );

		void groupAt( int pos, int heightGroup, int heightRow, GroupPointer & gPtr ) const;

		int logicalForGroup( GroupPointer gPtr, int deep = -1 ) const;
};

#endif


