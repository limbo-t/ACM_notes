## <p align="center"> 华东师范大学计算机科学与技术学院上机实践报告</p>

| **课程名称**：操作系统           | **年级**：2022级      | **基础实验成绩**：               |
| -------------------------------- | --------------------- | -------------------------------- |
| **指导教师**：石亮               | **姓名**：田亦海      | **扩展思考成绩：**               |
| **上机实践名称**：实验2-内存管理 | **学号**：10225101529 | **上机实践日期**：2023年10月16日 |

***



## Ⅰ 练习题







## Ⅱ 思考题



#### Q1 阅读`vmspace`的数据结构

```C
#pragma once
#include <common/list.h>
#include <common/radix.h>
#include <arch/mmu.h>
#include <machine.h>

struct vmregion {
        struct list_head node; /* vmr_list */
        vaddr_t start;
        size_t size;
        vmr_prop_t perm;
        struct pmobject *pmo;
};

struct vmspace {
        /* List head of vmregion (vmr_list) */
        struct list_head vmr_list;
        /* Root page table */
        void *pgtbl;

        u64 pcid;

        /* Heap-related: only used for user processes */
        struct vmregion *heap_vmr;
        vaddr_t user_current_heap;

        /* For the virtual address of mmap */
        vaddr_t user_current_mmap_addr;
};

typedef u64 pmo_type_t;
#define PMO_ANONYM       0 /* lazy allocation */
#define PMO_DATA         1 /* immediate allocation */
#define PMO_SHM          3 /* shared memory */
#define PMO_DEVICE       5 /* memory mapped device registers */
#define PMO_DATA_NOCACHE 6 /* non-cacheable immediate allocation */

#define PMO_FORBID 10 /* Forbidden area: avoid overflow */

struct pmobject {
        struct radix *radix; /* record physical pages */
        paddr_t start;
        size_t size;
        pmo_type_t type;
};

struct cap_group;
int create_pmo(u64 size, u64 type, struct cap_group *cap_group, struct pmobject **new_pmo);

int vmspace_init(struct vmspace *vmspace);

int vmspace_map_range(struct vmspace *vmspace, vaddr_t va, size_t len, vmr_prop_t flags, struct pmobject *pmo);

int vmspace_unmap_range(struct vmspace *vmspace, vaddr_t va, size_t len);
int unmap_pmo_in_vmspace(struct vmspace *vmspace, struct pmobject *pmo);

struct vmregion *find_vmr_for_va(struct vmspace *vmspace, vaddr_t addr);

void switch_vmspace_to(struct vmspace *);

void commit_page_to_pmo(struct pmobject *pmo, u64 index, paddr_t pa);
paddr_t get_page_from_pmo(struct pmobject *pmo, u64 index);

struct vmregion *init_heap_vmr(struct vmspace *vmspace, vaddr_t va, struct pmobject *pmo);

void kprint_vmr(struct vmspace *vmspace);

```









## Ⅲ 拓展题







## Ⅳ 总结





