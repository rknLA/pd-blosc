#include "m_pd.h"  

#define UNITBIT32 1572864.  /* 3*2^19; bit 32 has place value 1 */


#if defined(__FreeBSD__) || defined(__APPLE__) || defined(__FreeBSD_kernel__) \
    || defined(__OpenBSD__)
#include <machine/endian.h>
#endif

#if defined(__linux__) || defined(__CYGWIN__) || defined(__GNU__) || \
    defined(ANDROID)
#include <endian.h>
#endif

#ifdef __MINGW32__
#include <sys/param.h>
#endif

#ifdef _MSC_VER
/* _MSVC lacks BYTE_ORDER and LITTLE_ENDIAN */
#define LITTLE_ENDIAN 0x0001
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#if !defined(BYTE_ORDER) || !defined(LITTLE_ENDIAN) 
#error No byte order defined                                                    
#endif

#if BYTE_ORDER == LITTLE_ENDIAN
# define HIOFFSET 1                                                              
# define LOWOFFSET 0                                                             
#else                                                                           
# define HIOFFSET 0    /* word offset to find MSB */
# define LOWOFFSET 1    /* word offset to find LSB */
#endif

union tabfudge
{
    double tf_d;
    int32_t tf_i[2];
};

/* The above ifdef and tabfudge comes from the d_osc.c file in the pd source */
 
/*************** saw~ ***************/
static t_class *saw_tilde_class;
 
typedef struct _saw_tilde {
  t_object x_obj;
  t_float x_freq;
  t_float x_conv;
  double x_phase;
  double x_last_phase;
  t_outlet *x_out;
} t_saw_tilde;

double polyblep_sample(double phase, double phase_step)
{
    double t;
    if (phase < phase_step) {
        t = phase / phase_step;
        return (t * 2) - (t * t) - 1.0;
    }

    if (phase > (1.0 - phase_step)) {
        t = (phase - 1.0) / phase_step;
        return (t * t) + (t * 2) + 1.0;
    }

    return 0.0;
}

t_int *saw_tilde_perform(t_int *w)
{
    t_saw_tilde *x = (t_saw_tilde *)(w[1]);
    /*
     * the osc frequency is automatically promoted to a signal, so we
     * pull it from *in_freq instead of from x_freq.  this is to allow
     * signal-rate frequency changes.
     */
    t_sample *in_freq =  (t_sample *)(w[2]);
    t_sample *out_osc = (t_sample *)(w[3]);
    int num_samples = (int)(w[4]);

    double conv = x->x_conv;
    double last_phase = x->x_last_phase;
    double curr_phase = x->x_phase;
    double out_val;
    double phase_step;

    while (num_samples--)
    {
        /* (osc freq / sample rate) */
        phase_step = *in_freq++ * conv;
        out_val = (1.0 - (2.0 * curr_phase));
        out_val += polyblep_sample(curr_phase, phase_step);
        *out_osc++ = out_val;
        last_phase = curr_phase;
        curr_phase += phase_step;
        if (curr_phase > 1.0) {
            curr_phase -= 1.0;
        }
    }
    x->x_phase = curr_phase;
    x->x_last_phase = last_phase;
    return (w+5);
}

void saw_tilde_dsp(t_saw_tilde *x, t_signal **sp)
{
    x->x_conv = 1./sp[0]->s_sr;
    dsp_add(saw_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
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
  x->x_last_phase = 0;
  return (void *)x;  
}  
 
void saw_tilde_setup(void)
{
  saw_tilde_class = class_new(gensym("saw~"),
        (t_newmethod)saw_tilde_new,
        (t_method)saw_tilde_free,
        sizeof(t_saw_tilde),
        CLASS_DEFAULT,
        A_DEFFLOAT,
        0);
  class_addmethod(saw_tilde_class,
          (t_method)saw_tilde_dsp,
          gensym("dsp"),
          A_CANT,
          0);
  CLASS_MAINSIGNALIN(saw_tilde_class, t_saw_tilde, x_freq);
}
