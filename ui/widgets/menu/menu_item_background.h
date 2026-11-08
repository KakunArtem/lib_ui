// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <QtCore/QRect>
#include <QtGui/QImage>

class QPainter;

namespace style {
struct Menu;
} // namespace style

namespace Ui::Menu {

void PaintItemBackground(
	QPainter &p,
	const style::Menu &st,
	const QRect &rect,
	bool selected);
[[nodiscard]] QImage ItemRippleMask(QSize size);

} // namespace Ui::Menu
