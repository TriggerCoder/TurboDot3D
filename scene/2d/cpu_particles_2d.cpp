/**************************************************************************/
/*  cpu_particles_2d.cpp                                                  */
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
#include "cpu_particles_2d.h"

#include "scene/2d/gpu_particles_2d.h"
#include "scene/resources/atlas_texture.h"
#include "scene/resources/curve_texture.h"
#include "scene/resources/gradient_texture.h"
#include "scene/resources/particle_process_material.h"

void CPUParticles2D::set_emitting(bool p_emitting) {
	if (emitting == p_emitting) {
		return;
	}

	emitting = p_emitting;
	if (emitting) {
		active = true;
		set_process_internal(true);
	}
}

void CPUParticles2D::set_amount(int p_amount) {
	ERR_FAIL_COND_MSG(p_amount < 1, "Amount of particles must be greater than 0.");

	particles.resize(p_amount);
	{
		Particle *w = particles.ptrw();

		for (int i = 0; i < p_amount; i++) {
			w[i].active = false;
		}
	}

	particle_data.resize((8 + 4 + 4) * p_amount);
	RS::get_singleton()->multimesh_allocate_data(multimesh, p_amount, RS::MULTIMESH_TRANSFORM_2D, true, true);

	particle_order.resize(p_amount);
}

void CPUParticles2D::set_lifetime(double p_lifetime) {
	ERR_FAIL_COND_MSG(p_lifetime <= 0, "Particles lifetime must be greater than 0.");
	lifetime = p_lifetime;
}

void CPUParticles2D::set_one_shot(bool p_one_shot) {
	one_shot = p_one_shot;
}

void CPUParticles2D::set_pre_process_time(double p_time) {
	pre_process_time = p_time;
}

void CPUParticles2D::set_explosiveness_ratio(real_t p_ratio) {
	explosiveness_ratio = p_ratio;
}

void CPUParticles2D::set_randomness_ratio(real_t p_ratio) {
	randomness_ratio = p_ratio;
}

void CPUParticles2D::set_lifetime_randomness(double p_random) {
	lifetime_randomness = p_random;
}

void CPUParticles2D::set_use_local_coordinates(bool p_enable) {
	local_coords = p_enable;
	set_notify_transform(!p_enable);
}

void CPUParticles2D::set_speed_scale(double p_scale) {
	speed_scale = p_scale;
}

bool CPUParticles2D::is_emitting() const {
	return emitting;
}

int CPUParticles2D::get_amount() const {
	return particles.size();
}

double CPUParticles2D::get_lifetime() const {
	return lifetime;
}

bool CPUParticles2D::get_one_shot() const {
	return one_shot;
}

double CPUParticles2D::get_pre_process_time() const {
	return pre_process_time;
}

real_t CPUParticles2D::get_explosiveness_ratio() const {
	return explosiveness_ratio;
}

real_t CPUParticles2D::get_randomness_ratio() const {
	return randomness_ratio;
}

double CPUParticles2D::get_lifetime_randomness() const {
	return lifetime_randomness;
}

bool CPUParticles2D::get_use_local_coordinates() const {
	return local_coords;
}

double CPUParticles2D::get_speed_scale() const {
	return speed_scale;
}

void CPUParticles2D::set_draw_order(DrawOrder p_order) {
	draw_order = p_order;
}

CPUParticles2D::DrawOrder CPUParticles2D::get_draw_order() const {
	return draw_order;
}

void CPUParticles2D::_update_mesh_texture() {
	Size2 tex_size;
	if (texture.is_valid()) {
		tex_size = texture->get_size();
	} else {
		tex_size = Size2(1, 1);
	}

	Vector<Vector2> vertices = {
		-tex_size * 0.5,
		-tex_size * 0.5 + Vector2(tex_size.x, 0),
		-tex_size * 0.5 + tex_size,
		-tex_size * 0.5 + Vector2(0, tex_size.y)
	};

	Vector<Vector2> uvs;
	AtlasTexture *atlas_texure = Object::cast_to<AtlasTexture>(*texture);
	if (atlas_texure && atlas_texure->get_atlas().is_valid()) {
		Rect2 region_rect = atlas_texure->get_region();
		Size2 atlas_size = atlas_texure->get_atlas()->get_size();
		uvs.push_back(Vector2(region_rect.position.x / atlas_size.x, region_rect.position.y / atlas_size.y));
		uvs.push_back(Vector2((region_rect.position.x + region_rect.size.x) / atlas_size.x, region_rect.position.y / atlas_size.y));
		uvs.push_back(Vector2((region_rect.position.x + region_rect.size.x) / atlas_size.x, (region_rect.position.y + region_rect.size.y) / atlas_size.y));
		uvs.push_back(Vector2(region_rect.position.x / atlas_size.x, (region_rect.position.y + region_rect.size.y) / atlas_size.y));
	} else {
		uvs.push_back(Vector2(0, 0));
		uvs.push_back(Vector2(1, 0));
		uvs.push_back(Vector2(1, 1));
		uvs.push_back(Vector2(0, 1));
	}

	Vector<Color> colors = {
		Color(1, 1, 1, 1),
		Color(1, 1, 1, 1),
		Color(1, 1, 1, 1),
		Color(1, 1, 1, 1)
	};

	Vector<int> indices = { 0, 1, 2, 2, 3, 0 };

	Array arr;
	arr.resize(RS::ARRAY_MAX);
	arr[RS::ARRAY_VERTEX] = vertices;
	arr[RS::ARRAY_TEX_UV] = uvs;
	arr[RS::ARRAY_COLOR] = colors;
	arr[RS::ARRAY_INDEX] = indices;

	RS::get_singleton()->mesh_clear(mesh);
	RS::get_singleton()->mesh_add_surface_from_arrays(mesh, RS::PRIMITIVE_TRIANGLES, arr);
}

