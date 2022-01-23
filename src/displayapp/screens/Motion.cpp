#include "displayapp/screens/Motion.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

Motion::Motion(Pinetime::Applications::DisplayApp* app, Controllers::MotionController& motionController)
  : Screen(app), motionController {motionController} {
  chart = lv_chart_create(lv_scr_act(), NULL);
  lv_obj_set_size(chart, 240, 240);
  lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/
  // lv_chart_set_series_opa(chart, LV_OPA_70);                            /*Opacity of the data series*/
  // lv_chart_set_series_width(chart, 4);                                  /*Line width and point radious*/

  lv_chart_set_range(chart, -1100, 1100);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_point_count(chart, 10);

  /*Add 3 data series*/
  ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
  ser2 = lv_chart_add_series(chart, LV_COLOR_GREEN);
  ser3 = lv_chart_add_series(chart, LV_COLOR_YELLOW);

  lv_chart_init_points(chart, ser1, 0);
  lv_chart_init_points(chart, ser2, 0);
  lv_chart_init_points(chart, ser3, 0);
  lv_chart_refresh(chart); /*Required after direct set*/

  label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text_fmt(label, "X #FF0000 %d# Y #008000 %d# Z #FFFF00 %d#", 0, 0, 0);
  lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
  lv_label_set_recolor(label, true);

  labelStep = lv_label_create(lv_scr_act(), NULL);
  lv_obj_align(labelStep, chart, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
  lv_label_set_text(labelStep, "Steps ---");

    
  labelAcc = lv_label_create(lv_scr_act(), NULL);
  lv_obj_align(labelAcc, chart, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
  lv_label_set_text(labelStep, "ACC ---");
    
  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

Motion::~Motion() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void Motion::Refresh() {
/*
  double ggnorm = 64*64;
  double SMV = sqrt(motionController.X()/0x10*motionController.X()/0x10/ggnorm + motionController.Y()/0x10*motionController.Y()/0x10/ggnorm + motionController.Z()/0x10*motionController.Z()/0x10/ggnorm);
  uint16_t intSMV = (unsigned int)round(100*SMV);

  int16_t myX = motionController.X();
  int16_t myY = motionController.Y();
  int16_t myZ = motionController.Z();

  int16_t myX = motionController.X() / 0x10;
  int16_t myY = motionController.Y() / 0x10;
  int16_t myZ = motionController.Z() / 0x10;
*/
  int16_t accNorm = 1024;

  int16_t myX = 100 * motionController.X() / accNorm;
  int16_t myY = 100 * motionController.Y() / accNorm;
  int16_t myZ = 100 * motionController.Z() / accNorm;
  
  int16_t vectorSUM = abs(myX) + abs(myY) + abs(myZ) ;

  double SMV = sqrt(myX * myX + myY * myY + myZ * myZ ); 
  int16_t intSMV = (int)round(100*SMV);
  
/*  lv_chart_set_next(chart, ser1, motionController.X()); */
  lv_chart_set_next(chart, ser1, myX);
  lv_chart_set_next(chart, ser2, myY);
  lv_chart_set_next(chart, ser3, myZ);
  
  
/*
  lv_label_set_text_fmt(labelStep, "Steps %lu", motionController.NbSteps());
  
  lv_label_set_text_fmt(labelAcc, "vecSUM %lu", vectorSUM);
*/  

  lv_label_set_text_fmt(labelStep, "VSum %d intSMV %d", vectorSum, intSMV);
  
/*
  lv_label_set_text_fmt(label,
                        "X #FF0000 %d# Y #008000 %d# Z #FFFF00 %d#",
                        motionController.X() / 0x10,
                        motionController.Y() / 0x10,
                        motionController.Z() / 0x10);
                        
*/ 

  lv_label_set_text_fmt(label,"X #FF0000 %d# Y #008000 %d# Z #FFFF00 %d#", myX, myY, myZ); 
    
  lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
}
