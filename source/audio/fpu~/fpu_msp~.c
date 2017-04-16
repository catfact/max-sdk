/**
	@file
	fpu~ : max/msp wrapper for nonlinear string resonator

	@ingroup	catfact
*/

#include "ext.h"
#include "z_dsp.h"
#include "math.h"
#include "ext_buffer.h"
#include "ext_atomic.h"
#include "ext_obex.h"

#include "fpu.h"


typedef struct _fpu {
    t_pxobject obj;
    struct fpu fpu;
} t_fpu;


void *fpu_new(t_symbol *s, long argc, t_atom *argv);

void fpu_dsp64(t_fpu *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void fpu_float(t_fpu *x, double f);
void fpu_pos(t_fpu *x, int i, double f);
void fpu_clear(t_fpu *x);
void fpu_assist(t_fpu *x, void *b, long m, long a, char *s);

void fpu_perform64(t_fpu *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);


static t_class *fpu_class;

void ext_main(void *r) {
    t_class *c = class_new("fpu~", (method)fpu_new, (method)dsp_free, sizeof(t_fpu), 0L, 0);
    class_addmethod(c, (method)fpu_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)fpu_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)fpu_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)fpu_pos, "pos", A_LONG, A_FLOAT, 0);
    class_addmethod(c, (method)fpu_clear, "clear", 0);
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    fpu_class = c;
}

void *fpu_new(t_symbol *s, long argc, t_atom *argv) {
    t_fpu *x = object_alloc(fpu_class);
    dsp_setup((t_pxobject *)x, 4);
    fpu_init(&(x->fpu));
    outlet_new((t_object *)x, "signal");
    return (x);
}

void fpu_dsp64(t_fpu *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
    outlet_new((t_object *)x, "signal");
    fpu_set_sr(&x->fpu, (float)samplerate);
    dsp_add64(dsp64, (t_object *)x, (t_perfroutine64)fpu_perform64, 0, NULL);
    // post("fpu_dsp64");
}


void fpu_float(t_fpu *x, double f) {
    long in = proxy_getinlet((t_object *)x);
    switch(in) {
        case 0:
            fpu_set_beta(&x->fpu, f);
            break;
        case 1:
            fpu_set_rho(&x->fpu, f);
            break;
        case 2:
            fpu_set_epsilon(&x->fpu, f);
            break;
        case 3:
            fpu_set_pickup(&x->fpu, f);
            break;
    }
}

void fpu_pos(t_fpu *x, int n, double p) {
    post("set position: %d, %f", n, p);
    fpu_set_pos(&x->fpu, n, p);
}

void fpu_clear(t_fpu *x) {
    fpu_clear_state(&x->fpu);
}


void fpu_assist(t_fpu *x, void *b, long m, long a, char *s) {
    if (m == 2)
        sprintf(s,"(signal) output");
    else {
        switch (a) {
            case 0: sprintf(s,"(signal/float) beta (tuning)"); break;
            case 1: sprintf(s,"(signal/float) rho (damping)"); break;
            case 2: sprintf(s,"(signal/float) epsilon (nonlinearity)"); break;
            case 3: sprintf(s,"(signal/float) pickup position (0-%d", FPU_NUM_MASSES); break;
        }
    }
}

void fpu_perform64(t_fpu *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts,
                   long sampleframes, long flags, void *userparam) {
    // post("fpu_perform64");
    t_double *beta = ins[0];
    t_double *rho = ins[1];
    t_double *epsilon = ins[2];
    t_double *pickup = ins[3];
    t_double *out = outs[0];
    int n = sampleframes;
    while(n--) {
        //fpu_set_beta(&(x->fpu), *beta++);
        //fpu_set_rho(&(x->fpu), *rho++);
        //fpu_set_epsilon(&(x->fpu), *epsilon++);
        //fpu_set_pickup(&(x->fpu), *pickup++);
        fpu_update(&(x->fpu), 0.0); // input isn't used at the moment...
        *out++ = x->fpu.out;
    }
    
}

