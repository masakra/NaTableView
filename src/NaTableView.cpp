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

#include "NaTableView.h"

#include "NaHeaderView.h"

#include <QtGui>
#include <QDebug>

NaTableView::NaTableView( QWidget * parent )
	: QAbstractItemView( parent ),
	  height_row_cached( -1 ),
	  height_group_row_cached( -1 ),
	  m_offset( 0 )
{
	createHeader();
}

QRect
NaTableView::visualRect( const QModelIndex & index ) const
{
	return QRect(); // TODO
}

void
NaTableView::scrollTo( const QModelIndex & index, ScrollHint hint )
{
}

QModelIndex
NaTableView::indexAt( const QPoint & point ) const
{
	return QModelIndex(); // TODO
}

QModelIndex
NaTableView::moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers )
{
	return QModelIndex(); // TODO
}

int
NaTableView::horizontalOffset() const
{
	return 0;	// TODO
}

int
NaTableView::verticalOffset() const
{
	return 0; // TODO
}

bool
NaTableView::isIndexHidden( const QModelIndex & index ) const
{
	return false;
}

void
NaTableView::setSelection( const QRect & rect, QItemSelectionModel::SelectionFlags flags )
{
}

QRegion
NaTableView::visualRegionForSelection( const QItemSelection & selection ) const
{
	return QRegion();
}

void
NaTableView::createHeader()
{
	header = new NaHeaderView( this );

	connect( header, SIGNAL( groupsChanged( int, int ) ), SLOT( groupsChanged( int, int ) ) );
	// TODO добавить сигналы
}

void
NaTableView::setModel( QAbstractItemModel * model )
{
	header->setModel( model );

	QAbstractItemView::setModel( model );
}

void
NaTableView::updateGeometries()
{
	const int top = header->heightHint();

	setViewportMargins( 0, top, 0, 0 );

	// header

	const QRect vg = viewport()->geometry();

	header->setGeometry( vg.left(), vg.top() - top, vg.width(), top );
}

void
NaTableView::groupsChanged( int old_count, int new_count )	// slot
{
	if ( ! old_count || ! new_count )
		updateGeometries();

	if ( new_count )
		buildRootGroup();
	else
		rootGroup.clear();

	viewport()->update();
}

void
NaTableView::buildRootGroup()
{
	rootGroup.clear();

	QVector< int > logicals = header->groupsLogicals();

	for ( register int r = 0; r < model()->rowCount(); ++r ) {

		Group * group = & rootGroup;

		// проход по дереву групп к группе которая должна содержать строку
		for ( register int l = 0; l < logicals.size(); ++l ) {

			const GroupKey key = model()->index( r, logicals.at( l ) ).data( Qt::DisplayRole );

			// ( *group )[ key ] - ищет или создаёт группу с ключём key
			group = & ( *group )[ key ];
		}

		group->addRowLogical( r );
	}
}

void
NaTableView::paintEvent( QPaintEvent * e )
{
	QPainter painter( viewport() );


	int firstVisualColumn = header->cVisualIndexAt( e->rect().left() ),
	    lastVisualColumn = header->cVisualIndexAt( e->rect().right() );

	if ( firstVisualColumn == -1 )
		firstVisualColumn = 0;

	if ( lastVisualColumn == -1 )
		lastVisualColumn = header->columnsCount() - 1;


	if ( rootGroup.isEmpty() ) {
		int firstVisualRow = visualRowIndexAt( 0 ),
			lastVisualRow = visualRowIndexAt( viewport()->height() );

		if ( firstVisualRow == -1 )
			firstVisualRow = 0;

		if ( lastVisualRow == -1 )
			lastVisualRow = model()->rowCount() - 1;

		int hoff = header->cSectionViewportPosition( firstVisualColumn ),
			voff = rowViewportPosition( firstVisualRow ),
			colw;

		QRect cell( hoff, voff, 0, heightRow() );

		for ( int r = firstVisualRow; r <= lastVisualRow; ++r ) {
			for ( int c = firstVisualColumn; c <= lastVisualColumn; ++c ) {
				colw = header->columns()[ c ].size;
				cell.setWidth( colw );

				drawCell( painter, cell, model()->index( r, header->columns()[ c ].logical ) );

				cell.setX( cell.x() + colw );
			}
			cell.setX( hoff );
			cell.moveTop( cell.y() + heightRow() );
		}

	} else {	// draw groups

		GroupPointer firstVisualGroup = groupAt( e->rect().top() ),
					 lastVisualGroup = groupAt( e->rect().bottom() );

		//qDebug() << firstVisualGroup;

		if ( lastVisualGroup.isEmpty() )
			lastVisualGroup << rootGroup.lastGroupKey();

		//qDebug() << firstVisualGroup[ 0 ] << lastVisualGroup[ 0 ];

		int hoff = rootGroup.groupPosition( heightRow(), heightGroupRow(), firstVisualGroup );

		drawGroup( painter, firstVisualGroup, hoff, firstVisualColumn, lastVisualColumn );

		// нарисовать остальные корневые группы до lastVisualGroup

		Groups::const_iterator i = rootGroup.find( firstVisualGroup.at( 0 ) ),
							   bl = ++( rootGroup.find( lastVisualGroup.at( 0 ) ) );

		hoff += i->height( heightGroupRow(), heightRow() );
		++i;

		while ( i != bl && i != rootGroup.constEnd() ) {

			drawGroup( painter, GroupPointer() << i.key(), hoff, firstVisualColumn, lastVisualColumn );

			hoff += i->height( heightGroupRow(), heightRow() );

			++i;
		}
	}
}

