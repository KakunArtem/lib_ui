// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ui/widgets/box_content_divider.h"

#include "ui/widgets/soft_controls.h"
#include "styles/style_layers.h"
#include "styles/style_widgets.h"
#include "styles/palette.h"

#include <QtGui/QPainter>
#include <QtGui/QtEvents>

namespace Ui {
namespace {

[[nodiscard]] bool InsideCards(not_null<const QWidget*> widget) {
	for (auto parent = widget->parentWidget()
		; parent
		; parent = parent->parentWidget()) {
		if (parent->property(kCardsProperty).toBool()) {
			return true;
		}
	}
	return false;
}

} // namespace

BoxContentDivider::BoxContentDivider(
	QWidget *parent,
	int height,
	const style::DividerBar &st,
	RectParts parts)
: RpWidget(parent)
, _st(st)
, _parts(parts) {
	resize(width(), height);
}

void BoxContentDivider::paintEvent(QPaintEvent *e) {
	if (InsideCards(this)) {
		// WHY: in a card layout the gap between cards is the divider,
		// so the bar and its shadows would cut through the panel.
		return;
	}
	QPainter p(this);

	const auto fill = ControlFill(_st.bg);
	if (fill.alpha() > 0) {
		p.fillRect(e->rect(), fill);
	}
	if (_parts & RectPart::Top) {
		paintTop(p);
	}
	if (_parts & RectPart::Bottom) {
		paintBottom(p);
	}
}

void BoxContentDivider::paintTop(QPainter &p, int skip) {
	const auto dividerFillTop = QRect(
		0,
		skip,
		width(),
		_st.top.height());
	_st.top.fill(p, dividerFillTop);
}

void BoxContentDivider::paintBottom(QPainter &p, int skip) {
	const auto dividerFillBottom = myrtlrect(
		0,
		height() - skip - _st.bottom.height(),
		width(),
		_st.bottom.height());
	_st.bottom.fill(p, dividerFillBottom);
}

const style::color &BoxContentDivider::color() const {
	return _st.bg;
}

} // namespace Ui
