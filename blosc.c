#include "m_pd.h"  
 
/*************** saw~ ***************/
static t_class *saw_tilde_class;
 
typedef struct _saw_tilde {
  t_object x_obj;
  t_float x_freq;
  t_float x_conv;
  double x_phase;
  t_outlet *x_out;
} t_saw_tilde;

t_int *saw_tilde_perform(t_int *w)
{
    t_saw_tilde *x = (t_saw_tilde *)(w[1]);
    t_sample *in =  (t_sample *)(w[2]);
    t_sample *out = (t_sample *)(w[3]);

    return (w+4);
}

void saw_tilde_dsp(t_saw_tilde *x, t_signal **sp)
{
    dsp_add(saw_tilde_perform, 5, x,
            sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void saw_tilde_free(t_saw_tilde *x)
{
    outlet_free(x->x_out);
}
 
void *saw_tilde_new(t_floatarg f)
{  
  t_saw_tilde  *x = (t_saw_tilde  *)pd_new(saw_tilde_class);
  x->x_freq = f;
  x->x_out = outlet_new(&x->x_obj, &s_signal);
  x->x_conv = 0;
  x->x_phase = 0;
  return (void *)x;  
}  
 
void saw_tilde_setup(void)
{
  saw_tilde_class = class_new(gensym("saw~"),
        (t_newmethod)saw_tilde_new,
        (t_method)saw_tilde_free,
        sizeof(t_saw_tilde),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);
  class_addmethod(saw_tilde_class,
          (t_method)saw_tilde_dsp, gensym("dsp"), 0);
  CLASS_MAINSIGNALIN(saw_tilde_class, t_saw_tilde, x_freq);
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

