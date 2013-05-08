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

#include "Group.h"

#include <QAbstractItemModel>

#include <QDebug>

uint qHash( const QVariant & var )
{
	return qHash( var.toString() );
}

Group::Group()
	: m_parent( 0 ), m_logical( -1 )
{
}

Group::Group( int logical, int row, Group * parent )
	: m_parent( parent ), m_logical( logical )
{
	rows << row;
}

void
Group::operator<<( int i )
{
	rows << i;
}

void
Group::buildGroupsForColumns( QVector< int > logicals, const QAbstractItemModel * model )
{
	QVector< int > inners = logicals.mid( 1 );

	for ( register int r = 0; r < model->rowCount(); ++r ) {
		const QVariant key = model->index( r, logicals.first() ).data( Qt::DisplayRole );

		if ( contains( key ) ) {
			( *this )[ key ] << r;
		} else {
			Group group( logicals.first(), r, this );

			if ( ! inners.isEmpty() )
				buildGroupsForColumns( inners, model );

			insert( key, group );
		}
	}
}

int
Group::height( int heightGroup, int heightRow ) const
{
	if ( isEmpty() ) {
		return heightRow * rows.count() + heightGroup;

	} else {
		int h = 0;

		Groups::const_iterator i = constBegin();

		while ( i != constEnd() ) {

			h += i.value().height( heightGroup, heightRow );

			++i;
		}

		return h;
	}
}

void
Group::clear()
{
	Groups::clear();
	rows.clear();
}

void
Group::groupAt( int pos, int heightGroup, int heightRow, GroupPointer & gPtr ) const
{
	int next_group_pos = 0;

	Groups::const_iterator i = constBegin();

	while( i != constEnd() ) {
		next_group_pos += i.value().height( heightGroup, heightRow );

		if ( pos < next_group_pos ) {
			gPtr << i.key();
			i.value().groupAt( pos, heightGroup, heightRow, gPtr );
			return;
		}

		++i;
	}
}

int
Group::logicalForGroup( GroupPointer gPtr, int deep ) const
{
	///////// CONTINUE
}

