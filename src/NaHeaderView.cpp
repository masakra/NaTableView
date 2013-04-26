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

#include "NaHeaderView.h"

#include <QtGui>

NaHeaderView::NaHeaderView( QWidget * parent )
	: QAbstractItemView( parent ),
	  state( NoState ),
	  m_offset( 0 ),
	  m_gr_offset( 0 ),
	  heightColumns_cached( -1 ),
	  heightGroups_cached( -1 ),
	  snow_png( QPixmap(":/snow.png") ),
	  wind_png( ":/wind.png" ),
	  sectionIndicator( 0 ),
	  targetMarker( 0 ),
	  m_resizedSectionViewportPosition( -1 )
{
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	setFrameStyle( NoFrame );
	setFocusPolicy( Qt::NoFocus );

	viewport()->setMouseTracking( true );
	viewport()->setBackgroundRole( QPalette::Button );

	delete itemDelegate();
}

QRect
NaHeaderView::visualRect( const QModelIndex & index ) const
{
	return QRect(); // TODO
}

void
NaHeaderView::scrollTo( const QModelIndex & index, ScrollHint hint )
{
}

QModelIndex
NaHeaderView::indexAt( const QPoint & point ) const
{
	return QModelIndex(); // TODO
}

QModelIndex
NaHeaderView::moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers )
{
	return QModelIndex(); // TODO
}

int
NaHeaderView::horizontalOffset() const
{
	return 0;
}

int
NaHeaderView::verticalOffset() const
{
	return 0; // TODO
}

bool
NaHeaderView::isIndexHidden( const QModelIndex & index ) const
{
	return false; // TODO
}

void
NaHeaderView::setSelection( const QRect & rect, QItemSelectionModel::SelectionFlags flags )
{
}

QRegion
NaHeaderView::visualRegionForSelection( const QItemSelection & selection ) const
{
	return QRegion(); // TODO
}

void
NaHeaderView::setModel( QAbstractItemModel * model )
{
	if ( model == this->model() )
		return;

	// TODO disconnect from old model

	if ( model ) {
		connect( model, SIGNAL( columnsInserted( QModelIndex, int, int ) ),
				SLOT( sectionsInserted( const QModelIndex, int, int ) ) );

		connect( model, SIGNAL( columnsRemoved( QModelIndex, int, int ) ),
				SLOT( sectionsRemoved( const QModelIndex, int, int ) ) );

		connect( model, SIGNAL( headerDataChanged( Qt::Orientation, int, int ) ),
				SLOT( headerDataChanged( Qt::Orientation, int, int ) ) );
	}

	QAbstractItemView::setModel( model );

	initializeSections();
}

void
NaHeaderView::sectionsInserted( const QModelIndex & parent, int logicalFirst, int logicalLast )
{
}

void
NaHeaderView::sectionsRemoved( const QModelIndex & parent, int logicalFirst, int logicalLast )
{
}

void
NaHeaderView::headerDataChanged( Qt::Orientation, int logicalFirst, int logicalLast )
{
}

void
NaHeaderView::initializeSections()
{
	const int oldCount = columns.count() + groups.count(),
			  newCount = model()->columnCount();

	if ( newCount <= 0 )
		clear();

	else if ( newCount != oldCount ) {
		ungroup();

		const int min = qBound( 0, oldCount, newCount - 1 );

		for ( register int s = min; s <= newCount - 1; ++s )
			columns.append( s );
	}
	// STUB
	//groups.append( 10 );
}

void
NaHeaderView::clear()
{
	// TODO
}

void
NaHeaderView::ungroup()
{
	// TODO
}

QVariant
NaHeaderView::modelData( int logical, int role ) const
{
	return model()->headerData( logical, Qt::Horizontal, role );
}

int
NaHeaderView::heightColumns() const
{
	if ( heightColumns_cached > -1 )
		return heightColumns_cached;

	return heightColumns_cached = heightSections( columns );
}

int
NaHeaderView::heightGroups() const
{
	if ( heightGroups_cached > -1 )
		return heightGroups_cached;

	return heightGroups_cached = heightSections( groups ) + GROUP_AREA_MARGIN + GROUP_AREA_MARGIN;
}

int
NaHeaderView::heightSections( const Sections & sections ) const
{
	if ( sections.size() == 0 )
		return 0;

	QFontMetrics fm( font() );

	return fm.height() * 2;	// TODO возможно увеличить
}

int
NaHeaderView::heightHint()
{
	//return heightSections( columns ) + heightSections( groups ) + 2 * GROUP_AREA_MARGIN;
	return heightColumns() + heightGroups();
}

void
NaHeaderView::paintEvent( QPaintEvent * e )
{
	QPainter painter( viewport() );

	/*
	\\ group area
	*/

	//painter.fillRect( 0, 0, e->rect().right() + 1, heightGroups(), snow_png );
	painter.fillRect( 0, 0, e->rect().right() + 1, viewport()->height(), snow_png );
	const int wind_y = viewport()->height() - wind_png.height(),
			  wind_x = viewport()->width() - wind_png.width() - ( wind_y < 0 ? wind_y * 2 : 0 );
	//int wind_x = viewport()->width() - wind_png.width();
	//if ( wind_y < 0 )
		//wind_x -= wind_y * 2;
	painter.drawPixmap( wind_x, wind_y, wind_png );

	/*
	\\ ordinary sections
	*/

	painter.setPen( QPen( COLOR_BORDER ) );
	painter.drawLine( 0, heightGroups(), viewport()->width(), heightGroups() );

	for ( register int s = 0; s < columns.size(); ++s ) {
		int start = cVisualIndexAt( e->rect().left() ),
			end = cVisualIndexAt( e->rect().right() );

		start = ( start == -1 ? 0 : start );
		end = ( end == -1 ? columns.count() - 1 : end );

		for ( register int i = start; i <= end; ++i ) {
			const int pos = columns.pos( i );

			const QString text = modelData( columns[ i ].logical, Qt::DisplayRole ).toString();

			columns[ i ].draw( painter, pos, heightGroups(), heightColumns(), text );
		}
	}
}

