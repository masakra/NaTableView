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

 /** \class NaHeaderView
  *
  * \brief 
  */

#ifndef NAHEADERVIEW_H
#define NAHEADERVIEW_H

#define GROUP_AREA_MARGIN 10
#define GROUP_AREA_SPACING 5

#include <QAbstractItemView>
#include "Section.h"
#include "Sections.h"

class QLabel;

class NaHeaderView : public QAbstractItemView
{
	Q_OBJECT

	private:
		enum State {
			NoState,
			MoveSection,
			ResizeSection
		} state;

		void initializeSections();

		Sections s_columns,
				 s_groups;

		int m_offset,			///<
			m_gr_offset;		///<

		void clear();

		void ungroup();

		QVariant modelData( int logical, int role ) const;

		int heightColumns() const;

		mutable int heightColumns_cached;

		int heightGroups() const;

		mutable int heightGroups_cached;

		int heightSections( const Sections & sections ) const;

		QBrush snow_png;
		QPixmap wind_png;

		int gVisualIndexAt( int pos ) const;
		//int cVisualIndexAt( int pos ) const;	moved to puclic:

		int gSectionViewportPosition( int visual ) const;
		//int cSectionViewportPosition( int visual ) const;	 moved to public:

		int sectionHandleAt( const QPoint & pos ) const;

		int logicalIndexAt( const QPoint & pos ) const;

		int m_section,
			m_target,
			m_pressed,
			m_originalSize,
			m_resizedSectionViewportPosition;	// avoid cSectionViewportPosition multiple calls

		QPoint m_firstPos,
			   m_lastPos;

		void setupSectionIndicator( int logical, const QPoint & pos );

		void updateSectionIndicator( int logical, const QPoint & pos );

		void updateTargetMarker( const QPoint & pos );

		QLabel * sectionIndicator,
			   * targetMarker;

		QPoint sectionIndicatorOffset;

		void resizeSection( int visual, int size );

		bool targetIsGroupArea;

		bool overGroupArea( const QPoint & pos ) const;

		void moveSection();

		bool sectionInGroups( int logical ) const;

	protected:
		// ниже, 9 обязательных функции требующих реализации
		virtual QRect visualRect( const QModelIndex & index ) const;
		virtual void scrollTo( const QModelIndex & index, ScrollHint hint = EnsureVisible );
		virtual QModelIndex indexAt( const QPoint & point ) const;
		virtual QModelIndex moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );
		virtual int horizontalOffset() const;
		virtual int verticalOffset() const;
		virtual bool isIndexHidden( const QModelIndex & index ) const;
		virtual void setSelection( const QRect & rect, QItemSelectionModel::SelectionFlags flags );
		virtual QRegion visualRegionForSelection( const QItemSelection & selection ) const;

		// --------------------------------------------------
		//virtual bool event( QEvent * e );
		virtual void mousePressEvent( QMouseEvent * e );
		virtual void paintEvent( QPaintEvent * e );
		virtual void mouseMoveEvent( QMouseEvent * e );
		virtual void mouseReleaseEvent( QMouseEvent * e );

	protected Q_SLOTS:
		void sectionsInserted( const QModelIndex & parent, int logicalFirst, int logicalLast );
		void sectionsRemoved( const QModelIndex & parent, int logicalFirst, int logicalLast );

	public:
		NaHeaderView( QWidget * parent );

		virtual void setModel( QAbstractItemModel * model );

		int heightHint();

		int cVisualIndexAt( int pos ) const;

		int cSectionViewportPosition( int visual ) const;

		bool groupped() const;

		const Sections & columns() const;

		const Sections & groups() const;

		QVector< int > groupsLogicals() const;

		void setOffset( int new_offset );

	public Q_SLOTS:
		void headerDataChanged( Qt::Orientation, int logicalFirst, int logicalLast );

	Q_SIGNALS:
		void groupsChanged( int old_count, int new_count );
		void sectionResized( const QRect & rect );
		void sectionMoved( const QRect & rect );	// в области столбцов
};

#endif


