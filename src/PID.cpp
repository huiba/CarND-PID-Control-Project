#include "PID.h"
#include <limits>
#include <iostream>

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  K[0] = Kp_;
  K[1] = Ki_;
  K[2] = Kd_;
  p_error = 0;
  i_error = 0;
  d_error = 0;
  last_cte = -1;
  next_param_id = 0;
  better_err = true;
  full_update = false;
  best_cte = std::numeric_limits<double>::max();
}
void PID::UpdateParam(double cte, double tol)
{
  //twiddle
  double sum = 0;
  for (int i =0; i < 3; ++i) {
    sum += dk[i];
  }
  if (sum > tol || (next_param_id) % 3 == 0) {
    if (better_err) {
      K[next_param_id] += dk[next_param_id];
      if (cte < best_cte) {
        best_cte = cte;
        dk[next_param_id] *= 1.1;
        next_param_id = (next_param_id + 1) % 3;
        better_err = true;
      } 
      else {
        K[next_param_id] -= 2 * dk[next_param_id];
        better_err = false;
      }
    }
    else {
      if (cte < best_cte) {
        best_cte = cte;
        dk[next_param_id] *= 1.1;
      }
      else {
        K[next_param_id] += dk[next_param_id];
        dk[next_param_id] *= 0.9;
      }
      next_param_id = (next_param_id + 1) % 3;
    }
  }
  
  std::cout << "idx: " << next_param_id << std::endl;

  std::cout << "Kp: " << K[0] << std::endl;
  std::cout << "Ki: " << K[1] << std::endl;
  std::cout << "Kd: " << K[2] << std::endl;

  std::cout << "Kpd: " << dk[0] << std::endl;
  std::cout << "Kid: " << dk[1] << std::endl;
  std::cout << "Kdd: " << dk[2] << std::endl;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  if (last_cte == -1) {
    last_cte = cte;
  } 
  p_error = cte;
  d_error = cte - last_cte;
  i_error += cte;
  last_cte = cte;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return K[0] * p_error + K[1] * i_error + K[2] * d_error;
}