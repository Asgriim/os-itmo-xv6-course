// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.
struct {
    int refs[PHYSTOP / PGSIZE];
    struct spinlock ref_lock;
} ref_table;

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void add_ref(uint64 pa){
    ref_table.refs[pa / PGSIZE]++;
}

void dec_ref(uint64 pa) {
    ref_table.refs[pa / PGSIZE]--;
}

int get_ref_count(uint64 pa){
    return ref_table.refs[pa / PGSIZE];
}

void set_ref_val(uint64 pa, int val) {
    ref_table.refs[pa / PGSIZE] = val;
}

void hold_ref_lock() {
    acquire(&ref_table.ref_lock);
}

void release_ref_lock() {
    release(&ref_table.ref_lock);
}

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  initlock(&ref_table.ref_lock,"ref lock");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  acquire(&ref_table.ref_lock);
  dec_ref((uint64)pa);
  if (get_ref_count(pa) > 0) {
      release(&ref_table.ref_lock);
      return;
  }
  release(&ref_table.ref_lock);
  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
  {
      acquire(&ref_table.ref_lock);
      kmem.freelist = r->next;
      set_ref_val((uint64)r, 1);
      release(&ref_table.ref_lock);
  }
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
