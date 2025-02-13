#include "stdio.h";

typedef enum color {RED, BLACK} color;

typedef struct node {
  int data;
  node *left;
  node *right;
  node *parent;
  color color;
} node;

node* insert_bst(node *root, node *new) {
  if (root == NULL) return new;
  if (new->data < root->data) {
    if (root->left == NULL) {
      root->left = new;
      new->parent = root;
    } else {
      insert_bst(root->left, new);
    }
  } else {
    if (root->right == NULL) {
      root->right = new;
      new->parent = root;
    } else {
      insert_bst(root->right, new);
    }
  }
  return root;
}

node* get_sibling(node *el) {
  if (el->parent == NULL) {
    return NULL;
  } else {
    if (el < el->parent->data) {
      return el->parent->right;
    } else {
      return el->parent->left;
    }
  }
}

void fix_rb(node *to_fix) {
  // Case 1
  to_fix->color = RED;
  if (to_fix->parent == NULL) {
    to_fix->color = BLACK;
    return;
  }
  node* parent = to_fix->parent;
  if (parent == RED) {
    node* parent_sibling = get_sibling(parent);
    if (parent_sibling == RED) {
      parent->color = BLACK;
      parent_sibling->color = BLACK;
      fix_rb(parent->parent);
    } else {
      
    }
  }
}