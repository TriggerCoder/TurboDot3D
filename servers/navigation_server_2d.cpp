/**************************************************************************/
/*  navigation_server_2d.cpp                                              */
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
#include "navigation_server_2d.h"

#include "servers/navigation_server_3d.h"

NavigationServer2D *NavigationServer2D::singleton = nullptr;

void NavigationServer2D::_bind_methods() {

}

NavigationServer2D *NavigationServer2D::get_singleton() {
	return singleton;
}

NavigationServer2D::NavigationServer2D() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
	ERR_FAIL_NULL_MSG(NavigationServer3D::get_singleton(), "The Navigation3D singleton should be initialized before the 2D one.");
	NavigationServer3D::get_singleton()->connect("map_changed", callable_mp(this, &NavigationServer2D::_emit_map_changed));

#ifdef DEBUG_ENABLED
	NavigationServer3D::get_singleton()->connect(SNAME("navigation_debug_changed"), callable_mp(this, &NavigationServer2D::_emit_navigation_debug_changed_signal));
#endif // DEBUG_ENABLED
}

#ifdef DEBUG_ENABLED
void NavigationServer2D::_emit_navigation_debug_changed_signal() {
	emit_signal(SNAME("navigation_debug_changed"));
}
#endif // DEBUG_ENABLED

NavigationServer2D::~NavigationServer2D() {
	singleton = nullptr;
}

void NavigationServer2D::_emit_map_changed(RID p_map) {
	emit_signal(SNAME("map_changed"), p_map);
}

void NavigationServer2D::set_debug_enabled(bool p_enabled) {
	NavigationServer3D::get_singleton()->set_debug_enabled(p_enabled);
}

bool NavigationServer2D::get_debug_enabled() const {
	return NavigationServer3D::get_singleton()->get_debug_enabled();
}

#ifdef DEBUG_ENABLED
void NavigationServer2D::set_debug_navigation_enabled(bool p_enabled) {
	NavigationServer3D::get_singleton()->set_debug_navigation_enabled(p_enabled);
}

bool NavigationServer2D::get_debug_navigation_enabled() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_enabled();
}

void NavigationServer2D::set_debug_avoidance_enabled(bool p_enabled) {
	NavigationServer3D::get_singleton()->set_debug_avoidance_enabled(p_enabled);
}

bool NavigationServer2D::get_debug_avoidance_enabled() const {
	return NavigationServer3D::get_singleton()->get_debug_avoidance_enabled();
}

void NavigationServer2D::set_debug_navigation_edge_connection_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_edge_connection_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_edge_connection_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_edge_connection_color();
}

void NavigationServer2D::set_debug_navigation_geometry_face_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_geometry_face_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_geometry_face_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_geometry_face_color();
}

void NavigationServer2D::set_debug_navigation_geometry_face_disabled_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_geometry_face_disabled_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_geometry_face_disabled_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_geometry_face_disabled_color();
}

void NavigationServer2D::set_debug_navigation_link_connection_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_link_connection_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_link_connection_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_link_connection_color();
}

void NavigationServer2D::set_debug_navigation_link_connection_disabled_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_link_connection_disabled_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_link_connection_disabled_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_link_connection_disabled_color();
}

void NavigationServer2D::set_debug_navigation_geometry_edge_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_geometry_edge_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_geometry_edge_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_geometry_edge_color();
}

void NavigationServer2D::set_debug_navigation_geometry_edge_disabled_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_geometry_edge_disabled_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_geometry_edge_disabled_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_geometry_edge_disabled_color();
}

void NavigationServer2D::set_debug_navigation_enable_edge_connections(const bool p_value) {
	NavigationServer3D::get_singleton()->set_debug_navigation_enable_edge_connections(p_value);
}

bool NavigationServer2D::get_debug_navigation_enable_edge_connections() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_enable_edge_connections();
}

void NavigationServer2D::set_debug_navigation_enable_geometry_face_random_color(const bool p_value) {
	NavigationServer3D::get_singleton()->set_debug_navigation_enable_geometry_face_random_color(p_value);
}

