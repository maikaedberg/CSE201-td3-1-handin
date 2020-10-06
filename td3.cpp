#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double* res = new double[new_size];
  for (int i = 0; i < new_size; i++){
    if(i < length){res[i] = array[i];}
    else{res[i] = 0;}
    }
  delete[] array;
  return res;
}

double* shrink_array(double* array, int length, int new_size) {
    double* res = new double[new_size];
    for (int i = 0; i < new_size; i++){
      res[i] = array[i];}
    delete[] array;
    return res;
    }

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  if(current_size == max_size){
        array = extend_array(array, max_size, max_size + 5);
        max_size += 5;}
  array[current_size] = element;
  current_size ++;
  return array;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  if (current_size == 0){return array;}
  current_size = current_size - 1;
  if(current_size == max_size - 5){
      array = shrink_array(array, current_size, max_size - 5);
      max_size = max_size - 5;
  }

  return array;}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
      telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}
void swap(double &x, double &y){
    double temp = x;
        x = y;
        y = temp;
    }

void sort(double* array, const int size) {
    int min_id;
    for(int i = 0; i < (size / 3); i++)
        {min_id = i;
            for(int j = i + 1; j < (size/3); j++)
                {
                if (array[3*j] < array[3*min_id])
                        min_id = j;
                }
            swap(array[3*min_id], array[3*i]);
            swap(array[3*min_id + 1], array[3*i + 1]);
            swap(array[3*min_id + 2], array[3*i + 2]);

        }
    }


void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &telemetry,
                     int &telemetry_current_size,
                     int &telemetry_max_size){
    {
    for (int i = 0; i < tot_telemetries; i++) {
        for (int j = 0; j < telemetries_sizes[i]; j++){
            telemetry = append_to_array(telemetries[i][j],
                                        telemetry,
                                        telemetry_current_size,
                                        telemetry_max_size);}}
    }
    sort(telemetry, telemetry_current_size);
}
