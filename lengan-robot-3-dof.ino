// #include <Servo.h>

//perintah
int param_do = 12;
int step_do = 5;

// |target x|target y|target z|len 1|len 2|max 3|min len|ob|o1|o2|or|mode(get(0)\put(1))| 
float [12][5] = {
  {x, x, x, x, x, x, x, x, x, x, x, x},
  {x, x, x, x, x, x, x, x, x, x, x, x},
  {x, x, x, x, x, x, x, x, x, x, x, x},
  {x, x, x, x, x, x, x, x, x, x, x, x},
  {x, x, x, x, x, x, x, x, x, x, x, x}
};

// Servo sob, so1, so2, sc;
float target_x = 7; //cm
float target_y = 7; //cm
float target_z = 2; //cm

float len_1 = 10; //cm
float len_2 = 10; //cm

float maximal_len = 20;
float minimal_len = 0;

float theta_1 = 0;
float theta_2 = 0;
float theta_b = 0;
float theta_r = 0;
float sc_open = 90;
float sc_close = 0;

bool run_find = 1;
bool mode_do = 0;

void setup() {
  Serial.begin(9600);

  // sob.attach(2);
  // sob.attach(3);
  // sob.attach(4);
  // sob.attach(5);
}

void loop() {
  find_fk(target_x, target_y, target_z, len_1, len_2, maximal_len, minimal_len, &theta_b, &theta_1, &theta_2, &theta_r);

  // if (run_find) {
  //   find_fk(target_x, target_y, target_z, len_1, len_2, maximal_len, minimal_len, theta_b, theta_1, theta_2, theta_r);
  //   servo_get_target();
  //   find_fk(target_x, target_y, target_z, len_1, len_2, maximal_len, minimal_len, theta_b, theta_1, theta_2, theta_r);
  //   servo_put_target();
  //   run_find = 0;
  // }
}

// void servo_get_target() {
//   //close servo (ob = 0, o1 = 90, o2 = 0) and open crab
//   so2.write(0);
//   delay(200);
//   so1.write(90);
//   delay(200);
//   sob.write(0);
//   delay(200);
//   sc.write(sc_open);
//   delay(200);
//   //rotation servo ob
//   sob.write(theta_r);
//   delay(200);
//   //rotation servo o1 + or
//   so1.write(theta_1 + theta_r);
//   delay(200);
//   //rotation servo o2
//   so2.write(theta_2);
//   delay(200);
//   //close crab
//   sc.write(sc_close);
//   delay(200);
// }

// void servo_put_target() {
//   //close servo (ob = 0, o1 = 90, o2 = 0) and still close crab
//   so2.write(0);
//   delay(200);
//   so1.write(90);
//   delay(200);
//   sob.write(0);
//   delay(200);
//   sc.write(sc_close);
//   delay(200);
//   //rotation servo ob
//   sob.write(theta_r);
//   delay(200);
//   //rotation servo o1 + or
//   so1.write(theta_1);
//   delay(200);
//   //rotation servo o2
//   so2.write(theta_2);
//   delay(200);
//   //open crab
//   sc.write(sc_open);
//   delay(200);
//   //back
//   so2.write(0);
//   delay(200);
//   so1.write(90);
//   delay(200);
//   sob.write(0);
//   delay(200);
//   sc.write(sc_close);
//   delay(200);
// }

void find_fk(float target_X, float target_Y, float target_Z, float L1, float L2, float max_len, float min_len, float* thet_b, float* thet_1, float* thet_2, float* thet_r) {
  bool state_run = true;
  float l1 = L1;
  float l2 = L2;

  float pos_r = 0;
  float pos_z = 0;

  float o1 = 0;
  float o2 = 0;
  float ob = 0;
  float ot = 0;
  float oF = 0;

  float tx = target_X;
  float ty = target_Y;
  float tz = target_Z;
  float tr = 0;
  float tt = 0;

  float oR_rad = 0;
  float oR_deg = 0;
  float oT_rad = 0;
  float oT_deg = 0;
  float oF_rad = 0;
  float oF_deg = 0;

  float min_predict_o1 = 0;
  float max_predict_o1 = 90;
  float min_predict_o2 = 180;
  float max_predict_o2 = 360;

  float max_l = max_len;
  float min_l = min_len;

  oR_rad = atan2(ty, tx);
  oR_deg = degrees(oR_rad);
  ob = oR_deg;
  tr = sqrt((ty * ty) + (tx * tx));

  oT_rad = atan2(tz, tr);
  oT_deg = degrees(oT_rad);
  ot = oT_deg;
  tt = sqrt((tz * tz) + (tr * tr));

  Serial.println("done");

  if (tt > max_l || tt < min_l) {
    state_run = false;
    return;
  }
  
  float best_error = 99999;
  int interval[5] = {30, 15, 10, 5, 1};

  if (state_run) {
    for (int k = 0; k < 5; k++) {
      for (int i = min_predict_o1 ; i <= max_predict_o1; i += interval[k]) {
        for (int j = min_predict_o2; j <= max_predict_o2; j += interval[k]) {
          float t1 = radians(i);
          float t2 = radians(j);

          float r = l1 * cos(t1) + l2 * cos(t1 + t2);
          float z = l1 * sin(t1) + l2 * sin(t1 + t2);

          float t = sqrt((z*z) + (r*r));
          float error = abs(t - tt);
          if (r >= 0 && z >= 0) {
            if (error < best_error) {
              best_error = error;
              o1 = i;
              o2 = j;
              pos_r = r;
              pos_z = z;
            }
          }
        }
      }

      if (best_error < 0.5) break;

      max_predict_o1 = o1 + interval[k];
      max_predict_o2 = o2 + interval[k];
      min_predict_o1 = o1 - interval[k];
      min_predict_o2 = o2 - interval[k];

      if (max_predict_o1 >= 90) {
        max_predict_o1 = 90;
      }
      if (max_predict_o2 >= 180) {
        max_predict_o2 = 180;
      }
      if (min_predict_o1 <= 0) {
        min_predict_o1 = 0;
      }
      if (min_predict_o2 <= 0) {
        min_predict_o2 = 0;
      }
    }
  }

  for (int i = o1; i >= 0; i--) {
    if (pos_z < 0.5) {
      o1 = i;
      break;
    }
    pos_r = l1 * cos(radians(i)) + l2 * cos(radians(i) + radians(o2));
    pos_z = l1 * sin(radians(i)) + l2 * sin(radians(i) + radians(o2));
  }

  *thet_1 = ot;
  *thet_2 = o2;
  *thet_b = ob;
  *thet_r = ot;

  float normalize_o2 = 360 - o2;

  Serial.print(" ot : "); 
  Serial.print(ot); 
  Serial.print(" ob : "); 
  Serial.print(ob); 
  Serial.print(" ob1 : "); 
  Serial.print(o1); 
  Serial.print(" ob2 : "); 
  Serial.print(o2); 
  Serial.print(" obr : "); 
  Serial.println(ot); 
}



