// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/style/style_core_types.h"

#include <QtGui/QColor>

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

} // namespace Ui
