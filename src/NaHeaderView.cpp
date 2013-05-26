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

#include <QDebug>

NaHeaderView::NaHeaderView( QWidget * parent )
	: QAbstractItemView( parent ),
	  state( NoState ),
	  m_offset( 0 ),
	  m_gr_offset( 0 ),
	  heightColumns_cached( -1 ),
	  heightGroups_cached( -1 ),
	  snow_png( QPixmap(":/snow.png") ),
	  wind_png( ":/wind.png" ),
	  m_resizedSectionViewportPosition( -1 ),
	  sectionIndicator( 0 ),
	  targetMarker( 0 )
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
	painter.drawPixmap( wind_x, wind_y, wind_png );

	painter.setPen( QPen( COLOR_BORDER ) );

	int start = gVisualIndexAt( e->rect().left() ),
		end = gVisualIndexAt( e->rect().right() );

	start = ( start == -1 ? 0 : start );
	end = ( end == -1 ? groups.count() - 1 : end );

	// размеры закорючки
	const int small = GROUP_AREA_SPACING / 2,
			  big = qRound( heightColumns() * .3 ),
			  toff = ( heightColumns() - big ) / 2;

	for ( register int i = start; i <= end; ++i ) {

		const int pos = gSectionViewportPosition( i );

		if ( i > 0 ) {	// нарисовать закорючку
			painter.drawLine( pos - GROUP_AREA_SPACING, GROUP_AREA_MARGIN + toff,
					pos - GROUP_AREA_SPACING + small, GROUP_AREA_MARGIN + toff );
			painter.drawLine( pos - GROUP_AREA_SPACING + small, GROUP_AREA_MARGIN + toff,
					pos - GROUP_AREA_SPACING + small, GROUP_AREA_MARGIN + toff + big );
			painter.drawLine( pos - GROUP_AREA_SPACING + small, GROUP_AREA_MARGIN + toff + big,
					pos, GROUP_AREA_MARGIN + toff + big );
		}

		Section::draw( painter, pos, GROUP_AREA_MARGIN, DEFAULT_SECTION_SIZE, heightColumns(),
				modelData( groups[ i ].logical, Qt::DisplayRole ).toString(),
				Section::DrawRibbon, groups[ i ].color );
	}

	/*
	\\ ordinary sections
	*/

	painter.drawLine( 0, heightGroups(), viewport()->width(), heightGroups() );

	start = cVisualIndexAt( e->rect().left() );
	end = cVisualIndexAt( e->rect().right() );

	start = ( start == -1 ? 0 : start );
	end = ( end == -1 ? columns.count() - 1 : end );

	for ( register int i = start; i <= end; ++i ) {
		const int pos = columns.pos( i );

		columns[ i ].draw( painter, pos, heightGroups(), heightColumns(),
				modelData( columns[ i ].logical, Qt::DisplayRole ).toString(), Section::DrawHandle );
	}
}

