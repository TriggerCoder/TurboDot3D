/**************************************************************************/
/*  canvas_group.cpp                                                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-2024 Godot Engine contributors (see ORGAUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifdef TOOLS_ENABLED //2D
#include "canvas_group.h"

void CanvasGroup::set_fit_margin(real_t p_fit_margin) {
	ERR_FAIL_COND(p_fit_margin < 0.0);

	fit_margin = p_fit_margin;
	RS::get_singleton()->canvas_item_set_canvas_group_mode(get_canvas_item(), RS::CANVAS_GROUP_MODE_TRANSPARENT, clear_margin, true, fit_margin, use_mipmaps);

	queue_redraw();
}

real_t CanvasGroup::get_fit_margin() const {
	return fit_margin;
}

void CanvasGroup::set_clear_margin(real_t p_clear_margin) {
	ERR_FAIL_COND(p_clear_margin < 0.0);

	clear_margin = p_clear_margin;
	RS::get_singleton()->canvas_item_set_canvas_group_mode(get_canvas_item(), RS::CANVAS_GROUP_MODE_TRANSPARENT, clear_margin, true, fit_margin, use_mipmaps);

	queue_redraw();
}

real_t CanvasGroup::get_clear_margin() const {
	return clear_margin;
}

void CanvasGroup::set_use_mipmaps(bool p_use_mipmaps) {
	use_mipmaps = p_use_mipmaps;
	RS::get_singleton()->canvas_item_set_canvas_group_mode(get_canvas_item(), RS::CANVAS_GROUP_MODE_TRANSPARENT, clear_margin, true, fit_margin, use_mipmaps);
}
bool CanvasGroup::is_using_mipmaps() const {
	return use_mipmaps;
}

void CanvasGroup::_bind_methods() {
}

CanvasGroup::CanvasGroup() {
	set_fit_margin(10.0); //sets things
}
CanvasGroup::~CanvasGroup() {
	RS::get_singleton()->canvas_item_set_canvas_group_mode(get_canvas_item(), RS::CANVAS_GROUP_MODE_DISABLED);
}
#endif
