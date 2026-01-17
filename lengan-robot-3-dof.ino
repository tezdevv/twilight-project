float target_x = 0;
float target_y = 0;
float target_z = 0;

float length_1 = 0;
float length_2 = 0;
float length_3 = 0;

int theta_1 = 0;
int theta_2 = 0;
int theta_b = 0;
int theta_r = 0;

void setup() {

}

void loop() {
  
}

void find_fk(float target_x, float target_y, float target_z, float L1, float L2, float max_len, float min_len, float* thet_1, float* thet_2, float* thet_b, float* thet_r) {
  bool state_run = true;
  float l1 = L1;
  float l2 = L2;

  float o1 = 0;
  float o2 = 0;
  float ob = 0;
  float ot = 0;

  float tx = target_x;
  float ty = target_y;
  float tz = target_z;
  float tr = 0;
  float tt = 0;

  float oR_rad = 0;
  float oR_deg = 0;
  float oT_rad = 0;
  float oT_deg = 0;

  float min_predict_o1 = 0;
  float max_predict_o1 = 90;
  float min_predict_o2 = 0;
  float max_predict_o2 = 180;

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

          float r = l1 * cos(t1) + l2 * cos(abs(t1 - t2));
          float z = l1 * sin(t1) + l2 * sin(abs(t1 - t2));

          float t = sqrt((z*z) + (r*r));
          float error = abs(t - tt);
          if (error < best_error) {
            best_error = error;
            o1 = i;
            o2 = j;
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

  *thet_1 = o1;
  *thet_2 = o2;
  *thet_b = ob;
  *thet_r = ot; 
}



