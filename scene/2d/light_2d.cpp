/**************************************************************************/
/*  light_2d.cpp                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
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
#include "light_2d.h"

void Light2D::owner_changed_notify() {
	// For cases where owner changes _after_ entering tree (as example, editor editing).
	_update_light_visibility();
}

void Light2D::_update_light_visibility() {
	if (!is_inside_tree()) {
		return;
	}

	bool editor_ok = true;

	if (editor_only) {
		if (!Engine::get_singleton()->is_editor_hint()) {
			editor_ok = false;
		} else {
			editor_ok = (get_tree()->get_edited_scene_root() && (this == get_tree()->get_edited_scene_root() || get_owner() == get_tree()->get_edited_scene_root()));
		}
	}

	RS::get_singleton()->canvas_light_set_enabled(canvas_light, enabled && is_visible_in_tree() && editor_ok);
}

void Light2D::set_enabled(bool p_enabled) {
	enabled = p_enabled;
	_update_light_visibility();
}

bool Light2D::is_enabled() const {
	return enabled;
}

void Light2D::set_editor_only(bool p_editor_only) {
	editor_only = p_editor_only;
	_update_light_visibility();
}

bool Light2D::is_editor_only() const {
	return editor_only;
}

void Light2D::set_color(const Color &p_color) {
	color = p_color;
	RS::get_singleton()->canvas_light_set_color(canvas_light, color);
}

Color Light2D::get_color() const {
	return color;
}

void Light2D::set_height(real_t p_height) {
	height = p_height;
	RS::get_singleton()->canvas_light_set_height(canvas_light, height);
}

real_t Light2D::get_height() const {
	return height;
}

void Light2D::set_energy(real_t p_energy) {
	energy = p_energy;
	RS::get_singleton()->canvas_light_set_energy(canvas_light, energy);
}

real_t Light2D::get_energy() const {
	return energy;
}

void Light2D::set_z_range_min(int p_min_z) {
	z_min = p_min_z;
	RS::get_singleton()->canvas_light_set_z_range(canvas_light, z_min, z_max);
}

int Light2D::get_z_range_min() const {
	return z_min;
}

void Light2D::set_z_range_max(int p_max_z) {
	z_max = p_max_z;
	RS::get_singleton()->canvas_light_set_z_range(canvas_light, z_min, z_max);
}

int Light2D::get_z_range_max() const {
	return z_max;
}

void Light2D::set_layer_range_min(int p_min_layer) {
	layer_min = p_min_layer;
	RS::get_singleton()->canvas_light_set_layer_range(canvas_light, layer_min, layer_max);
}

int Light2D::get_layer_range_min() const {
	return layer_min;
}

void Light2D::set_layer_range_max(int p_max_layer) {
	layer_max = p_max_layer;
	RS::get_singleton()->canvas_light_set_layer_range(canvas_light, layer_min, layer_max);
}

int Light2D::get_layer_range_max() const {
	return layer_max;
}

void Light2D::set_item_cull_mask(int p_mask) {
	item_mask = p_mask;
	RS::get_singleton()->canvas_light_set_item_cull_mask(canvas_light, item_mask);
}

int Light2D::get_item_cull_mask() const {
	return item_mask;
}

void Light2D::set_item_shadow_cull_mask(int p_mask) {
	item_shadow_mask = p_mask;
	RS::get_singleton()->canvas_light_set_item_shadow_cull_mask(canvas_light, item_shadow_mask);
}

int Light2D::get_item_shadow_cull_mask() const {
	return item_shadow_mask;
}

void Light2D::set_shadow_enabled(bool p_enabled) {
	shadow = p_enabled;
	RS::get_singleton()->canvas_light_set_shadow_enabled(canvas_light, shadow);
	notify_property_list_changed();
}

bool Light2D::is_shadow_enabled() const {
	return shadow;
}

void Light2D::set_shadow_filter(ShadowFilter p_filter) {
	ERR_FAIL_INDEX(p_filter, SHADOW_FILTER_MAX);
	shadow_filter = p_filter;
	RS::get_singleton()->canvas_light_set_shadow_filter(canvas_light, RS::CanvasLightShadowFilter(p_filter));
	notify_property_list_changed();
}

Light2D::ShadowFilter Light2D::get_shadow_filter() const {
	return shadow_filter;
}

void Light2D::set_shadow_color(const Color &p_shadow_color) {
	shadow_color = p_shadow_color;
	RS::get_singleton()->canvas_light_set_shadow_color(canvas_light, shadow_color);
}

Color Light2D::get_shadow_color() const {
	return shadow_color;
}

void Light2D::set_blend_mode(BlendMode p_mode) {
	blend_mode = p_mode;
	RS::get_singleton()->canvas_light_set_blend_mode(_get_light(), RS::CanvasLightBlendMode(p_mode));
}

Light2D::BlendMode Light2D::get_blend_mode() const {
	return blend_mode;
}

void Light2D::_physics_interpolated_changed() {
	RenderingServer::get_singleton()->canvas_light_set_interpolated(canvas_light, is_physics_interpolated());
}

void Light2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_CANVAS: {
			RS::get_singleton()->canvas_light_attach_to_canvas(canvas_light, get_canvas());
			_update_light_visibility();
		} break;

		case NOTIFICATION_TRANSFORM_CHANGED: {
			RS::get_singleton()->canvas_light_set_transform(canvas_light, get_global_transform());
		} break;

		case NOTIFICATION_VISIBILITY_CHANGED: {
			_update_light_visibility();
		} break;

		case NOTIFICATION_RESET_PHYSICS_INTERPOLATION: {
			if (is_visible_in_tree() && is_physics_interpolated()) {
				// Explicitly make sure the transform is up to date in RenderingServer before
				// resetting. This is necessary because NOTIFICATION_TRANSFORM_CHANGED
				// is normally deferred, and a client change to transform will not always be sent
				// before the reset, so we need to guarantee this.
				RS::get_singleton()->canvas_light_set_transform(canvas_light, get_global_transform());
				RS::get_singleton()->canvas_light_reset_physics_interpolation(canvas_light);
			}
		} break;

		case NOTIFICATION_EXIT_CANVAS: {
			RS::get_singleton()->canvas_light_attach_to_canvas(canvas_light, RID());
			_update_light_visibility();
		} break;
	}
}

void Light2D::set_shadow_smooth(real_t p_amount) {
	shadow_smooth = p_amount;
	RS::get_singleton()->canvas_light_set_shadow_smooth(canvas_light, shadow_smooth);
}

real_t Light2D::get_shadow_smooth() const {
	return shadow_smooth;
}

void Light2D::_validate_property(PropertyInfo &p_property) const {
	if (!shadow && (p_property.name == "shadow_color" || p_property.name == "shadow_filter" || p_property.name == "shadow_filter_smooth" || p_property.name == "shadow_item_cull_mask")) {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}

	if (shadow && p_property.name == "shadow_filter_smooth" && shadow_filter == SHADOW_FILTER_NONE) {
		p_property.usage = PROPERTY_USAGE_NO_EDITOR;
	}
}

void Light2D::_bind_methods() {
}

Light2D::Light2D() {
	canvas_light = RenderingServer::get_singleton()->canvas_light_create();
	set_notify_transform(true);
}

Light2D::~Light2D() {
	ERR_FAIL_NULL(RenderingServer::get_singleton());
	RenderingServer::get_singleton()->free(canvas_light);
}

Dictionary PointLight2D::_edit_get_state() const {
	Dictionary state = Node2D::_edit_get_state();
	state["offset"] = get_texture_offset();
	return state;
}

void PointLight2D::_edit_set_state(const Dictionary &p_state) {
	Node2D::_edit_set_state(p_state);
	set_texture_offset(p_state["offset"]);
}

void PointLight2D::_edit_set_pivot(const Point2 &p_pivot) {
	set_position(get_transform().xform(p_pivot));
	set_texture_offset(get_texture_offset() - p_pivot);
}

Point2 PointLight2D::_edit_get_pivot() const {
	return Vector2();
}

bool PointLight2D::_edit_use_pivot() const {
	return true;
}

Rect2 PointLight2D::_edit_get_rect() const {
	if (texture.is_null()) {
		return Rect2();
	}

	Size2 s = texture->get_size() * _scale;
	return Rect2(texture_offset - s / 2.0, s);
}

bool PointLight2D::_edit_use_rect() const {
	return !texture.is_null();
}


Rect2 PointLight2D::get_anchorable_rect() const {
	if (texture.is_null()) {
		return Rect2();
	}

	Size2 s = texture->get_size() * _scale;
	return Rect2(texture_offset - s / 2.0, s);
}

void PointLight2D::set_texture(const Ref<Texture2D> &p_texture) {
	texture = p_texture;
	if (texture.is_valid()) {
		RS::get_singleton()->canvas_light_set_texture(_get_light(), texture->get_rid());
	} else {
		RS::get_singleton()->canvas_light_set_texture(_get_light(), RID());
	}

	update_configuration_warnings();
}

Ref<Texture2D> PointLight2D::get_texture() const {
	return texture;
}

void PointLight2D::set_texture_offset(const Vector2 &p_offset) {
	texture_offset = p_offset;
	RS::get_singleton()->canvas_light_set_texture_offset(_get_light(), texture_offset);
	item_rect_changed();
}

Vector2 PointLight2D::get_texture_offset() const {
	return texture_offset;
}

PackedStringArray PointLight2D::get_configuration_warnings() const {
	PackedStringArray warnings = Node::get_configuration_warnings();

	if (!texture.is_valid()) {
		warnings.push_back(RTR("A texture with the shape of the light must be supplied to the \"Texture\" property."));
	}

	return warnings;
}

void PointLight2D::set_texture_scale(real_t p_scale) {
	_scale = p_scale;
	// Avoid having 0 scale values, can lead to errors in physics and rendering.
	if (_scale == 0) {
		_scale = CMP_EPSILON;
	}
	RS::get_singleton()->canvas_light_set_texture_scale(_get_light(), _scale);
	item_rect_changed();
}

real_t PointLight2D::get_texture_scale() const {
	return _scale;
}

void PointLight2D::_bind_methods() {
/*
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &PointLight2D::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &PointLight2D::get_texture);

	ClassDB::bind_method(D_METHOD("set_texture_offset", "texture_offset"), &PointLight2D::set_texture_offset);
	ClassDB::bind_method(D_METHOD("get_texture_offset"), &PointLight2D::get_texture_offset);

	ClassDB::bind_method(D_METHOD("set_texture_scale", "texture_scale"), &PointLight2D::set_texture_scale);
	ClassDB::bind_method(D_METHOD("get_texture_scale"), &PointLight2D::get_texture_scale);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset", PROPERTY_HINT_NONE, "suffix:px"), "set_texture_offset", "get_texture_offset");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "texture_scale", PROPERTY_HINT_RANGE, "0.01,50,0.01"), "set_texture_scale", "get_texture_scale");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "height", PROPERTY_HINT_RANGE, "0,1024,1,or_greater,suffix:px"), "set_height", "get_height");
*/
}

PointLight2D::PointLight2D() {
	RS::get_singleton()->canvas_light_set_mode(_get_light(), RS::CANVAS_LIGHT_MODE_POINT);
	set_hide_clip_children(true);
}

void DirectionalLight2D::set_max_distance(real_t p_distance) {
	max_distance = p_distance;
	RS::get_singleton()->canvas_light_set_directional_distance(_get_light(), max_distance);
}

real_t DirectionalLight2D::get_max_distance() const {
	return max_distance;
}

void DirectionalLight2D::_bind_methods() {
}

DirectionalLight2D::DirectionalLight2D() {
	RS::get_singleton()->canvas_light_set_mode(_get_light(), RS::CANVAS_LIGHT_MODE_DIRECTIONAL);
	set_max_distance(max_distance); // Update RenderingServer.
	set_hide_clip_children(true);
}
#endif