int
NaHeaderView::cVisualIndexAt( int pos ) const
{
	int next_section_pos = m_gr_offset - m_offset;

	for ( register int i = 0; i < columns.count(); ++i ) {

		next_section_pos += columns[ i ].size;

		if ( pos < next_section_pos )
			return i;
	}

	return -1;
}

void
NaHeaderView::mouseMoveEvent( QMouseEvent * e )
{
	const int x = e->x(),
		      y = e->y();

	if ( x < 0 || y < 0 )
		return;

	switch ( state ) {
		case ResizeSection: {
			const int delta = x - m_firstPos;
			resizeSection( m_section, qMax( m_originalSize + delta, MIN_SECTION_SIZE ) );
		}
		case NoState: {

			const bool hasCursor = testAttribute( Qt::WA_SetCursor );

			// at group area
			if ( y <= heightGroups() ) {
				if ( hasCursor )
					unsetCursor();
				break;
			}

			// otherwise at columns area
			const int handle = sectionHandleAt( x );

			if ( handle != -1 ) {
				if ( ! hasCursor )
					setCursor( Qt::SplitHCursor );

			} else if ( hasCursor )
				unsetCursor();

			return;
		}

		default:
			break;
	}

}

int
NaHeaderView::sectionHandleAt( int pos ) const
{
	const int visual = cVisualIndexAt( pos );

	if ( visual == -1 )
		return -1;

	const int sectionWidth = columns[ visual ].size,
			  sectionPos = cSectionViewportPosition( visual );

	if ( pos >= ( sectionWidth + sectionPos - HANDLE_WIDTH ) )
		return visual;

	return -1;
}

int
NaHeaderView::cSectionViewportPosition( int visual ) const
{
	// TODO добавить offset
	return columns.pos( visual );
}

void
NaHeaderView::mousePressEvent( QMouseEvent * e )
{
	if ( state != NoState )
		return;

	if ( e->button() != Qt::LeftButton )
		return;

	const int handle = sectionHandleAt( e->pos().x() );

	if ( handle == -1 ) {	// секция перемещается

		m_section = m_target = m_pressed = logicalIndexAt( e->pos() );

		if ( m_section == -1 )
			return;

		state = MoveSection;
		setupSectionIndicator( m_section, e->pos() );

	} else {				// меняется размер секции
		m_originalSize = columns[ handle ].size;
		state = ResizeSection;
		m_section = handle;
	}

	m_firstPos = m_lastPos = e->x();
}

int
NaHeaderView::logicalIndexAt( const QPoint & pos ) const
{
	if ( pos.y() <= heightGroups() ) {	// область группировки

	} else {

		int next_section_start = 0;		// TODO добавить offset

		for ( int s = 0; s < columns.size(); ++s ) {
			next_section_start += columns[ s ].size;

			if ( pos.x() < next_section_start )
				return columns[ s ].logical;
		}
	}

	return -1;
}

void
NaHeaderView::setupSectionIndicator( int logical, const QPoint & pos )
{
	if ( ! sectionIndicator )
		sectionIndicator = new QLabel( viewport() );

	if ( ! targetMarker )
		targetMarker = new QLabel( viewport() );

	int px,
		py,
		w;
	const int h = heightColumns();

	int visualIndex = groups.visualIndex( logical );

	if ( visualIndex != -1 ) {
		// TODO
	} else {
		visualIndex = columns.visualIndex( logical );

		if ( visualIndex != -1 ) {
			px = cSectionViewportPosition( logical );
			py = heightGroups();
			w = columns[ visualIndex ].size;
		}
	}

	if ( visualIndex == -1 )
		return;

	sectionIndicator->resize( w, h );

	QPixmap pixmap( w, h );

	pixmap.fill( QColor( 0, 0, 0, 40 ) );

	QPainter painter( &pixmap );
	painter.setOpacity( .60 );

	Section::draw( painter, 0, 0, w, h, modelData( logical, Qt::DisplayRole ).toString(),
			Section::WithoutHandle );

	sectionIndicator->setPixmap( pixmap );
	sectionIndicatorOffset = QPoint( pos.x() - px, pos.y() - py );

	// target indicator

	targetMarker->setPixmap( QPixmap(":/arrow.png") );
}

void
NaHeaderView::resizeSection( int visual, int size )
{
	if ( visual < 0 || visual >= columns.count() )
		return;

	const int oldSize = columns[ visual ].size;

	if ( oldSize == size )
		return;

	columns[ visual ].size = size;

	if ( m_resizedSectionViewportPosition == -1 )
		m_resizedSectionViewportPosition = cSectionViewportPosition( visual );

	QRect rect( m_resizedSectionViewportPosition, heightGroups(),
			viewport()->width() - m_resizedSectionViewportPosition, heightColumns() );

	rect.normalized();

	viewport()->update( rect );

	// TODO emit signal sectionResized( rect );
}

void
NaHeaderView::mouseReleaseEvent( QMouseEvent * e )
{
	state = NoState;
	m_pressed = -1;
	m_resizedSectionViewportPosition = -1;
}