bool NavigationServer2D::get_debug_navigation_enable_geometry_face_random_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_enable_geometry_face_random_color();
}

void NavigationServer2D::set_debug_navigation_enable_edge_lines(const bool p_value) {
	NavigationServer3D::get_singleton()->set_debug_navigation_enable_edge_lines(p_value);
}

bool NavigationServer2D::get_debug_navigation_enable_edge_lines() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_enable_edge_lines();
}

void NavigationServer2D::set_debug_navigation_agent_path_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_agent_path_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_agent_path_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_agent_path_color();
}

void NavigationServer2D::set_debug_navigation_enable_agent_paths(const bool p_value) {
	NavigationServer3D::get_singleton()->set_debug_navigation_enable_agent_paths(p_value);
}

bool NavigationServer2D::get_debug_navigation_enable_agent_paths() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_enable_agent_paths();
}

void NavigationServer2D::set_debug_navigation_agent_path_point_size(real_t p_point_size) {
	NavigationServer3D::get_singleton()->set_debug_navigation_agent_path_point_size(p_point_size);
}

real_t NavigationServer2D::get_debug_navigation_agent_path_point_size() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_agent_path_point_size();
}

void NavigationServer2D::set_debug_navigation_avoidance_enable_agents_radius(const bool p_value) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_enable_agents_radius(p_value);
}

bool NavigationServer2D::get_debug_navigation_avoidance_enable_agents_radius() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_enable_agents_radius();
}

void NavigationServer2D::set_debug_navigation_avoidance_enable_obstacles_radius(const bool p_value) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_enable_obstacles_radius(p_value);
}

bool NavigationServer2D::get_debug_navigation_avoidance_enable_obstacles_radius() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_enable_obstacles_radius();
}

void NavigationServer2D::set_debug_navigation_avoidance_agents_radius_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_agents_radius_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_avoidance_agents_radius_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_agents_radius_color();
}

void NavigationServer2D::set_debug_navigation_avoidance_obstacles_radius_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_obstacles_radius_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_avoidance_obstacles_radius_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_obstacles_radius_color();
}

void NavigationServer2D::set_debug_navigation_avoidance_static_obstacle_pushin_face_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushin_face_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_avoidance_static_obstacle_pushin_face_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_face_color();
}

void NavigationServer2D::set_debug_navigation_avoidance_static_obstacle_pushout_face_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushout_face_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_avoidance_static_obstacle_pushout_face_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_face_color();
}

void NavigationServer2D::set_debug_navigation_avoidance_static_obstacle_pushin_edge_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushin_edge_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_avoidance_static_obstacle_pushin_edge_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushin_edge_color();
}

void NavigationServer2D::set_debug_navigation_avoidance_static_obstacle_pushout_edge_color(const Color &p_color) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_static_obstacle_pushout_edge_color(p_color);
}

Color NavigationServer2D::get_debug_navigation_avoidance_static_obstacle_pushout_edge_color() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_static_obstacle_pushout_edge_color();
}

void NavigationServer2D::set_debug_navigation_avoidance_enable_obstacles_static(const bool p_value) {
	NavigationServer3D::get_singleton()->set_debug_navigation_avoidance_enable_obstacles_static(p_value);
}

bool NavigationServer2D::get_debug_navigation_avoidance_enable_obstacles_static() const {
	return NavigationServer3D::get_singleton()->get_debug_navigation_avoidance_enable_obstacles_static();
}
#endif // DEBUG_ENABLED

///////////////////////////////////////////////////////

NavigationServer2DCallback NavigationServer2DManager::create_callback = nullptr;

void NavigationServer2DManager::set_default_server(NavigationServer2DCallback p_callback) {
	create_callback = p_callback;
}

NavigationServer2D *NavigationServer2DManager::new_default_server() {
	if (create_callback == nullptr) {
		return nullptr;
	}

	return create_callback();
}
#endif
