#include "m_pd.h"  
 
/*************** saw~ ***************/
static t_class *scalarsaw_class;  
 
typedef struct _saw {  
  t_object  x_obj;  
  t_float f_osc_frequency;
} t_saw;  
 
void saw_bang(t_saw *x)  
{  
  post("Hello world!");  
}  
 
void *saw_new(void)  
{  
  t_saw *x = (t_saw *)pd_new(scalarsaw_class);  
  
  return (void *)x;  
}  
 
void saw_setup(void) {  
  scalarsaw_class = class_new(gensym("saw"),  
        (t_newmethod)saw_new,  
        0, sizeof(t_saw),  
        CLASS_DEFAULT, 0);  
  class_addbang(scalarsaw_class, saw_bang);  
}

/*************** square~ ***************/
static t_class *scalarsquare_class;  
 
typedef struct _square {  
  t_object  x_obj;  
  t_float f_osc_frequency;
  t_float f_duty_cycle;
  t_float f_phase;
} t_square;  
 
void square_bang(t_square *x)  
{  
  post("Hello world!");  
}  
 
void *square_new(void)  
{  
  t_square *x = (t_square *)pd_new(scalarsquare_class);  
  
  return (void *)x;  
}  
 
void square_setup(void) {  
  scalarsquare_class = class_new(gensym("square"),  
        (t_newmethod)square_new,  
        0, sizeof(t_square),  
        CLASS_DEFAULT, 0);  
  class_addbang(scalarsquare_class, square_bang);  
}

