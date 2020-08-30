/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature BLUE_BALL = vex::vision::signature (1, -3457, -2201, -2829, 7663, 11119, 9391, 3, 0);
vex::vision::signature RED_BALL = vex::vision::signature (2, 5267, 7735, 6501, -943, 1, -471, 3, 0);
vex::vision::signature SIG_3 = vex::vision::signature (3, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature NOTHING = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision vs = vex::vision (vex::PORT1, 50, BLUE_BALL, RED_BALL, SIG_3, SIG_4, SIG_5, SIG_6, NOTHING);
/*vex-vision-config:end*/