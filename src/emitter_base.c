#include "emitter_base.h"

static void Destroy(emitter_base_t * const base);
static void Create(emitter_base_t * const base, event_t event, uint32_t period);
static bool Emit(emitter_base_t * const base, event_t event);

extern void Emitter_Init(emitter_base_t * const base, fifo_base_t * fifo)
{
    assert(base != NULL);
    assert(base->fifo != NULL);

    static const emitter_vfunc_t vfunc =
    {
        .emit = Emit,
        .create = Create,
        .destroy = Destroy,
    };
    base->vfunc = &vfunc;
}

static bool Emit(emitter_base_t * const base, event_t event)
{
    (void)base;
    (void)event;
    return false;
}

static void Destroy(emitter_base_t * const base)
{
    (void)base;
    assert(false);
}

static void Create(emitter_base_t * const base, event_t event, uint32_t period)
{
    (void)base;
    (void)event;
    (void)period;
    assert(false);
}

