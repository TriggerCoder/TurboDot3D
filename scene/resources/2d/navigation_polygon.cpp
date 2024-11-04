/**************************************************************************/
/*  navigation_polygon.cpp                                                */
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
#include "navigation_polygon.h"

#include "core/math/geometry_2d.h"
#include "core/os/mutex.h"

#include "thirdparty/misc/polypartition.h"

Rect2 NavigationPolygon::_edit_get_rect() const {
	RWLockRead read_lock(rwlock);
	if (rect_cache_dirty) {
		item_rect = Rect2();
		bool first = true;

		for (int i = 0; i < outlines.size(); i++) {
			const Vector<Vector2> &outline = outlines[i];
			const int outline_size = outline.size();
			if (outline_size < 3) {
				continue;
			}
			const Vector2 *p = outline.ptr();
			for (int j = 0; j < outline_size; j++) {
				if (first) {
					item_rect = Rect2(p[j], Vector2(0, 0));
					first = false;
				} else {
					item_rect.expand_to(p[j]);
				}
			}
		}

		rect_cache_dirty = false;
	}
	return item_rect;
}

bool NavigationPolygon::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	RWLockRead read_lock(rwlock);
	for (int i = 0; i < outlines.size(); i++) {
		const Vector<Vector2> &outline = outlines[i];
		const int outline_size = outline.size();
		if (outline_size < 3) {
			continue;
		}
		if (Geometry2D::is_point_in_polygon(p_point, Variant(outline))) {
			return true;
		}
	}
	return false;
}

void NavigationPolygon::set_vertices(const Vector<Vector2> &p_vertices) {
	RWLockWrite write_lock(rwlock);
	{
		MutexLock lock(navigation_mesh_generation);
		navigation_mesh.unref();
	}
	vertices = p_vertices;
	rect_cache_dirty = true;
}

Vector<Vector2> NavigationPolygon::get_vertices() const {
	RWLockRead read_lock(rwlock);
	return vertices;
}

void NavigationPolygon::_set_polygons(const TypedArray<Vector<int32_t>> &p_array) {
	RWLockWrite write_lock(rwlock);
	{
		MutexLock lock(navigation_mesh_generation);
		navigation_mesh.unref();
	}
	polygons.resize(p_array.size());
	for (int i = 0; i < p_array.size(); i++) {
		polygons.write[i].indices = p_array[i];
	}
}

TypedArray<Vector<int32_t>> NavigationPolygon::_get_polygons() const {
	RWLockRead read_lock(rwlock);
	TypedArray<Vector<int32_t>> ret;
	ret.resize(polygons.size());
	for (int i = 0; i < ret.size(); i++) {
		ret[i] = polygons[i].indices;
	}

	return ret;
}

void NavigationPolygon::_set_outlines(const TypedArray<Vector<Vector2>> &p_array) {
	RWLockWrite write_lock(rwlock);
	outlines.resize(p_array.size());
	for (int i = 0; i < p_array.size(); i++) {
		outlines.write[i] = p_array[i];
	}
	rect_cache_dirty = true;
}

TypedArray<Vector<Vector2>> NavigationPolygon::_get_outlines() const {
	RWLockRead read_lock(rwlock);
	TypedArray<Vector<Vector2>> ret;
	ret.resize(outlines.size());
	for (int i = 0; i < ret.size(); i++) {
		ret[i] = outlines[i];
	}

	return ret;
}

void NavigationPolygon::add_polygon(const Vector<int> &p_polygon) {
	RWLockWrite write_lock(rwlock);
	Polygon polygon;
	polygon.indices = p_polygon;
	polygons.push_back(polygon);
	{
		MutexLock lock(navigation_mesh_generation);
		navigation_mesh.unref();
	}
}

void NavigationPolygon::add_outline_at_index(const Vector<Vector2> &p_outline, int p_index) {
	RWLockWrite write_lock(rwlock);
	outlines.insert(p_index, p_outline);
	rect_cache_dirty = true;
}

int NavigationPolygon::get_polygon_count() const {
	RWLockRead read_lock(rwlock);
	return polygons.size();
}

Vector<int> NavigationPolygon::get_polygon(int p_idx) {
	RWLockRead read_lock(rwlock);
	ERR_FAIL_INDEX_V(p_idx, polygons.size(), Vector<int>());
	return polygons[p_idx].indices;
}

void NavigationPolygon::clear_polygons() {
	RWLockWrite write_lock(rwlock);
	polygons.clear();
	{
		MutexLock lock(navigation_mesh_generation);
		navigation_mesh.unref();
	}
}

void NavigationPolygon::clear() {
	RWLockWrite write_lock(rwlock);
	polygons.clear();
	vertices.clear();
	{
		MutexLock lock(navigation_mesh_generation);
		navigation_mesh.unref();
	}
}

void NavigationPolygon::set_data(const Vector<Vector2> &p_vertices, const Vector<Vector<int>> &p_polygons) {
	RWLockWrite write_lock(rwlock);
	vertices = p_vertices;
	polygons.resize(p_polygons.size());
	for (int i = 0; i < p_polygons.size(); i++) {
		polygons.write[i].indices = p_polygons[i];
	}
	{
		MutexLock lock(navigation_mesh_generation);
		navigation_mesh.unref();
	}
}

void NavigationPolygon::get_data(Vector<Vector2> &r_vertices, Vector<Vector<int>> &r_polygons) {
	RWLockRead read_lock(rwlock);
	r_vertices = vertices;
	r_polygons.resize(polygons.size());
	for (int i = 0; i < polygons.size(); i++) {
		r_polygons.write[i] = polygons[i].indices;
	}
}