void CPUParticles2D::set_texture(const Ref<Texture2D> &p_texture) {
	if (p_texture == texture) {
		return;
	}

	if (texture.is_valid()) {
		texture->disconnect_changed(callable_mp(this, &CPUParticles2D::_texture_changed));
	}

	texture = p_texture;

	if (texture.is_valid()) {
		texture->connect_changed(callable_mp(this, &CPUParticles2D::_texture_changed));
	}

	queue_redraw();
	_update_mesh_texture();
}

void CPUParticles2D::_texture_changed() {
	if (texture.is_valid()) {
		queue_redraw();
		_update_mesh_texture();
	}
}

Ref<Texture2D> CPUParticles2D::get_texture() const {
	return texture;
}

void CPUParticles2D::set_fixed_fps(int p_count) {
	fixed_fps = p_count;
}

int CPUParticles2D::get_fixed_fps() const {
	return fixed_fps;
}

void CPUParticles2D::set_fractional_delta(bool p_enable) {
	fractional_delta = p_enable;
}

bool CPUParticles2D::get_fractional_delta() const {
	return fractional_delta;
}

PackedStringArray CPUParticles2D::get_configuration_warnings() const {
	PackedStringArray warnings = Node2D::get_configuration_warnings();

	CanvasItemMaterial *mat = Object::cast_to<CanvasItemMaterial>(get_material().ptr());

	if (get_material().is_null() || (mat && !mat->get_particles_animation())) {
		if (get_param_max(PARAM_ANIM_SPEED) != 0.0 || get_param_max(PARAM_ANIM_OFFSET) != 0.0 ||
				get_param_curve(PARAM_ANIM_SPEED).is_valid() || get_param_curve(PARAM_ANIM_OFFSET).is_valid()) {
			warnings.push_back(RTR("CPUParticles2D animation requires the usage of a CanvasItemMaterial with \"Particles Animation\" enabled."));
		}
	}

	return warnings;
}

void CPUParticles2D::restart() {
	time = 0;
	frame_remainder = 0;
	cycle = 0;
	emitting = false;

	{
		int pc = particles.size();
		Particle *w = particles.ptrw();

		for (int i = 0; i < pc; i++) {
			w[i].active = false;
		}
	}

	set_emitting(true);
}

void CPUParticles2D::set_direction(Vector2 p_direction) {
	direction = p_direction;
}

Vector2 CPUParticles2D::get_direction() const {
	return direction;
}

void CPUParticles2D::set_spread(real_t p_spread) {
	spread = p_spread;
}

real_t CPUParticles2D::get_spread() const {
	return spread;
}

void CPUParticles2D::set_param_min(Parameter p_param, real_t p_value) {
	ERR_FAIL_INDEX(p_param, PARAM_MAX);

	parameters_min[p_param] = p_value;
	if (parameters_min[p_param] > parameters_max[p_param]) {
		set_param_max(p_param, p_value);
	}
}

real_t CPUParticles2D::get_param_min(Parameter p_param) const {
	ERR_FAIL_INDEX_V(p_param, PARAM_MAX, 0);

	return parameters_min[p_param];
}

void CPUParticles2D::set_param_max(Parameter p_param, real_t p_value) {
	ERR_FAIL_INDEX(p_param, PARAM_MAX);

	parameters_max[p_param] = p_value;
	if (parameters_min[p_param] > parameters_max[p_param]) {
		set_param_min(p_param, p_value);
	}

	update_configuration_warnings();
}

real_t CPUParticles2D::get_param_max(Parameter p_param) const {
	ERR_FAIL_INDEX_V(p_param, PARAM_MAX, 0);

	return parameters_max[p_param];
}

static void _adjust_curve_range(const Ref<Curve> &p_curve, real_t p_min, real_t p_max) {
	Ref<Curve> curve = p_curve;
	if (!curve.is_valid()) {
		return;
	}

	curve->ensure_default_setup(p_min, p_max);
}

void CPUParticles2D::set_param_curve(Parameter p_param, const Ref<Curve> &p_curve) {
	ERR_FAIL_INDEX(p_param, PARAM_MAX);

	curve_parameters[p_param] = p_curve;

	switch (p_param) {
		case PARAM_INITIAL_LINEAR_VELOCITY: {
			//do none for this one
		} break;
		case PARAM_ANGULAR_VELOCITY: {
			_adjust_curve_range(p_curve, -360, 360);
		} break;
		case PARAM_ORBIT_VELOCITY: {
			_adjust_curve_range(p_curve, -500, 500);
		} break;
		case PARAM_LINEAR_ACCEL: {
			_adjust_curve_range(p_curve, -200, 200);
		} break;
		case PARAM_RADIAL_ACCEL: {
			_adjust_curve_range(p_curve, -200, 200);
		} break;
		case PARAM_TANGENTIAL_ACCEL: {
			_adjust_curve_range(p_curve, -200, 200);
		} break;
		case PARAM_DAMPING: {
			_adjust_curve_range(p_curve, 0, 100);
		} break;
		case PARAM_ANGLE: {
			_adjust_curve_range(p_curve, -360, 360);
		} break;
		case PARAM_SCALE: {
		} break;
		case PARAM_HUE_VARIATION: {
			_adjust_curve_range(p_curve, -1, 1);
		} break;
		case PARAM_ANIM_SPEED: {
			_adjust_curve_range(p_curve, 0, 200);
		} break;
		case PARAM_ANIM_OFFSET: {
		} break;
		default: {
		}
	}

	update_configuration_warnings();
}

Ref<Curve> CPUParticles2D::get_param_curve(Parameter p_param) const {
	ERR_FAIL_INDEX_V(p_param, PARAM_MAX, Ref<Curve>());

	return curve_parameters[p_param];
}

