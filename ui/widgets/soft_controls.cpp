// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ui/widgets/soft_controls.h"

#include "styles/palette.h"

namespace Ui {
namespace {

auto Enabled = false;
auto Hover = QColor();

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

} // namespace Ui