void
NaTableView::drawGroup( QPainter & painter, const GroupPointer & gPtr, int pos,
		int firstVisualColumn, int lastVisualColumn )
{
	const QColor color = header->groups()[ gPtr.size() - 1 ].color;

	const int logical = header->groups()[ gPtr.size() - 1 ].logical;

	QRect rect( 0, pos, viewport()->width(), heightGroupRow() - 1 );

	painter.save();

	/*
	\\ строка заголовка группы
	*/

	QLinearGradient gradient( 0, pos, 0, pos + heightGroupRow() );
	gradient.setColorAt( 0, color );
	gradient.setColorAt( 1, color.darker( 300 ) );

	painter.setPen( QPen( color.darker( 400 ) ) );
	painter.setBrush( QBrush( gradient ) );

	painter.drawRect( rect );

	/*
	\\ текст
	*/
	QRect rectText( rect );
	rectText.setLeft( rectText.left() + 3 );
	rectText.setRight( rectText.right() - 1 );

	const QString text = model()->headerData( logical, Qt::Horizontal, Qt::DisplayRole ).toString() +
		" = " + gPtr.last().toString();

	painter.setPen( QPen( Qt::white ) );
	painter.drawText( rectText, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, text );

	/*
	\\ вложенные группы или строки
	*/

	const Group * group = rootGroup.group( gPtr );

	if ( group == 0 ) {
		painter.restore();
		return ;
	}

	pos += heightGroupRow();

	if ( ! group->isEmpty() ) {			// имеются вложенные группы

		Group::const_iterator i = group->constBegin();

		while ( i != group->constEnd() ) {

			drawGroup( painter, GroupPointer( gPtr ) << i.key(), pos,
					firstVisualColumn, lastVisualColumn );

			pos += i->height( heightGroupRow(), heightRow() );

			++i;
		}

	} else {						// должны быть вложенные строки
									// сделать проверку TODO
		int hoff = header->cSectionViewportPosition( firstVisualColumn ),
			colw;

		QRect cell( hoff, pos, 0, heightRow() );

		pos += group->rows().size() * heightRow();

		for ( int r = 0; r < group->rows().size(); ++r ) {

			const int row_logical = group->rows().at( r );

			for ( int c = firstVisualColumn; c <= lastVisualColumn; ++c ) {
				colw = header->columns()[ c ].size;
				cell.setWidth( colw );

				drawCell( painter, cell, model()->index( row_logical, header->columns()[ c ].logical ) );

				cell.setX( cell.x() + colw );
			}
			cell.setX( hoff );
			cell.moveTop( cell.y() + heightRow() );
		}
	}

	painter.restore();
}

int
NaTableView::visualRowIndexAt( int pos ) const
{
	if ( heightRow() == 0 )
		return -1;

	const int row = ( pos + m_offset ) / heightRow();

	if ( row != 0 && row > model()->rowCount() - 1 )
		return -1;

	return row;
}

int
NaTableView::heightRow() const
{
	if ( height_row_cached != -1 )
		return height_row_cached;

	QFontMetrics fm( font() );

	return height_row_cached = qRound( fm.height() * 1.5 );
}

int
NaTableView::heightGroupRow() const
{
	if ( height_group_row_cached != -1 )
		return height_group_row_cached;

	QFontMetrics fm( font() );

	return height_group_row_cached = qRound( fm.height() * 1.8 );
}

int
NaTableView::rowViewportPosition( int visual ) const
{
	if ( rootGroup.isEmpty() ) {
		return visual * heightRow() - m_offset;
	} else {
	}
}

void
NaTableView::drawCell( QPainter & painter, const QRect & cell, const QModelIndex & modelIndex ) const
{
	painter.save();

	painter.setPen( QPen( COLOR_GRID ) );

	painter.drawLine( cell.bottomLeft(), cell.bottomRight() );
	painter.drawLine( cell.topRight(), cell.bottomRight() );
	/*
	\\ text
	*/
	QRect rectText( cell );
	rectText.setLeft( rectText.left() + 3 );
	rectText.setRight( rectText.right() - 1 );

	painter.setPen( QPen( COLOR_TEXT ) );
	painter.drawText( rectText, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine,
			modelIndex.data( Qt::DisplayRole ).toString() );

	painter.restore();
}

GroupPointer
NaTableView::groupAt( int pos ) const
{
	GroupPointer gPtr;

	rootGroup.groupAt( pos, heightGroupRow(), heightRow(), gPtr );

	return gPtr;
}