void CPUParticles2D::set_color(const Color &p_color) {
	color = p_color;
}

Color CPUParticles2D::get_color() const {
	return color;
}

void CPUParticles2D::set_color_ramp(const Ref<Gradient> &p_ramp) {
	color_ramp = p_ramp;
}

Ref<Gradient> CPUParticles2D::get_color_ramp() const {
	return color_ramp;
}

void CPUParticles2D::set_color_initial_ramp(const Ref<Gradient> &p_ramp) {
	color_initial_ramp = p_ramp;
}

Ref<Gradient> CPUParticles2D::get_color_initial_ramp() const {
	return color_initial_ramp;
}

void CPUParticles2D::set_particle_flag(ParticleFlags p_particle_flag, bool p_enable) {
	ERR_FAIL_INDEX(p_particle_flag, PARTICLE_FLAG_MAX);
	particle_flags[p_particle_flag] = p_enable;
}

bool CPUParticles2D::get_particle_flag(ParticleFlags p_particle_flag) const {
	ERR_FAIL_INDEX_V(p_particle_flag, PARTICLE_FLAG_MAX, false);
	return particle_flags[p_particle_flag];
}

void CPUParticles2D::set_emission_shape(EmissionShape p_shape) {
	ERR_FAIL_INDEX(p_shape, EMISSION_SHAPE_MAX);
	emission_shape = p_shape;
	notify_property_list_changed();
}

void CPUParticles2D::set_emission_sphere_radius(real_t p_radius) {
	emission_sphere_radius = p_radius;
}

void CPUParticles2D::set_emission_rect_extents(Vector2 p_extents) {
	emission_rect_extents = p_extents;
}

void CPUParticles2D::set_emission_points(const Vector<Vector2> &p_points) {
	emission_points = p_points;
}

void CPUParticles2D::set_emission_normals(const Vector<Vector2> &p_normals) {
	emission_normals = p_normals;
}

void CPUParticles2D::set_emission_colors(const Vector<Color> &p_colors) {
	emission_colors = p_colors;
}

real_t CPUParticles2D::get_emission_sphere_radius() const {
	return emission_sphere_radius;
}

Vector2 CPUParticles2D::get_emission_rect_extents() const {
	return emission_rect_extents;
}

Vector<Vector2> CPUParticles2D::get_emission_points() const {
	return emission_points;
}

Vector<Vector2> CPUParticles2D::get_emission_normals() const {
	return emission_normals;
}

Vector<Color> CPUParticles2D::get_emission_colors() const {
	return emission_colors;
}

CPUParticles2D::EmissionShape CPUParticles2D::get_emission_shape() const {
	return emission_shape;
}

void CPUParticles2D::set_gravity(const Vector2 &p_gravity) {
	gravity = p_gravity;
}

Vector2 CPUParticles2D::get_gravity() const {
	return gravity;
}

void CPUParticles2D::set_scale_curve_x(Ref<Curve> p_scale_curve) {
	scale_curve_x = p_scale_curve;
}

void CPUParticles2D::set_scale_curve_y(Ref<Curve> p_scale_curve) {
	scale_curve_y = p_scale_curve;
}

void CPUParticles2D::set_split_scale(bool p_split_scale) {
	split_scale = p_split_scale;
	notify_property_list_changed();
}

Ref<Curve> CPUParticles2D::get_scale_curve_x() const {
	return scale_curve_x;
}

Ref<Curve> CPUParticles2D::get_scale_curve_y() const {
	return scale_curve_y;
}

bool CPUParticles2D::get_split_scale() {
	return split_scale;
}

void CPUParticles2D::_validate_property(PropertyInfo &p_property) const {
	if (p_property.name == "emission_sphere_radius" && (emission_shape != EMISSION_SHAPE_SPHERE && emission_shape != EMISSION_SHAPE_SPHERE_SURFACE)) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (p_property.name == "emission_rect_extents" && emission_shape != EMISSION_SHAPE_RECTANGLE) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if ((p_property.name == "emission_point_texture" || p_property.name == "emission_color_texture") && (emission_shape < EMISSION_SHAPE_POINTS)) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (p_property.name == "emission_normals" && emission_shape != EMISSION_SHAPE_DIRECTED_POINTS) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (p_property.name == "emission_points" && emission_shape != EMISSION_SHAPE_POINTS && emission_shape != EMISSION_SHAPE_DIRECTED_POINTS) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (p_property.name == "emission_colors" && emission_shape != EMISSION_SHAPE_POINTS && emission_shape != EMISSION_SHAPE_DIRECTED_POINTS) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}
	if (p_property.name.begins_with("scale_curve_") && !split_scale) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}
}

static uint32_t idhash(uint32_t x) {
	x = ((x >> uint32_t(16)) ^ x) * uint32_t(0x45d9f3b);
	x = ((x >> uint32_t(16)) ^ x) * uint32_t(0x45d9f3b);
	x = (x >> uint32_t(16)) ^ x;
	return x;
}

static real_t rand_from_seed(uint32_t &seed) {
	int k;
	int s = int(seed);
	if (s == 0) {
		s = 305420679;
	}
	k = s / 127773;
	s = 16807 * (s - k * 127773) - 2836 * k;
	if (s < 0) {
		s += 2147483647;
	}
	seed = uint32_t(s);
	return (seed % uint32_t(65536)) / 65535.0;
}

