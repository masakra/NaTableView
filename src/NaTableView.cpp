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
	: QAbstractItemView( parent )
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
	m_header = new NaHeaderView( this );

	// TODO добавить сигналы
}

void
NaTableView::setModel( QAbstractItemModel * model )
{
	m_header->setModel( model );

	QAbstractItemView::setModel( model );
}

void
NaTableView::updateGeometries()
{
	const int top = m_header->heightHint();

	setViewportMargins( 0, top, 0, 0 );

	// header

	const QRect vg = viewport()->geometry();

	m_header->setGeometry( vg.left(), vg.top() - top, vg.width(), top );
}

