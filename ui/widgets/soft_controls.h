// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/style/style_core_types.h"

#include <QtGui/QColor>
#include <QtCore/QRectF>
#include <QtGui/QPainterPath>

#include <optional>
#include <vector>

class QWidget;

namespace Ui {

struct SoftControlColors {
	QColor windowBg;
	QColor boxBg;
	QColor windowBgOver;
	QColor hover;
};

[[nodiscard]] QColor ResolveControlFill(
	const QColor &fill,
	const SoftControlColors &colors);

void SetSoftControls(bool enabled);
[[nodiscard]] bool SoftControls();
void SetSoftControlHover(QColor hover);
[[nodiscard]] QColor ControlFill(const style::color &fill);

// A BoxContent with this property set to true never shows the lines
// at the top and bottom of its scroll area.
inline constexpr auto kNoScrollShadowsProperty = "softNoScrollShadows";

// A widget that draws rounded cards behind its rows stores the card
// rectangles (in its own coordinates) and their corner radius here, so a
// row can keep its hover and ripple inside the rounded card corners.
void SetCardShapes(
	not_null<QWidget*> content,
	const std::vector<QRectF> &cards,
	float64 radius);

// The rounded hover shape, in widget coordinates, of a row that sits in
// one of those cards: a pill inset from the card's edges. Empty when the
// widget is not in a card.
[[nodiscard]] std::optional<QPainterPath> CardRowShape(
	not_null<const QWidget*> widget);

} // namespace Ui
