#ifndef EMITTER_BASE_H
#define EMITTER_BASE_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "state.h"
#include "fifo_base.h"

typedef struct emitter_vfunc_t emitter_vfunc_t;
typedef struct
{
    emitter_vfunc_t const * vfunc;
    fifo_base_t * fifo;
}
emitter_base_t;

struct emitter_vfunc_t
{
    bool (*emit)(emitter_base_t * const base, event_t event);
    void (*create)(emitter_base_t * const base, event_t event, uint32_t period);
    void (*destroy)(emitter_base_t * const base);
};

extern void Emitter_Init(emitter_base_t * const base, fifo_base_t * const fifo);

inline static bool Emitter_Emit( emitter_base_t * const base, event_t event )
{
    assert( base != NULL );
    assert( base->vfunc != NULL);
    assert( base->vfunc->emit != NULL);
    return (base->vfunc->emit)(base, event);
}

inline static void Emitter_Create( emitter_base_t * const base, event_t event, uint32_t period)
{
    assert( base != NULL ); 
    assert( base->vfunc != NULL);
    assert( base->vfunc->create != NULL);
    (base->vfunc->create)(base, event, period);
}

inline static void Emitter_Destroy( emitter_base_t * const base, event_t event )
{
    assert( base != NULL );
    assert( base->vfunc != NULL);
    assert( base->vfunc->destroy != NULL);
    (base->vfunc->destroy)(base);
}

#endif /* EMITTER_BASE_H_ */
