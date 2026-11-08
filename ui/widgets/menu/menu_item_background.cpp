// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ui/widgets/menu/menu_item_background.h"

#include "ui/effects/ripple_animation.h"
#include "ui/widgets/soft_controls.h"
#include "ui/painter.h"
#include "styles/style_widgets.h"

#include <QtGui/QPainter>

namespace Ui::Menu {
namespace {

[[nodiscard]] QRect SoftItemRect(const QRect &rect) {
	const auto margin = st::menuSoftItemMargin;
	return rect.marginsRemoved(QMargins(margin, 0, margin, 0));
}

} // namespace

void PaintItemBackground(
		QPainter &p,
		const style::Menu &st,
		const QRect &rect,
		bool selected) {
	if (!SoftControls()) {
		if (selected && st.itemBgOver->c.alpha() < 255) {
			p.fillRect(rect, st.itemBg);
		}
		p.fillRect(rect, selected ? st.itemBgOver : st.itemBg);
		return;
	}
	p.fillRect(rect, st.itemBg);
	if (!selected) {
		return;
	}
	const auto radius = st::menuSoftItemRadius;
	p.save();
	p.setRenderHint(QPainter::Antialiasing);
	p.setPen(Qt::NoPen);
	p.setBrush(st.itemBgOver);
	p.drawRoundedRect(SoftItemRect(rect), radius, radius);
	p.restore();
}

QImage ItemRippleMask(QSize size) {
	if (!SoftControls()) {
		return RippleAnimation::RectMask(size);
	}
	const auto radius = st::menuSoftItemRadius;
	return RippleAnimation::MaskByDrawer(size, false, [&](QPainter &p) {
		p.drawRoundedRect(
			SoftItemRect(QRect(QPoint(), size)),
			radius,
			radius);
	});
}

} // namespace Ui::Menu
