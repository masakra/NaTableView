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
	rootGroup.buildGroupsForColumns( header->groupsLogicals(), model() );
}

void
NaTableView::paintEvent( QPaintEvent * e )
{
	QPainter painter( viewport() );

	if ( rootGroup.isEmpty() ) {
		int firstVisualColumn = header->cVisualIndexAt( e->rect().left() ),
		    lastVisualColumn = header->cVisualIndexAt( e->rect().right() );

		if ( firstVisualColumn == -1 )
			firstVisualColumn = 0;

		if ( lastVisualColumn == -1 )
			lastVisualColumn = header->columnsCount() - 1;

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
				colw = header->section( c ).size;
				cell.setWidth( colw );

				drawCell( painter, cell, model()->index( r, header->section( c ).logical ) );

				cell.setX( cell.x() + colw );
			}
			cell.setX( hoff );
			cell.moveTop( cell.y() + heightRow() );
		}

	} else {	// draw groups

		GroupPointer firstVisualGroup = groupAt( e->rect().top() ),
					 lastVisualGroup = groupAt( e->rect().bottom() );

		if ( lastVisualGroup.isEmpty() )
			lastVisualGroup << rootGroup.lastGroupKey();

		qDebug() << firstVisualGroup[ 0 ] << lastVisualGroup[ 0 ];

		int hoff = rootGroup.groupPosition( 20, 25, firstVisualGroup );

		drawGroup( painter, firstVisualGroup, hoff );
	}
}

void
NaTableView::drawGroup( QPainter & painter, const GroupPointer & gPtr, int pos )
{
	const QColor color = header->colorGroup( gPtr.size() - 1 );

	qDebug() << gPtr.size() << color;

	QRect rect( 0, pos, viewport()->width(), 25 );

	painter.save();

	QLinearGradient gradient( 0, pos, 0, pos + 25 );
	gradient.setColorAt( 0, color );
	gradient.setColorAt( 1, color.darker( 300 ) );

	painter.setPen( QPen( color.darker( 400 ) ) );
	painter.setBrush( QBrush( gradient ) );

	painter.drawRect( rect );


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

	return height_row_cached = qRound( fm.height() * 1.3 );
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

	rootGroup.groupAt( pos, 25, 20, gPtr );

	return gPtr;
}

