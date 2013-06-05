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

 /** \class NaTableView
  *
  * \brief 
  */

#ifndef NATABLEVIEW_H
#define NATABLEVIEW_H

#define COLOR_GRID Qt::gray
#define COLOR_TEXT Qt::black

#include <QAbstractItemView>
#include "Group.h"

class NaHeaderView;

class NaTableView : public QAbstractItemView
{
	Q_OBJECT

	private:
		void createHeader();

		NaHeaderView * header;

		//bool groupped() const;

		int visualRowIndexAt( int pos ) const;

		GroupPointer groupAt( int pos ) const;

		int heightRow() const;

		mutable int height_row_cached;

		int heightGroupRow() const;

		mutable int height_group_row_cached;

		int m_offset;

		int rowViewportPosition( int visual ) const;

		void drawCell( QPainter & painter, const QRect & cell, const QModelIndex & modelIndex ) const;

		Group rootGroup;	///< корневая группа

		void buildRootGroup();

		void drawGroup( QPainter & painter, const GroupPointer & gPtr, int pos,
				int firstVisualColumn, int lastVisualColumn );

	private Q_SLOTS:
		void groupsChanged( int old_count, int new_count );

	protected:
		// ниже, 9 обязательных функции требующих реализации
		virtual QRect visualRect( const QModelIndex & index ) const;
		virtual void scrollTo( const QModelIndex & index, ScrollHint hint = EnsureVisible );
		virtual QModelIndex indexAt( const QPoint & point ) const;
		virtual QModelIndex moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );
		virtual int horizontalOffset() const;
		virtual int verticalOffset() const;
		virtual bool isIndexHidden( const QModelIndex & index ) const;

		/** \fn void setSelection( const QRect & rect, QItemSelectionModel::SelectionFlags flags )
		//
		// Корректно работает только при
		// setSelectionMode( QAbstractItemView::SingleSelection );
		// setSelectionBehavior( QAbstractItemView::SelectRows );
		// устанавливается в конструкторе. Если требуется другое поведение виджета при
		// выделении, перепиши.
		*/
		virtual void setSelection( const QRect & rect, QItemSelectionModel::SelectionFlags flags );
		virtual QRegion visualRegionForSelection( const QItemSelection & selection ) const;

		virtual void paintEvent( QPaintEvent * e );

		// ------------------------------
		virtual void updateGeometries();

	public:
		NaTableView( QWidget * parent = 0 );

		virtual void setModel( QAbstractItemModel * model );
};

#endif


