
#include "Ball.h"
#include "SDL_image.h"
#include <iostream>




Ball::Ball(double radius, Room *room)
{
	room_ = room;
	radius_ = radius;

	
	v_[0] = 0;
	v_[1] = 0;

	sv_[0] = 0;
	sv_[1] = 0;

	for (int i = 0; i < 8; i++) {
		path_x[i] = radius_ + room_->left_wall_x();
		path_y[i] = radius_;
	}

	mass_ = 2; // 2kg
	coeff_of_restitution_ = 0.7;
	coeff_of_friction_ = 0.95;

	Reset();

	is_center_col = false;
	delay_ms = 200.f;
	keep_ms = delay_ms;
}

void
Ball::Reset()
{
	p_[0] = radius_ + room_->left_wall_x();
	p_[1] = radius_;

	v_[0] = 0;
	v_[1] = 0;
}

void
Ball::Launch(double initial_force_x, double initial_force_y)
{
	v_[0] = v_[0] + (initial_force_x/mass_);
	v_[1] = v_[1] + (initial_force_y/mass_);
}

void
Ball::Simulated_Launch(double initial_force_x, double initial_force_y) {
	sv_[0] = initial_force_x / mass_;
	sv_[1] = initial_force_y / mass_;
}

void 
Ball::Update(double timestep_s)
{
	double dt = timestep_s;	// seconds
	
	// 가속도
	double a[2];
	a[0] = 0;
	a[1] = room_->gravitational_acc_y();// Gravity

	// Move
	p_[0] = p_[0] + dt * v_[0];
	p_[1] = p_[1] + dt * v_[1];


	// Collision with Ground
	if ( p_[1]-radius_ < room_->ground_height() && v_[1] < 0 ) 
	{
		p_[1] = radius_ + room_->ground_height();

		v_[1] = -1*v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];

		//Friction
		v_[0] *= coeff_of_friction_;
		
		if (v_[0] < 0.001f && v_[0] > -0.001f)
			v_[0] = 0.f;
	}

	// Collision with Ceilling
	else if ( p_[1]+radius_ > room_->height() && v_[1] > 0 ) 
	{
		p_[1] = room_->height() - radius_;

		v_[1] = -1*v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
	}

	// Collision with Left Wall
	if ( p_[0] - radius_ < room_->left_wall_x() && v_[0] < 0 )
	{
		p_[0] = room_->left_wall_x() + radius_;

		v_[0] = -1 * v_[0];

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	// Collision with Right Wall
	else if (p_[0] + radius_ > room_->right_wall_x() && v_[0] > 0)
	{
		p_[0] = room_->right_wall_x() - radius_;

		v_[0] = -1 * v_[0];

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	// Collision with Center Wall
	if (!is_center_col) {
		if (p_[1] - radius_ < room_->vertiacal_fence_height() && p_[0] > room_->vertiacal_fence_pos_x() - radius_ && p_[0] < room_->vertiacal_fence_pos_x() + radius_) {
			double cos_ = v_[0] / sqrt(v_[0] * v_[0] + v_[1] * v_[1]);
			double sin_ = v_[1] / sqrt(v_[0] * v_[0] + v_[1] * v_[1]);

			if (v_[0] > 0)
				v_[0] = -cos_ * v_[0];
			else
				v_[0] = cos_ * v_[0];

			if (v_[1] > 0)
				v_[1] = -sin_ * v_[1];
			else
				v_[1] = sin_ * v_[1];

			v_[0] = coeff_of_restitution_ * v_[0];
			v_[1] = coeff_of_restitution_ * v_[1];

			is_center_col = true;
		}
	}
	else {
		keep_ms -= 33.f;
		if (keep_ms < 0) {
			is_center_col = false;
			keep_ms = delay_ms;
		}
	}

	//속도 = 이전속도 + 시간(dt) * 가속도;
	v_[0] = v_[0] + dt * a[0];
	v_[1] = v_[1] + dt * a[1];

	// Moving Path
	for (int i = 0; i < 8; i++) {
		double path_t = (double) (i + 1) / 8;
		path_x[i] = radius_ + room_->left_wall_x() + sv_[0] * path_t;
		path_y[i] = radius_ + sv_[1] * path_t + 0.5 * a[1] * path_t * path_t;
	}
}