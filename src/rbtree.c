#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (p == NULL)
  {
    return NULL;
  }

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  if (p->nil == NULL)
  {
    return NULL;
  }

  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

void printGraph(rbtree *t, node_t *cur, int level, int isLeft) {
    if (cur == t->nil) {
        return;
    }

    // 오른쪽 자식 노드 출력
    printGraph(t, cur->right, level + 1, 0);

    // 현재 노드 출력
    for (int i = 0; i < level - 1; i++) {
        printf("    ");
    }
    if (level > 0) {
        printf(isLeft ? " \\_ " : " /⎺ ");  // 왼쪽 자식일 경우 "\\" 출력, 오른쪽 자식일 경우 "/" 출력
    }
    if (cur->color == RBTREE_RED)
    {
      printf("\x1b[31m%d\x1b[0m\n", cur->key);
    }
    else{
      printf("%d\n", cur->key);
    }

    // 왼쪽 자식 노드 출력
    printGraph(t, cur->left, level + 1, 1);
}


// tree 프린트
void printTree(rbtree *t, node_t *node){
    if(node== NULL || node == t->nil) return;
    char *color_str;
    if (node->color == RBTREE_RED) {
        color_str = "R";
    } else {
        color_str = "B";
    }
    printf("%d(%s) ", node->key, color_str);
    printTree(t,node->left);
    printTree(t,node->right);
}

void delete_node(rbtree *t, node_t *n){
  if(n != t->nil){  
      delete_node(t, n->left);
      delete_node(t, n->right);
      free(n);
      n =NULL; 
    }
}
//트리 지우기
void delete_rbtree(rbtree *t) {
  // t->nil = NULL;
  // t->root = NULL;
  if (t != NULL)
  {
    if (t->root != t->nil) {
      delete_node(t, t->root);  
    }

  free(t->nil);
  t->nil = NULL;
  free(t);
  t=NULL;
  }
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else {
    x->parent->right =y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y){
  node_t *x = y->left;
  y->left = x->right;
  if (x->right != t->nil){
    x->right->parent = y;
  }
  x->parent = y->parent;
  if (y->parent == t->nil)
  {
    t->root = x;
  }
  else if (y == y->parent->right)
  {
    y->parent->right = x;
  }
  else{
    y->parent->left = x;
    
  }
  x->right = y;
  y->parent = x;
}

void rb_insert_fixup(rbtree *t, node_t *n){
  while (n->parent->color == RBTREE_RED)
  {
    if (n->parent == n->parent->parent->left)
    {
      node_t *y = n->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        n->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;

        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else 
      {
        if (n == n->parent->right)
        { 
          n = n->parent;
          left_rotate(t, n);
        }
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        right_rotate(t, n->parent->parent);
      }
    }
    else{
      node_t *x = n->parent->parent->left;
      if (x->color == RBTREE_RED)
      {
        n->parent->color = RBTREE_BLACK;
        x->color = RBTREE_BLACK;
        
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      }
      else 
      {
        if (n == n->parent->left)
        {
          n = n->parent;
          right_rotate(t, n);
        }

        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        left_rotate(t, n->parent->parent);
      }
    }   
  }
  t->root->color = RBTREE_BLACK;
  t->root->parent = t->nil;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // if (rbtree_find(t, key))
  // {
  //   printf("The input number already exists\n");
  //   return NULL;
  // }
  
  node_t *newNode = (node_t *)malloc(sizeof(node_t));
  if (newNode == NULL) {
    return NULL;
  }
  node_t *y = t->nil;
  node_t *x = t->root;
  newNode->key = key;
  newNode->parent = NULL;

  while (x != t->nil)
  {
    y = x;
    if (newNode->key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }

  newNode->parent = y;
  if (y == t->nil)
  {
    t->root = newNode;
  }
  else if (newNode->key < y->key)
  {
    y->left = newNode;
  } else{
    y->right = newNode;
  }

  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->color = RBTREE_RED;

  rb_insert_fixup(t, newNode);
  printf("\"%d\" has been succesfully inserted!\n", newNode->key);
  return t->root;
}

// 삭제할 노드 찾기
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur = t->root;
  
  while(cur != t->nil){
    if (key < cur->key)
    {
      cur = cur->left;
    }
    else if (key >cur->key)
    {
      cur = cur->right;
    }
    else 
    {
      break;
    }
  }

  if (cur == t->nil)
  {
    return NULL;
  }

  printf("find함수 %d", cur->key);

  return cur;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *cur = t->root;
  if (cur == NULL){
    return NULL;
  }

  if (cur == t->nil){
    return cur;
  }

  while (cur->left != t->nil){
    cur = cur->left;
  }
  // printf("%d", cur->key);
  return cur;
}

// 후임자 찾기
node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t->root;
  if (cur == NULL){
    return NULL;
  }

  if (cur == t->nil){
    return cur;
  }

  while (cur->right != t->nil){
    cur = cur->right;
  }

  return cur;
}

void rb_delete_fixup(rbtree *t, node_t *x){
  node_t *w = NULL;

  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      w = x->parent->right;
      // case 1
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      //case 2
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      //case 3
      else {
       if (w->right->color == RBTREE_BLACK){
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t, w);
        w = x->parent->right;
        }
        //case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
        }
    }
    else{
      w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}
  

node_t *tree_min(rbtree *t, node_t *node)
{
    if (node == t->nil){
      return NULL;
    }

    while (node->left != t->nil){
      node = node->left;
    }

  return node;
   
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

//노드 삭제
int rbtree_erase(rbtree *t, node_t *p) {
  if (p == NULL)
  {
    return -1;
  }

  node_t *y = p;
  color_t y_original_color = y->color;
  node_t *x = NULL;
  
  if (p->left == t->nil)
  {
    x = p->right;
    rb_transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    rb_transplant(t, p, p->left);
  }
  else{
    y = tree_min(t, p->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent == p)
    {
      x->parent = y;
    }
    else{
      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent= y;
    y->color = p->color;
  }
  if (y_original_color == RBTREE_BLACK)
  {
    rb_delete_fixup(t, x);
  }
  free(p);
  p = NULL;  
  printf("erase");
  return 0;
}

void inorder_traversal(const rbtree *t, node_t *cur, key_t *arr, size_t *index, const size_t n) {
    if (cur == t->nil || *index >= n){
          return;
    }
    inorder_traversal(t, cur->left, arr, index, n);
    
    if (*index < n) {
        arr[*index] = cur->key;
        (*index)++;
    }

    inorder_traversal(t, cur->right, arr, index, n);
    
}



int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t == NULL || arr == NULL || n == 0){
        return -1;
  }

  size_t index = 0;
  inorder_traversal(t, t->root, arr, &index, n);

  return 0;
}