void CPUParticles2D::_update_internal() {
	if (particles.size() == 0 || !is_visible_in_tree()) {
		_set_do_redraw(false);
		return;
	}

	double delta = get_process_delta_time();
	if (!active && !emitting) {
		set_process_internal(false);
		_set_do_redraw(false);

		//reset variables
		time = 0;
		frame_remainder = 0;
		cycle = 0;
		return;
	}
	_set_do_redraw(true);

	if (time == 0 && pre_process_time > 0.0) {
		double frame_time;
		if (fixed_fps > 0) {
			frame_time = 1.0 / fixed_fps;
		} else {
			frame_time = 1.0 / 30.0;
		}

		double todo = pre_process_time;

		while (todo >= 0) {
			_particles_process(frame_time);
			todo -= frame_time;
		}
	}

	if (fixed_fps > 0) {
		double frame_time = 1.0 / fixed_fps;
		double decr = frame_time;

		double ldelta = delta;
		if (ldelta > 0.1) { //avoid recursive stalls if fps goes below 10
			ldelta = 0.1;
		} else if (ldelta <= 0.0) { //unlikely but..
			ldelta = 0.001;
		}
		double todo = frame_remainder + ldelta;

		while (todo >= frame_time) {
			_particles_process(frame_time);
			todo -= decr;
		}

		frame_remainder = todo;

	} else {
		_particles_process(delta);
	}

	_update_particle_data_buffer();
}

