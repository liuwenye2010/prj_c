#ifndef _LIST_H_INCLUDED_
#define _LIST_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __STATIC_INLINE 
#define __STATIC_INLINE   static inline 
#endif 

//  Double linked list.

struct list_s {
  struct list_s *prev;
  struct list_s *next;
};

int __STATIC_INLINE _list_is_empty(struct list_s * e) {
  return e->next==e;
}

void __STATIC_INLINE _list_initialize(struct list_s * e) {
  e->prev=e->next=e;
}

void __STATIC_INLINE _list_add_before(struct list_s * e,struct list_s * l) {
  e->prev=l->prev;
  e->next=l;
  l->prev->next=e;
  l->prev=e;
}

void __STATIC_INLINE _list_add_after(struct list_s * e,struct list_s * l) {
  e->next=l->next;
  e->prev=l;
  l->next->prev=e;
  l->next=e;
}

void __STATIC_INLINE _list_del(struct list_s * e) {
  e->next->prev=e->prev;
  e->prev->next=e->next;
  _list_initialize(e);
}

#ifdef __cplusplus
}
#endif

#endif 	// LIST_H_INCLUDED_
