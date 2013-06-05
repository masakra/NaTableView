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

/*
uint qHash( const QVariant & var )
{
	return qHash( var.toString() );
}
*/

Group::Group()
	: m_parent( 0 )
{
}

Group::Group( int row, Group * parent )
	: m_parent( parent )
{
	m_rows << row;
}

void
Group::operator<<( int i )
{
	m_rows << i;
}

void
Group::buildGroupsForColumns( QVector< int > logicals, const QAbstractItemModel * model )
{

	for ( register int r = 0; r < model->rowCount(); ++r ) {

		//gTGroups * gT

		for ( register int l = 0; l < logicals.size(); ++l ) {

		}
	}

	/*
	QVector< int > inners = logicals.mid( 1 );

	for ( register int r = 0; r < model->rowCount(); ++r ) {
		const GroupKey key = model->index( r, logicals.first() ).data( Qt::DisplayRole );

		if ( contains( key ) ) {
			( *this )[ key ] << r;
		} else {
			Group group( r, this );

			if ( ! inners.isEmpty() )
				group.buildGroupsForColumns( inners, model );

			insert( key, group );
		}
	}
	*/
}

int
Group::height( int heightGroup, int heightRow ) const
{
	if ( isEmpty() ) {
		return heightRow * m_rows.count() + heightGroup;

	} else {
		int h = 0;

		Groups::const_iterator i = constBegin();

		while ( i != constEnd() ) {

			h += i.value().height( heightGroup, heightRow );

			++i;
		}

		return h + heightGroup;
	}
}

void
Group::clear()
{
	Groups::clear();
	m_rows.clear();
}

void
Group::groupAt( int pos, int heightGroup, int heightRow, GroupPointer & gPtr,
		int group_pos ) const
{
	if ( pos < group_pos )
		return;

	int next_group_pos = group_pos;

	Groups::const_iterator i = constBegin();

	while( i != constEnd() ) {

		//qDebug() << i.key();

		next_group_pos += i.value().height( heightGroup, heightRow );

		//qDebug() << "group_pos" << group_pos << "next_group_pos" << next_group_pos;

		if ( pos < next_group_pos ) {
			gPtr << i.key();
			//i.value().groupAt( pos, heightGroup, heightRow, gPtr, group_pos + heightGroup );
			return;
		}

		++i;
	}
}

int
Group::groupPosition( int heightGroup, int heightRow, const GroupPointer & gPtr ) const
{
	if ( gPtr.isEmpty() )
		return 0;

	if ( ! contains( gPtr.head() ) )
		return 0;

	int pos = 0;

	Groups::const_iterator i = constBegin();

	while ( i != constEnd() ) {

		if ( i.key() != gPtr.head() ) {
			pos += i.value().height( heightGroup, heightRow );

		} else {
			if ( gPtr.isSingle() )
				return pos;
			else
				return pos + i.value().groupPosition( heightGroup, heightRow, gPtr.shifted() );
		}

		++i;
	}

	return 0;
}

QVariant
Group::lastGroupKey() const
{
	QVariant iter;

	Groups::const_iterator i = constBegin();

	while ( i != constEnd() ) {

		iter = i.key();

		++i;
	}

	return iter;
}

void
Group::addRowLogical( int row_logical )
{
	m_rows.append( row_logical );
}

const Group *
Group::group( const GroupPointer & gPtr )
{
	Group * g = this;

	for ( int i = 0; i < gPtr.size(); ++i ) {

		if ( g->contains( gPtr.at( i ) ) )
			g = & ( *g )[ gPtr.at( i ) ];

		else
			return 0;
	}

	return g;
}

const QVector< int > &
Group::rows() const
{
	return m_rows;
}