void CPUParticles2D::_particles_process(double p_delta) {
	p_delta *= speed_scale;

	int pcount = particles.size();
	Particle *w = particles.ptrw();

	Particle *parray = w;

	double prev_time = time;
	time += p_delta;
	if (time > lifetime) {
		time = Math::fmod(time, lifetime);
		cycle++;
		if (one_shot && cycle > 0) {
			set_emitting(false);
			notify_property_list_changed();
		}
	}

	Transform2D emission_xform;
	Transform2D velocity_xform;
	if (!local_coords) {
		emission_xform = get_global_transform();
		velocity_xform = emission_xform;
		velocity_xform[2] = Vector2();
	}

	double system_phase = time / lifetime;

	bool should_be_active = false;
	for (int i = 0; i < pcount; i++) {
		Particle &p = parray[i];

		if (!emitting && !p.active) {
			continue;
		}

		double local_delta = p_delta;

		// The phase is a ratio between 0 (birth) and 1 (end of life) for each particle.
		// While we use time in tests later on, for randomness we use the phase as done in the
		// original shader code, and we later multiply by lifetime to get the time.
		double restart_phase = double(i) / double(pcount);

		if (randomness_ratio > 0.0) {
			uint32_t seed = cycle;
			if (restart_phase >= system_phase) {
				seed -= uint32_t(1);
			}
			seed *= uint32_t(pcount);
			seed += uint32_t(i);
			double random = double(idhash(seed) % uint32_t(65536)) / 65536.0;
			restart_phase += randomness_ratio * random * 1.0 / double(pcount);
		}

		restart_phase *= (1.0 - explosiveness_ratio);
		double restart_time = restart_phase * lifetime;
		bool restart = false;

		if (time > prev_time) {
			// restart_time >= prev_time is used so particles emit in the first frame they are processed

			if (restart_time >= prev_time && restart_time < time) {
				restart = true;
				if (fractional_delta) {
					local_delta = time - restart_time;
				}
			}

		} else if (local_delta > 0.0) {
			if (restart_time >= prev_time) {
				restart = true;
				if (fractional_delta) {
					local_delta = lifetime - restart_time + time;
				}

			} else if (restart_time < time) {
				restart = true;
				if (fractional_delta) {
					local_delta = time - restart_time;
				}
			}
		}

		if (p.time * (1.0 - explosiveness_ratio) > p.lifetime) {
			restart = true;
		}

		float tv = 0.0;

		if (restart) {
			if (!emitting) {
				p.active = false;
				continue;
			}
			p.active = true;

			/*real_t tex_linear_velocity = 0;
			if (curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY].is_valid()) {
				tex_linear_velocity = curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY]->sample(0);
			}*/

			real_t tex_angle = 1.0;
			if (curve_parameters[PARAM_ANGLE].is_valid()) {
				tex_angle = curve_parameters[PARAM_ANGLE]->sample(tv);
			}

			real_t tex_anim_offset = 1.0;
			if (curve_parameters[PARAM_ANGLE].is_valid()) {
				tex_anim_offset = curve_parameters[PARAM_ANGLE]->sample(tv);
			}

			p.seed = Math::rand();

			p.angle_rand = Math::randf();
			p.scale_rand = Math::randf();
			p.hue_rot_rand = Math::randf();
			p.anim_offset_rand = Math::randf();

			if (color_initial_ramp.is_valid()) {
				p.start_color_rand = color_initial_ramp->get_color_at_offset(Math::randf());
			} else {
				p.start_color_rand = Color(1, 1, 1, 1);
			}

			real_t angle1_rad = direction.angle() + Math::deg_to_rad((Math::randf() * 2.0 - 1.0) * spread);
			Vector2 rot = Vector2(Math::cos(angle1_rad), Math::sin(angle1_rad));
			p.velocity = rot * Math::lerp(parameters_min[PARAM_INITIAL_LINEAR_VELOCITY], parameters_max[PARAM_INITIAL_LINEAR_VELOCITY], (real_t)Math::randf());

			real_t base_angle = tex_angle * Math::lerp(parameters_min[PARAM_ANGLE], parameters_max[PARAM_ANGLE], p.angle_rand);
			p.rotation = Math::deg_to_rad(base_angle);

			p.custom[0] = 0.0; // unused
			p.custom[1] = 0.0; // phase [0..1]
			p.custom[2] = tex_anim_offset * Math::lerp(parameters_min[PARAM_ANIM_OFFSET], parameters_max[PARAM_ANIM_OFFSET], p.anim_offset_rand);
			p.custom[3] = (1.0 - Math::randf() * lifetime_randomness);
			p.transform = Transform2D();
			p.time = 0;
			p.lifetime = lifetime * p.custom[3];
			p.base_color = Color(1, 1, 1, 1);

			switch (emission_shape) {
				case EMISSION_SHAPE_POINT: {
					//do none
				} break;
				case EMISSION_SHAPE_SPHERE: {
					real_t t = Math_TAU * Math::randf();
					real_t radius = emission_sphere_radius * Math::randf();
					p.transform[2] = Vector2(Math::cos(t), Math::sin(t)) * radius;
				} break;
				case EMISSION_SHAPE_SPHERE_SURFACE: {
					real_t s = Math::randf(), t = Math_TAU * Math::randf();
					real_t radius = emission_sphere_radius * Math::sqrt(1.0 - s * s);
					p.transform[2] = Vector2(Math::cos(t), Math::sin(t)) * radius;
				} break;
				case EMISSION_SHAPE_RECTANGLE: {
					p.transform[2] = Vector2(Math::randf() * 2.0 - 1.0, Math::randf() * 2.0 - 1.0) * emission_rect_extents;
				} break;
				case EMISSION_SHAPE_POINTS:
				case EMISSION_SHAPE_DIRECTED_POINTS: {
					int pc = emission_points.size();
					if (pc == 0) {
						break;
					}

					int random_idx = Math::rand() % pc;

					p.transform[2] = emission_points.get(random_idx);

					if (emission_shape == EMISSION_SHAPE_DIRECTED_POINTS && emission_normals.size() == pc) {
						Vector2 normal = emission_normals.get(random_idx);
						Transform2D m2;
						m2.columns[0] = normal;
						m2.columns[1] = normal.orthogonal();
						p.velocity = m2.basis_xform(p.velocity);
					}

					if (emission_colors.size() == pc) {
						p.base_color = emission_colors.get(random_idx);
					}
				} break;
				case EMISSION_SHAPE_MAX: { // Max value for validity check.
					break;
				}
			}

			if (!local_coords) {
				p.velocity = velocity_xform.xform(p.velocity);
				p.transform = emission_xform * p.transform;
			}

		} else if (!p.active) {
			continue;
		} else if (p.time > p.lifetime) {
			p.active = false;
			tv = 1.0;
		} else {
			uint32_t alt_seed = p.seed;

			p.time += local_delta;
			p.custom[1] = p.time / lifetime;
			tv = p.time / p.lifetime;

			real_t tex_linear_velocity = 1.0;
			if (curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY].is_valid()) {
				tex_linear_velocity = curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY]->sample(tv);
			}

			real_t tex_orbit_velocity = 1.0;
			if (curve_parameters[PARAM_ORBIT_VELOCITY].is_valid()) {
				tex_orbit_velocity = curve_parameters[PARAM_ORBIT_VELOCITY]->sample(tv);
			}

			real_t tex_angular_velocity = 1.0;
			if (curve_parameters[PARAM_ANGULAR_VELOCITY].is_valid()) {
				tex_angular_velocity = curve_parameters[PARAM_ANGULAR_VELOCITY]->sample(tv);
			}

			real_t tex_linear_accel = 1.0;
			if (curve_parameters[PARAM_LINEAR_ACCEL].is_valid()) {
				tex_linear_accel = curve_parameters[PARAM_LINEAR_ACCEL]->sample(tv);
			}

			real_t tex_tangential_accel = 1.0;
			if (curve_parameters[PARAM_TANGENTIAL_ACCEL].is_valid()) {
				tex_tangential_accel = curve_parameters[PARAM_TANGENTIAL_ACCEL]->sample(tv);
			}

			real_t tex_radial_accel = 1.0;
			if (curve_parameters[PARAM_RADIAL_ACCEL].is_valid()) {
				tex_radial_accel = curve_parameters[PARAM_RADIAL_ACCEL]->sample(tv);
			}

			real_t tex_damping = 1.0;
			if (curve_parameters[PARAM_DAMPING].is_valid()) {
				tex_damping = curve_parameters[PARAM_DAMPING]->sample(tv);
			}

			real_t tex_angle = 1.0;
			if (curve_parameters[PARAM_ANGLE].is_valid()) {
				tex_angle = curve_parameters[PARAM_ANGLE]->sample(tv);
			}
			real_t tex_anim_speed = 1.0;
			if (curve_parameters[PARAM_ANIM_SPEED].is_valid()) {
				tex_anim_speed = curve_parameters[PARAM_ANIM_SPEED]->sample(tv);
			}

			real_t tex_anim_offset = 1.0;
			if (curve_parameters[PARAM_ANIM_OFFSET].is_valid()) {
				tex_anim_offset = curve_parameters[PARAM_ANIM_OFFSET]->sample(tv);
			}

			Vector2 force = gravity;
			Vector2 pos = p.transform[2];

			//apply linear acceleration
			force += p.velocity.length() > 0.0 ? p.velocity.normalized() * tex_linear_accel * Math::lerp(parameters_min[PARAM_LINEAR_ACCEL], parameters_max[PARAM_LINEAR_ACCEL], rand_from_seed(alt_seed)) : Vector2();
			//apply radial acceleration
			Vector2 org = emission_xform[2];
			Vector2 diff = pos - org;
			force += diff.length() > 0.0 ? diff.normalized() * (tex_radial_accel)*Math::lerp(parameters_min[PARAM_RADIAL_ACCEL], parameters_max[PARAM_RADIAL_ACCEL], rand_from_seed(alt_seed)) : Vector2();
			//apply tangential acceleration;
			Vector2 yx = Vector2(diff.y, diff.x);
			force += yx.length() > 0.0 ? (yx * Vector2(-1.0, 1.0)).normalized() * (tex_tangential_accel * Math::lerp(parameters_min[PARAM_TANGENTIAL_ACCEL], parameters_max[PARAM_TANGENTIAL_ACCEL], rand_from_seed(alt_seed))) : Vector2();
			//apply attractor forces
			p.velocity += force * local_delta;
			//orbit velocity
			real_t orbit_amount = tex_orbit_velocity * Math::lerp(parameters_min[PARAM_ORBIT_VELOCITY], parameters_max[PARAM_ORBIT_VELOCITY], rand_from_seed(alt_seed));
			if (orbit_amount != 0.0) {
				real_t ang = orbit_amount * local_delta * Math_TAU;
				// Not sure why the ParticleProcessMaterial code uses a clockwise rotation matrix,
				// but we use -ang here to reproduce its behavior.
				Transform2D rot = Transform2D(-ang, Vector2());
				p.transform[2] -= diff;
				p.transform[2] += rot.basis_xform(diff);
			}
			if (curve_parameters[PARAM_INITIAL_LINEAR_VELOCITY].is_valid()) {
				p.velocity = p.velocity.normalized() * tex_linear_velocity;
			}

			if (parameters_max[PARAM_DAMPING] + tex_damping > 0.0) {
				real_t v = p.velocity.length();
				real_t damp = tex_damping * Math::lerp(parameters_min[PARAM_DAMPING], parameters_max[PARAM_DAMPING], rand_from_seed(alt_seed));
				v -= damp * local_delta;
				if (v < 0.0) {
					p.velocity = Vector2();
				} else {
					p.velocity = p.velocity.normalized() * v;
				}
			}
			real_t base_angle = (tex_angle)*Math::lerp(parameters_min[PARAM_ANGLE], parameters_max[PARAM_ANGLE], p.angle_rand);
			base_angle += p.custom[1] * lifetime * tex_angular_velocity * Math::lerp(parameters_min[PARAM_ANGULAR_VELOCITY], parameters_max[PARAM_ANGULAR_VELOCITY], rand_from_seed(alt_seed));
			p.rotation = Math::deg_to_rad(base_angle); //angle
			p.custom[2] = tex_anim_offset * Math::lerp(parameters_min[PARAM_ANIM_OFFSET], parameters_max[PARAM_ANIM_OFFSET], p.anim_offset_rand) + tv * tex_anim_speed * Math::lerp(parameters_min[PARAM_ANIM_SPEED], parameters_max[PARAM_ANIM_SPEED], rand_from_seed(alt_seed));
		}
		//apply color
		//apply hue rotation

		Vector2 tex_scale = Vector2(1.0, 1.0);
		if (split_scale) {
			if (scale_curve_x.is_valid()) {
				tex_scale.x = scale_curve_x->sample(tv);
			} else {
				tex_scale.x = 1.0;
			}
			if (scale_curve_y.is_valid()) {
				tex_scale.y = scale_curve_y->sample(tv);
			} else {
				tex_scale.y = 1.0;
			}
		} else {
			if (curve_parameters[PARAM_SCALE].is_valid()) {
				real_t tmp_scale = curve_parameters[PARAM_SCALE]->sample(tv);
				tex_scale.x = tmp_scale;
				tex_scale.y = tmp_scale;
			}
		}

		real_t tex_hue_variation = 0.0;
		if (curve_parameters[PARAM_HUE_VARIATION].is_valid()) {
			tex_hue_variation = curve_parameters[PARAM_HUE_VARIATION]->sample(tv);
		}

		real_t hue_rot_angle = (tex_hue_variation)*Math_TAU * Math::lerp(parameters_min[PARAM_HUE_VARIATION], parameters_max[PARAM_HUE_VARIATION], p.hue_rot_rand);
		real_t hue_rot_c = Math::cos(hue_rot_angle);
		real_t hue_rot_s = Math::sin(hue_rot_angle);

		Basis hue_rot_mat;
		{
			Basis mat1(0.299, 0.587, 0.114, 0.299, 0.587, 0.114, 0.299, 0.587, 0.114);
			Basis mat2(0.701, -0.587, -0.114, -0.299, 0.413, -0.114, -0.300, -0.588, 0.886);
			Basis mat3(0.168, 0.330, -0.497, -0.328, 0.035, 0.292, 1.250, -1.050, -0.203);

			for (int j = 0; j < 3; j++) {
				hue_rot_mat[j] = mat1[j] + mat2[j] * hue_rot_c + mat3[j] * hue_rot_s;
			}
		}

		if (color_ramp.is_valid()) {
			p.color = color_ramp->get_color_at_offset(tv) * color;
		} else {
			p.color = color;
		}

		Vector3 color_rgb = hue_rot_mat.xform_inv(Vector3(p.color.r, p.color.g, p.color.b));
		p.color.r = color_rgb.x;
		p.color.g = color_rgb.y;
		p.color.b = color_rgb.z;

		p.color *= p.base_color * p.start_color_rand;

		if (particle_flags[PARTICLE_FLAG_ALIGN_Y_TO_VELOCITY]) {
			if (p.velocity.length() > 0.0) {
				p.transform.columns[1] = p.velocity.normalized();
				p.transform.columns[0] = p.transform.columns[1].orthogonal();
			}

		} else {
			p.transform.columns[0] = Vector2(Math::cos(p.rotation), -Math::sin(p.rotation));
			p.transform.columns[1] = Vector2(Math::sin(p.rotation), Math::cos(p.rotation));
		}

		//scale by scale
		Vector2 base_scale = tex_scale * Math::lerp(parameters_min[PARAM_SCALE], parameters_max[PARAM_SCALE], p.scale_rand);
		if (base_scale.x < 0.00001) {
			base_scale.x = 0.00001;
		}
		if (base_scale.y < 0.00001) {
			base_scale.y = 0.00001;
		}
		p.transform.columns[0] *= base_scale.x;
		p.transform.columns[1] *= base_scale.y;

		p.transform[2] += p.velocity * local_delta;

		should_be_active = true;
	}
	if (!Math::is_equal_approx(time, 0.0) && active && !should_be_active) {
		active = false;
		emit_signal(SceneStringName(finished));
	}
}

