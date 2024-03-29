#pragma once

#include "SDL.h"
#include "Room.h"


class Ball
{
public:
	Ball(double radius, Room* room);

	void Reset();
	virtual void Update(double timestep_s);
	void Launch(double initial_force_x, double initial_force_y);


	void velocity(double x, double y) { v_[0] = x; v_[1] = y; }
	double radius() const { return radius_; }
	double mass() const { return mass_; }
	double coeff_of_restitution() { return coeff_of_restitution_; }

	double pos_x() const { return p_[0]; }
	double pos_y() const { return p_[1]; }

	void Simulated_Launch(double initial_force_x, double initial_force_y);
	double* get_path_x() { return path_x; }
	double* get_path_y() { return path_y; }
	double sv_x() const { return sv_[0]; }
	double sv_y() const { return sv_[1]; }

protected:
	Room* room_;
	
	double radius_;	// meter
	double mass_;	// kg
	double coeff_of_restitution_; // from 0 o 1
	double coeff_of_friction_; // from 0 o 1

	// position
	double p_[2];	// x, y

	// velocity
	double v_[2];	// x, y

	// simulated velocity
	double sv_[2];	// x, y

	// moving path
	double path_x[8];
	double path_y[8];

	bool is_center_col;
	float delay_ms, keep_ms;
};

