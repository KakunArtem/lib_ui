// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ui/platform/ui_popup_backdrop.h"

#include <QtGui/QPainter>
#include <QtWidgets/QWidget>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>
#endif // Q_OS_WIN

namespace Ui::Platform {
namespace {

constexpr auto kProperty = "_ui_popup_backdrop";

auto Enabled = false;
auto FillOpacity = 1.;

#ifdef Q_OS_WIN
constexpr auto kDWMWA_USE_IMMERSIVE_DARK_MODE = DWORD(20);
constexpr auto kDWMWA_WINDOW_CORNER_PREFERENCE = DWORD(33);
constexpr auto kDWMWA_SYSTEMBACKDROP_TYPE = DWORD(38);
constexpr auto kDWMWCP_DEFAULT = DWORD(0);
constexpr auto kDWMWCP_ROUND = DWORD(2);
constexpr auto kDWMSBT_NONE = DWORD(1);
constexpr auto kDWMSBT_TRANSIENTWINDOW = DWORD(3);
#endif // Q_OS_WIN

} // namespace

void SetPopupBackdrop(bool enabled, float64 fillOpacity) {
	Enabled = enabled;
	FillOpacity = std::clamp(fillOpacity, 0., 1.);
}

bool PopupBackdrop() {
#ifdef Q_OS_WIN
	return Enabled;
#else // Q_OS_WIN
	return false;
#endif // Q_OS_WIN
}

void MarkPopupBackdrop(not_null<QWidget*> window, bool enabled) {
	window->setProperty(kProperty, enabled);
}

void ApplyPopupBackdrop(not_null<QWidget*> window, bool dark) {
#ifdef Q_OS_WIN
	const auto handle = reinterpret_cast<HWND>(window->winId());
	if (!handle) {
		return;
	}
	const auto margins = MARGINS{ -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(handle, &margins);
	const auto darkValue = BOOL(dark ? TRUE : FALSE);
	DwmSetWindowAttribute(
		handle,
		kDWMWA_USE_IMMERSIVE_DARK_MODE,
		&darkValue,
		sizeof(darkValue));
	const auto corner = kDWMWCP_ROUND;
	DwmSetWindowAttribute(
		handle,
		kDWMWA_WINDOW_CORNER_PREFERENCE,
		&corner,
		sizeof(corner));
	const auto type = kDWMSBT_TRANSIENTWINDOW;
	DwmSetWindowAttribute(
		handle,
		kDWMWA_SYSTEMBACKDROP_TYPE,
		&type,
		sizeof(type));
#endif // Q_OS_WIN
}

void RemovePopupBackdrop(not_null<QWidget*> window) {
#ifdef Q_OS_WIN
	const auto handle = reinterpret_cast<HWND>(window->winId());
	if (!handle) {
		return;
	}
	const auto type = kDWMSBT_NONE;
	DwmSetWindowAttribute(
		handle,
		kDWMWA_SYSTEMBACKDROP_TYPE,
		&type,
		sizeof(type));
	const auto corner = kDWMWCP_DEFAULT;
	DwmSetWindowAttribute(
		handle,
		kDWMWA_WINDOW_CORNER_PREFERENCE,
		&corner,
		sizeof(corner));
	const auto margins = MARGINS{ 0, 0, 0, 0 };
	DwmExtendFrameIntoClientArea(handle, &margins);
#endif // Q_OS_WIN
}

bool InPopupBackdrop(QPainter &p) {
	const auto widget = dynamic_cast<QWidget*>(p.device());
	return widget && widget->window()->property(kProperty).toBool();
}

void FillPopupBackground(QPainter &p, const QRect &rect, QColor color) {
	color.setAlphaF(color.alphaF() * FillOpacity);
	const auto mode = p.compositionMode();
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.fillRect(rect, color);
	p.setCompositionMode(mode);
}

} // namespace Ui::Platform
