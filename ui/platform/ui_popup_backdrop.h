// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "base/basic_types.h"

#include <QtGui/QColor>

class QPainter;
class QRect;
class QWidget;

namespace Ui::Platform {

void SetPopupBackdrop(bool enabled, float64 fillOpacity);
[[nodiscard]] bool PopupBackdrop();

void MarkPopupBackdrop(not_null<QWidget*> window, bool enabled);
void ApplyPopupBackdrop(not_null<QWidget*> window, bool dark);
void RemovePopupBackdrop(not_null<QWidget*> window);

[[nodiscard]] bool InPopupBackdrop(QPainter &p);
void FillPopupBackground(QPainter &p, const QRect &rect, QColor color);

} // namespace Ui::Platform