void CPUParticles2D::_update_particle_data_buffer() {
	MutexLock lock(update_mutex);

	int pc = particles.size();

	int *ow;
	int *order = nullptr;

	float *w = particle_data.ptrw();
	const Particle *r = particles.ptr();
	float *ptr = w;

	if (draw_order != DRAW_ORDER_INDEX) {
		ow = particle_order.ptrw();
		order = ow;

		for (int i = 0; i < pc; i++) {
			order[i] = i;
		}
		if (draw_order == DRAW_ORDER_LIFETIME) {
			SortArray<int, SortLifetime> sorter;
			sorter.compare.particles = r;
			sorter.sort(order, pc);
		}
	}

	for (int i = 0; i < pc; i++) {
		int idx = order ? order[i] : i;

		Transform2D t = r[idx].transform;

		if (!local_coords) {
			t = inv_emission_transform * t;
		}

		if (r[idx].active) {
			ptr[0] = t.columns[0][0];
			ptr[1] = t.columns[1][0];
			ptr[2] = 0;
			ptr[3] = t.columns[2][0];
			ptr[4] = t.columns[0][1];
			ptr[5] = t.columns[1][1];
			ptr[6] = 0;
			ptr[7] = t.columns[2][1];

		} else {
			memset(ptr, 0, sizeof(float) * 8);
		}

		Color c = r[idx].color;

		ptr[8] = c.r;
		ptr[9] = c.g;
		ptr[10] = c.b;
		ptr[11] = c.a;

		ptr[12] = r[idx].custom[0];
		ptr[13] = r[idx].custom[1];
		ptr[14] = r[idx].custom[2];
		ptr[15] = r[idx].custom[3];

		ptr += 16;
	}
}

