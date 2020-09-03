/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature BLUE_BALL = vex::vision::signature (1, -3295, -2081, -2688, 3295, 5535, 4414, 1.8, 0);
vex::vision::signature RED_BALL = vex::vision::signature (2, 3455, 7009, 5232, -1023, 1, -510, 1.9, 0);
vex::vision::signature SIG_3 = vex::vision::signature (3, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature NOTHING = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision vs = vex::vision (vex::PORT1, 50, BLUE_BALL, RED_BALL, SIG_3, SIG_4, SIG_5, SIG_6, NOTHING);
/*vex-vision-config:end*/