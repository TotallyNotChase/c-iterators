#ifndef IT_ITERATOR_H
#define IT_ITERATOR_H

#include "maybe.h"
#include "typeclass.h"

#define DeclareIteratorOf(ElmntTypename)                                                                               \
    typeclass(Maybe(ElmntTypename) (*next)(void* self, void* ctx); void* ctx) ElmntTypename##Iterator

#define Iterator(ElmntTypename) ElmntTypename##Iterator

#define impl_iterator(ItrType, ElmntTypename, CtxType, next_f, Name)                                                   \
    ElmntTypename##Iterator Name(ItrType* x, CtxType* init_ctx)                                                        \
    {                                                                                                                  \
        Maybe(ElmntTypename) (*const next_)(ItrType * self, CtxType * ctx) = (next_f);                                 \
        return (ElmntTypename##Iterator){                                                                              \
            .next = (Maybe(ElmntTypename)(*)(void*, void*))next_, .ctx = init_ctx, .self = x};                         \
    }

#endif /* !IT_ITERATOR_H */