void CPUParticles2D::_set_do_redraw(bool p_do_redraw) {
	if (do_redraw == p_do_redraw) {
		return;
	}
	do_redraw = p_do_redraw;

	{
		MutexLock lock(update_mutex);

		if (do_redraw) {
			RS::get_singleton()->connect("frame_pre_draw", callable_mp(this, &CPUParticles2D::_update_render_thread));
			RS::get_singleton()->canvas_item_set_update_when_visible(get_canvas_item(), true);

			RS::get_singleton()->multimesh_set_visible_instances(multimesh, -1);
		} else {
			if (RS::get_singleton()->is_connected("frame_pre_draw", callable_mp(this, &CPUParticles2D::_update_render_thread))) {
				RS::get_singleton()->disconnect("frame_pre_draw", callable_mp(this, &CPUParticles2D::_update_render_thread));
			}
			RS::get_singleton()->canvas_item_set_update_when_visible(get_canvas_item(), false);

			RS::get_singleton()->multimesh_set_visible_instances(multimesh, 0);
		}
	}

	queue_redraw(); // redraw to update render list
}

void CPUParticles2D::_update_render_thread() {
	MutexLock lock(update_mutex);

	RS::get_singleton()->multimesh_set_buffer(multimesh, particle_data);
}

void CPUParticles2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			set_process_internal(emitting);
		} break;

		case NOTIFICATION_EXIT_TREE: {
			_set_do_redraw(false);
		} break;

		case NOTIFICATION_DRAW: {
			// first update before rendering to avoid one frame delay after emitting starts
			if (emitting && (time == 0)) {
				_update_internal();
			}

			if (!do_redraw) {
				return; // don't add to render list
			}

			RID texrid;
			if (texture.is_valid()) {
				texrid = texture->get_rid();
			}

			RS::get_singleton()->canvas_item_add_multimesh(get_canvas_item(), multimesh, texrid);
		} break;

		case NOTIFICATION_INTERNAL_PROCESS: {
			_update_internal();
		} break;

		case NOTIFICATION_TRANSFORM_CHANGED: {
			inv_emission_transform = get_global_transform().affine_inverse();

			if (!local_coords) {
				int pc = particles.size();

				float *w = particle_data.ptrw();
				const Particle *r = particles.ptr();
				float *ptr = w;

				for (int i = 0; i < pc; i++) {
					Transform2D t = inv_emission_transform * r[i].transform;

					if (r[i].active) {
						ptr[0] = t.columns[0][0];
						ptr[1] = t.columns[1][0];
						ptr[2] = 0;
						ptr[3] = t.columns[2][0];
						ptr[4] = t.columns[0][1];
						ptr[5] = t.columns[1][1];
						ptr[6] = 0;
						ptr[7] = t.columns[2][1];

					} else {
						memset(ptr, 0, sizeof(float) * 8);
					}

					ptr += 16;
				}
			}
		} break;
	}
}

