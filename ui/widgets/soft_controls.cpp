// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ui/widgets/soft_controls.h"

#include "styles/palette.h"
#include "ui/style/style_core_scale.h"

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtWidgets/QWidget>

namespace Ui {
namespace {

auto Enabled = false;
auto Hover = QColor();

constexpr auto kCardShapesProperty = "softCardShapes";
constexpr auto kCardRadiusProperty = "softCardRadius";

// The bars below a chat sit on the wallpaper, not on a surface.
[[nodiscard]] bool IsChatBarFill(const style::color &fill) {
	return (fill.operator->() == st::historyComposeButtonBg.operator->())
		|| (fill.operator->() == st::historyComposeButtonBgOver.operator->());
}

} // namespace

QColor ResolveControlFill(
		const QColor &fill,
		const SoftControlColors &colors) {
	if (fill == colors.windowBg || fill == colors.boxBg) {
		return QColor(0, 0, 0, 0);
	} else if (fill == colors.windowBgOver) {
		return colors.hover;
	}
	return fill;
}

void SetSoftControls(bool enabled) {
	Enabled = enabled;
}

bool SoftControls() {
	return Enabled;
}

void SetSoftControlHover(QColor hover) {
	Hover = hover;
}

QColor ControlFill(const style::color &fill) {
	if (!Enabled || IsChatBarFill(fill)) {
		return fill->c;
	}
	return ResolveControlFill(fill->c, {
		.windowBg = st::windowBg->c,
		.boxBg = st::boxBg->c,
		.windowBgOver = st::windowBgOver->c,
		.hover = Hover,
	});
}

void SetCardShapes(
		not_null<QWidget*> content,
		const std::vector<QRectF> &cards,
		float64 radius) {
	auto list = QVariantList();
	list.reserve(int(cards.size()));
	for (const auto &card : cards) {
		list.push_back(card);
	}
	content->setProperty(kCardShapesProperty, list);
	content->setProperty(kCardRadiusProperty, radius);
}

std::optional<QPainterPath> CardRowShape(not_null<const QWidget*> widget) {
	if (!Enabled) {
		return std::nullopt;
	}
	auto offset = QPoint();
	auto content = widget.get();
	while (content && !content->property(kCardShapesProperty).isValid()) {
		offset += content->pos();
		content = content->parentWidget();
		if (content && content->isWindow()) {
			return std::nullopt;
		}
	}
	if (!content) {
		return std::nullopt;
	}
	const auto radius = content->property(kCardRadiusProperty).toDouble();
	if (radius <= 0.) {
		return std::nullopt;
	}
	const auto mine = QRectF(QRect(offset, widget->size()));
	const auto list = content->property(kCardShapesProperty).toList();
	for (const auto &value : list) {
		const auto card = value.toRectF();
		if (!card.intersects(mine)) {
			continue;
		}
		// WHY: a full-width hover keeps a hard edge wherever the row
		// ends inside the card, so rows in a card get a rounded pill
		// inset from the card's edges instead.
		const auto sideInset = float64(style::ConvertScale(4));
		const auto verticalInset = float64(style::ConvertScale(2));
		const auto pill = mine.intersected(card).marginsRemoved({
			sideInset,
			verticalInset,
			sideInset,
			verticalInset,
		}).translated(-offset);
		if (pill.isEmpty()) {
			return std::nullopt;
		}
		const auto corner = std::max(
			radius - sideInset,
			float64(style::ConvertScale(4)));
		auto shape = QPainterPath();
		shape.addRoundedRect(pill, corner, corner);
		return shape;
	}
	return std::nullopt;
}

} // namespace Ui
