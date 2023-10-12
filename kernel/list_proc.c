//
// Created by asgrim on 12.10.23.
//

#include "list_proc.h"
int list_empty(struct proc_list *root_node) {
    return root_node == 0;
}

int push_plist(struct proc_list *root_node, struct proc_list *node){
    node->next = 0;
    struct proc_list *end = root_node;
    while (end->next != 0) {
        end = end->next;
    }
    end->next = node;
    return 0;
}

int delete_plist(struct proc_list *root_node, struct proc_list *node) {
    if (list_empty(root_node)) {
        return -1;
    }
    if (root_node == node) {
        root_node = 0;
        return 0;
    }
    struct proc_list *end = root_node;
    while (end->next != node) {
        end = end->next;
    }
    end->next = node->next;
    return 0;
}

int delete_plist_e(struct proc_list *root_node, struct proc *p) {
    if (list_empty(root_node)) {
        return -1;
    }

    struct proc_list *end = root_node;
    while (end->next != 0) {
        if (end->next->p == p) {
            end->next = end->next->next;
            return 0;
        }
        end = end->next;
    }

    return -1;
}