void CPUParticles2D::convert_from_particles(Node *p_particles) {
	GPUParticles2D *gpu_particles = Object::cast_to<GPUParticles2D>(p_particles);
	ERR_FAIL_NULL_MSG(gpu_particles, "Only GPUParticles2D nodes can be converted to CPUParticles2D.");

	set_emitting(gpu_particles->is_emitting());
	set_amount(gpu_particles->get_amount());
	set_lifetime(gpu_particles->get_lifetime());
	set_one_shot(gpu_particles->get_one_shot());
	set_pre_process_time(gpu_particles->get_pre_process_time());
	set_explosiveness_ratio(gpu_particles->get_explosiveness_ratio());
	set_randomness_ratio(gpu_particles->get_randomness_ratio());
	set_use_local_coordinates(gpu_particles->get_use_local_coordinates());
	set_fixed_fps(gpu_particles->get_fixed_fps());
	set_fractional_delta(gpu_particles->get_fractional_delta());
	set_speed_scale(gpu_particles->get_speed_scale());
	set_draw_order(DrawOrder(gpu_particles->get_draw_order()));
	set_texture(gpu_particles->get_texture());

	Ref<Material> mat = gpu_particles->get_material();
	if (mat.is_valid()) {
		set_material(mat);
	}

	Ref<ParticleProcessMaterial> proc_mat = gpu_particles->get_process_material();
	if (proc_mat.is_null()) {
		return;
	}

	Vector3 dir = proc_mat->get_direction();
	set_direction(Vector2(dir.x, dir.y));
	set_spread(proc_mat->get_spread());

	set_color(proc_mat->get_color());

	Ref<GradientTexture1D> gt = proc_mat->get_color_ramp();
	if (gt.is_valid()) {
		set_color_ramp(gt->get_gradient());
	}

	Ref<GradientTexture1D> gti = proc_mat->get_color_initial_ramp();
	if (gti.is_valid()) {
		set_color_initial_ramp(gti->get_gradient());
	}

	set_particle_flag(PARTICLE_FLAG_ALIGN_Y_TO_VELOCITY, proc_mat->get_particle_flag(ParticleProcessMaterial::PARTICLE_FLAG_ALIGN_Y_TO_VELOCITY));

	set_emission_shape(EmissionShape(proc_mat->get_emission_shape()));
	set_emission_sphere_radius(proc_mat->get_emission_sphere_radius());
	Vector2 rect_extents = Vector2(proc_mat->get_emission_box_extents().x, proc_mat->get_emission_box_extents().y);
	set_emission_rect_extents(rect_extents);

	Ref<CurveXYZTexture> scale3D = proc_mat->get_param_texture(ParticleProcessMaterial::PARAM_SCALE);
	if (scale3D.is_valid()) {
		split_scale = true;
		scale_curve_x = scale3D->get_curve_x();
		scale_curve_y = scale3D->get_curve_y();
	}
	set_gravity(Vector2(proc_mat->get_gravity().x, proc_mat->get_gravity().y));
	set_lifetime_randomness(proc_mat->get_lifetime_randomness());

#define CONVERT_PARAM(m_param)                                                                  \
	set_param_min(m_param, proc_mat->get_param_min(ParticleProcessMaterial::m_param));          \
	{                                                                                           \
		Ref<CurveTexture> ctex = proc_mat->get_param_texture(ParticleProcessMaterial::m_param); \
		if (ctex.is_valid())                                                                    \
			set_param_curve(m_param, ctex->get_curve());                                        \
	}                                                                                           \
	set_param_max(m_param, proc_mat->get_param_max(ParticleProcessMaterial::m_param));

	CONVERT_PARAM(PARAM_INITIAL_LINEAR_VELOCITY);
	CONVERT_PARAM(PARAM_ANGULAR_VELOCITY);
	CONVERT_PARAM(PARAM_ORBIT_VELOCITY);
	CONVERT_PARAM(PARAM_LINEAR_ACCEL);
	CONVERT_PARAM(PARAM_RADIAL_ACCEL);
	CONVERT_PARAM(PARAM_TANGENTIAL_ACCEL);
	CONVERT_PARAM(PARAM_DAMPING);
	CONVERT_PARAM(PARAM_ANGLE);
	CONVERT_PARAM(PARAM_SCALE);
	CONVERT_PARAM(PARAM_HUE_VARIATION);
	CONVERT_PARAM(PARAM_ANIM_SPEED);
	CONVERT_PARAM(PARAM_ANIM_OFFSET);

#undef CONVERT_PARAM
}

void CPUParticles2D::_bind_methods() {
}

CPUParticles2D::CPUParticles2D() {
	mesh = RenderingServer::get_singleton()->mesh_create();
	multimesh = RenderingServer::get_singleton()->multimesh_create();
	RenderingServer::get_singleton()->multimesh_set_mesh(multimesh, mesh);

	set_emitting(true);
	set_amount(8);
	set_use_local_coordinates(false);

	set_param_min(PARAM_INITIAL_LINEAR_VELOCITY, 0);
	set_param_min(PARAM_ANGULAR_VELOCITY, 0);
	set_param_min(PARAM_ORBIT_VELOCITY, 0);
	set_param_min(PARAM_LINEAR_ACCEL, 0);
	set_param_min(PARAM_RADIAL_ACCEL, 0);
	set_param_min(PARAM_TANGENTIAL_ACCEL, 0);
	set_param_min(PARAM_DAMPING, 0);
	set_param_min(PARAM_ANGLE, 0);
	set_param_min(PARAM_SCALE, 1);
	set_param_min(PARAM_HUE_VARIATION, 0);
	set_param_min(PARAM_ANIM_SPEED, 0);
	set_param_min(PARAM_ANIM_OFFSET, 0);

	set_param_max(PARAM_INITIAL_LINEAR_VELOCITY, 0);
	set_param_max(PARAM_ANGULAR_VELOCITY, 0);
	set_param_max(PARAM_ORBIT_VELOCITY, 0);
	set_param_max(PARAM_LINEAR_ACCEL, 0);
	set_param_max(PARAM_RADIAL_ACCEL, 0);
	set_param_max(PARAM_TANGENTIAL_ACCEL, 0);
	set_param_max(PARAM_DAMPING, 0);
	set_param_max(PARAM_ANGLE, 0);
	set_param_max(PARAM_SCALE, 1);
	set_param_max(PARAM_HUE_VARIATION, 0);
	set_param_max(PARAM_ANIM_SPEED, 0);
	set_param_max(PARAM_ANIM_OFFSET, 0);

	for (int i = 0; i < PARTICLE_FLAG_MAX; i++) {
		particle_flags[i] = false;
	}

	set_color(Color(1, 1, 1, 1));

	_update_mesh_texture();
}

CPUParticles2D::~CPUParticles2D() {
	ERR_FAIL_NULL(RenderingServer::get_singleton());
	RS::get_singleton()->free(multimesh);
	RS::get_singleton()->free(mesh);
}
#endif