int
NaHeaderView::gVisualIndexAt( int pos ) const
{
	const int virtual_sec = pos / ( DEFAULT_SECTION_SIZE + GROUP_AREA_SPACING );

	return virtual_sec < groups.size() ? virtual_sec : -1;
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
			const int delta = x - m_firstPos.x();
			resizeSection( m_section, qMax( m_originalSize + delta, MIN_SECTION_SIZE ) );
			//m_lastPos = x;	????????????????????? TODO
			return;
		}
		case MoveSection: {
			if ( ( e->pos() - m_firstPos ).manhattanLength() >= qApp->startDragDistance() ||
			  ! sectionIndicator->isHidden() ) {


				updateSectionIndicator( m_section, e->pos() );
				updateTargetMarker( e->pos() );
				// viewport()->update() ????????????????????? TODO
			}
			return;
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
NaHeaderView::gSectionViewportPosition( int visual ) const
{
	return  GROUP_AREA_MARGIN + visual * ( GROUP_AREA_SPACING + DEFAULT_SECTION_SIZE );
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

	//m_firstPos = m_lastPos = e->x();
	m_firstPos = m_lastPos = e->pos();
}

int
NaHeaderView::logicalIndexAt( const QPoint & pos ) const
{
	const int x = pos.x(),
		      y = pos.y();

	if ( y <= heightGroups() ) {	// область группировки

		if ( groups.isEmpty() )
			return -1;

		// отсечь отступы области группировки: верхний...
		if ( y < GROUP_AREA_MARGIN )
			return -1;
		// ...и нижний ...
		if ( y > GROUP_AREA_MARGIN + heightColumns() &&
				y <= heightGroups() )
			return -1;

		int section_end = GROUP_AREA_MARGIN; // TODO добавить gr_offset

		for ( int s = 0; s < groups.size(); ++s ) {
			if ( x < section_end )	// попали в левый отступ или закорючку
				return -1;

			section_end += DEFAULT_SECTION_SIZE;

			if ( x < section_end )
				return groups[ s ].logical;

			section_end += GROUP_AREA_SPACING;
		}

		return -1;

	} else {

		int next_section_start = 0;		// TODO добавить offset

		for ( int s = 0; s < columns.size(); ++s ) {
			next_section_start += columns[ s ].size;

			if ( x < next_section_start )
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

	if ( ! targetMarker ) {
		targetMarker = new QLabel( viewport() );
		targetMarker->setPixmap( QPixmap(":/target.png") );
	}

	int px,
		py,
		w;
	const int h = heightColumns();

	int visual = groups.visualIndex( logical );

	if ( visual != -1 ) {
		px = gSectionViewportPosition( visual );
		py = GROUP_AREA_MARGIN;
		w = DEFAULT_SECTION_SIZE;

	} else {
		visual = columns.visualIndex( logical );

		if ( visual != -1 ) {
			px = cSectionViewportPosition( visual );
			py = heightGroups();
			w = columns[ visual ].size;
		}
	}

	if ( visual == -1 )
		return;

	sectionIndicator->resize( w, h );

	QPixmap pixmap( w, h );

	pixmap.fill( QColor( 0, 0, 0, 40 ) );

	QPainter painter( &pixmap );
	painter.setOpacity( .60 );

	Section::draw( painter, 0, 0, w, h, modelData( logical, Qt::DisplayRole ).toString(),
			Section::DrawNothing );


	sectionIndicator->setPixmap( pixmap );
	sectionIndicatorOffset = QPoint( pos.x() - px, pos.y() - py );
}

void
NaHeaderView::updateSectionIndicator( int logical, const QPoint & pos )
{
	if ( ! sectionIndicator )
		return;

	if ( logical == -1 || m_target == -1 ) {
		sectionIndicator->hide();
		return;
	}

	int x = pos.x() - sectionIndicatorOffset.x(),
		y = pos.y() - sectionIndicatorOffset.y();

	x = qMax( 0, qMin( x, viewport()->width() ) );
	y = qMax( 0, qMin( y, heightGroups() ) );

	sectionIndicator->move( x, y );

	sectionIndicator->show();
}

void
NaHeaderView::updateTargetMarker( const QPoint & pos )
{
	if ( ! targetMarker )
		return;

	if ( m_target == -1 ) {
		targetMarker->hide();
		return;
	}

	targetIsGroupArea = ( pos.y() <= heightGroups() );

	int x,
		y;

	if ( targetIsGroupArea ) {

		if ( groups.isEmpty() ) {
			x = GROUP_AREA_MARGIN;
			m_target = 0;

		} else {
			int visual = gVisualIndexAt( pos.x() );

			if ( visual == -1 )
				visual = groups.count() - 1;

			x = gSectionViewportPosition( visual );

			int sectionCenter = x + DEFAULT_SECTION_SIZE / 2;

			if ( sectionCenter >= pos.x() )
				m_target = visual;
			else {
				x += DEFAULT_SECTION_SIZE + GROUP_AREA_SPACING;
				m_target = visual + 1;
			}
		}

		x -= targetMarker->width() / 2;
		y = heightGroups() / 2;

	} else {
		int visual = cVisualIndexAt( pos.x() );

		if ( visual == -1 )
			visual = columns.count() - 1;

		x = cSectionViewportPosition( visual );

		int sectionCenter = x + columns[ visual ].size / 2;

		if ( sectionCenter >= pos.x() ) {
			x -= targetMarker->width() / 2;
			m_target = visual;
		} else {
			x += columns[ visual ].size - targetMarker->width() / 2;
			m_target = visual + 1;
		}

		y = heightGroups() + heightColumns() / 2 - targetMarker->height() / 3;
	}

	targetMarker->move( x, y );
	targetMarker->show();
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
	switch ( state ) {
		case MoveSection : {
			if ( targetMarker->isHidden() )
				break;

			moveSection();

			m_section = m_target = -1;
			sectionIndicator->hide();
			targetMarker->hide();
			break;
		}
		default:
			break;
	}

	state = NoState;
	m_pressed = -1;
	m_resizedSectionViewportPosition = -1;
}

bool
NaHeaderView::overGroupArea( const QPoint & pos ) const
{
	const int y = pos.y();
		      //x = pos.x(); TODO добавить проверку y

	return ( y >= 0 && y < heightGroups() );
}

void
NaHeaderView::moveSection()
{
	if ( m_section == -1 )
		return;

	int visual_from = groups.visualIndex( m_section );

	const bool from_groups = visual_from != -1;

	if ( ! from_groups )
		visual_from = columns.visualIndex( m_section );

	if ( from_groups == targetIsGroupArea &&
			visual_from == m_target ) {
		viewport()->update( sectionIndicator->x(), 0, sectionIndicator->width(), heightHint() );
		return;
	}

	const int oldGroupsCount = groups.count();

	Sections & secs = targetIsGroupArea ? groups : columns;

	//const bool pasteLast = targetIsGroupArea ? m_target == groups.size() : m_target == columns.size();

	const Section sec = from_groups ? groups[ visual_from ] : columns[ visual_from ];

	// добавление секции

	if ( m_target == secs.count() )
		secs.append( sec );
	else
		secs.insert( m_target, sec );

	// удаление перемещённой секции

	// секция перемещается в пределах одной зоны и сдвигается ближе к началу
	if ( from_groups == targetIsGroupArea &&
			visual_from > m_target )
		++visual_from;

	if ( from_groups )
		groups.remove( visual_from );
	else
		columns.remove( visual_from );

	if ( from_groups || targetIsGroupArea ) {
		heightGroups_cached = -1;

		emit groupsChanged( oldGroupsCount, groups.count() );
	}

	viewport()->update();
}

bool
NaHeaderView::groupped() const
{
	return ! groups.isEmpty();
}

/*
bool
NaHeaderView::sectionInGroups( int logical ) const
{
	// ищем там где меньше секций

	const bool groups_less = groups.size() < columns.size();

	const Sections & less = groups_less ? groups : columns;

	for ( int i = 0; i < less.size(); ++i )
		if ( less[ i ].logical == logical )
			return groups_less;

	return ! groups_less;
}
*/

int
NaHeaderView::columnsCount() const
{
	return columns.count();
}

const Section &
NaHeaderView::section( int visual ) const
{
	return columns[ visual ];
}

QVector< int >
NaHeaderView::groupsLogicals() const
{
	return groups.logicals();
}

QColor
NaHeaderView::colorGroup( int visual ) const
{
	if ( visual >= groups.count() )
		return QColor();

	return groups[ visual ].color;
}


