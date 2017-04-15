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
    t_pxobject f_obj;
    struct fpu f;
} t_fpu;


void *fpu_new(t_symbol *s, long argc, t_atom *argv);
void fpu_free(t_fpu *x);

void fpu_dsp64(t_lores *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void fpu_perform64(t_fpu *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);


static t_class *fpu_class;

void ext_main(void *r) {
    t_class *c = class_new("fpu~", (method)fpu_new, (method)fpu_free, sizeof(t_fpu), 0L, 0);
    class_addmethod(c, (method)fpu_dsp64, "dsp64", A_CANT, 0);
}

void *fpu_new(t_symbol *s, long argc, t_atom *argv) {
    t_fpu *x = object_alloc(fpu_class);
}

void fpu_free(t_fpu *x) {
    
}

void fpu_dsp64(t_fpu *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
    dsp_setup((t_pxobject *)x, 0);
    outlet_new((t_object *)x, "signal");
}

void fpu_perform64(t_fpu *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
}