Ref<NavigationMesh> NavigationPolygon::get_navigation_mesh() {
	MutexLock lock(navigation_mesh_generation);

	if (navigation_mesh.is_null()) {
		navigation_mesh.instantiate();
		Vector<Vector3> verts;
		Vector<Vector<int>> polys;
		{
			verts.resize(get_vertices().size());
			Vector3 *w = verts.ptrw();

			const Vector2 *r = get_vertices().ptr();

			for (int i(0); i < get_vertices().size(); i++) {
				w[i] = Vector3(r[i].x, 0.0, r[i].y);
			}
		}

		for (int i(0); i < get_polygon_count(); i++) {
			polys.push_back(get_polygon(i));
		}

		navigation_mesh->set_data(verts, polys);
		navigation_mesh->set_cell_size(cell_size); // Needed to not fail the cell size check on the server
	}

	return navigation_mesh;
}

void NavigationPolygon::add_outline(const Vector<Vector2> &p_outline) {
	RWLockWrite write_lock(rwlock);
	outlines.push_back(p_outline);
	rect_cache_dirty = true;
}

int NavigationPolygon::get_outline_count() const {
	RWLockRead read_lock(rwlock);
	return outlines.size();
}

void NavigationPolygon::set_outline(int p_idx, const Vector<Vector2> &p_outline) {
	RWLockWrite write_lock(rwlock);
	ERR_FAIL_INDEX(p_idx, outlines.size());
	outlines.write[p_idx] = p_outline;
	rect_cache_dirty = true;
}

void NavigationPolygon::remove_outline(int p_idx) {
	RWLockWrite write_lock(rwlock);
	ERR_FAIL_INDEX(p_idx, outlines.size());
	outlines.remove_at(p_idx);
	rect_cache_dirty = true;
}

Vector<Vector2> NavigationPolygon::get_outline(int p_idx) const {
	RWLockRead read_lock(rwlock);
	ERR_FAIL_INDEX_V(p_idx, outlines.size(), Vector<Vector2>());
	return outlines[p_idx];
}

void NavigationPolygon::clear_outlines() {
	RWLockWrite write_lock(rwlock);
	outlines.clear();
	rect_cache_dirty = true;
}

void NavigationPolygon::set_cell_size(real_t p_cell_size) {
	cell_size = p_cell_size;
	get_navigation_mesh()->set_cell_size(cell_size);
}

real_t NavigationPolygon::get_cell_size() const {
	return cell_size;
}

void NavigationPolygon::set_border_size(real_t p_value) {
	ERR_FAIL_COND(p_value < 0.0);
	border_size = p_value;
}

real_t NavigationPolygon::get_border_size() const {
	return border_size;
}

void NavigationPolygon::set_parsed_geometry_type(ParsedGeometryType p_geometry_type) {
	ERR_FAIL_INDEX(p_geometry_type, PARSED_GEOMETRY_MAX);
	parsed_geometry_type = p_geometry_type;
	notify_property_list_changed();
}

NavigationPolygon::ParsedGeometryType NavigationPolygon::get_parsed_geometry_type() const {
	return parsed_geometry_type;
}

void NavigationPolygon::set_parsed_collision_mask(uint32_t p_mask) {
	parsed_collision_mask = p_mask;
}

uint32_t NavigationPolygon::get_parsed_collision_mask() const {
	return parsed_collision_mask;
}

void NavigationPolygon::set_parsed_collision_mask_value(int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Collision layer number must be between 1 and 32 inclusive.");
	uint32_t mask = get_parsed_collision_mask();
	if (p_value) {
		mask |= 1 << (p_layer_number - 1);
	} else {
		mask &= ~(1 << (p_layer_number - 1));
	}
	set_parsed_collision_mask(mask);
}

bool NavigationPolygon::get_parsed_collision_mask_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Collision layer number must be between 1 and 32 inclusive.");
	return get_parsed_collision_mask() & (1 << (p_layer_number - 1));
}

void NavigationPolygon::set_source_geometry_mode(SourceGeometryMode p_geometry_mode) {
	ERR_FAIL_INDEX(p_geometry_mode, SOURCE_GEOMETRY_MAX);
	source_geometry_mode = p_geometry_mode;
	notify_property_list_changed();
}

NavigationPolygon::SourceGeometryMode NavigationPolygon::get_source_geometry_mode() const {
	return source_geometry_mode;
}

void NavigationPolygon::set_source_geometry_group_name(const StringName &p_group_name) {
	source_geometry_group_name = p_group_name;
}

StringName NavigationPolygon::get_source_geometry_group_name() const {
	return source_geometry_group_name;
}

void NavigationPolygon::set_agent_radius(real_t p_value) {
	ERR_FAIL_COND(p_value < 0);
	agent_radius = p_value;
}

real_t NavigationPolygon::get_agent_radius() const {
	return agent_radius;
}

void NavigationPolygon::set_baking_rect(const Rect2 &p_rect) {
	baking_rect = p_rect;
	emit_changed();
}

Rect2 NavigationPolygon::get_baking_rect() const {
	return baking_rect;
}

void NavigationPolygon::set_baking_rect_offset(const Vector2 &p_rect_offset) {
	baking_rect_offset = p_rect_offset;
	emit_changed();
}

Vector2 NavigationPolygon::get_baking_rect_offset() const {
	return baking_rect_offset;
}

void NavigationPolygon::_bind_methods() {
}

void NavigationPolygon::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == "parsed_collision_mask") {
		if (parsed_geometry_type == PARSED_GEOMETRY_MESH_INSTANCES) {
			p_property.usage = PROPERTY_USAGE_NONE;
			return;
		}
	}

	if (p_property.name == "parsed_source_group_name") {
		if (source_geometry_mode == SOURCE_GEOMETRY_ROOT_NODE_CHILDREN) {
			p_property.usage = PROPERTY_USAGE_NONE;
			return;
		}
	}
}
#endif
