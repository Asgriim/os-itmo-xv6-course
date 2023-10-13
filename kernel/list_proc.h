//
// Created by asgrim on 12.10.23.
//

#ifndef XV6_RISCV_LIST_PROC_H
#define XV6_RISCV_LIST_PROC_H
struct proc_list {
    struct proc_list *next;
    struct proc *p;
};


int list_empty(struct proc_list *root_node);


int push_plist(struct proc_list *root_node, struct proc_list *node);

int delete_plist(struct proc_list *root_node, struct proc_list *node);

struct proc_list* delete_plist_e(struct proc_list *root_node, struct proc *p);

#endif //XV6_RISCV_LIST_PROC_H
