#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = calloc(1, sizeof(node_t));
  nil_node->color = RBTREE_BLACK;
  nil_node->left = nil_node->right = nil_node->parent = NULL;
  p->nil = nil_node;
  p->root = p->nil;

  return p;
} // 다시 보기

void delete_rbtree_node(rbtree *t, node_t *node)
{
  if (node == t->nil)
  {
    return;
  }
  delete_rbtree(t, node->right);
  delete_rbtree(t, node->left);
  free(node);
}

void delete_rbtree(rbtree *t)
{
  delete_rbtree(t, t->root);
  free(t->nil);
  free(t);
}

int is_left_right(node_t *new_node)
{
  return new_node == new_node->parent->right &&
         new_node->parent == new_node->parent->parent->left;
}

int is_right_left(node_t *new_node)
{
  return new_node == new_node->parent->left &&
         new_node->parent == new_node->parent->parent->right;
}

void left_rotate(rbtree *t, node_t *x)
{                       // x는 내가 예전에 생각한 부모노드, y는 예전에 new_node
  node_t *y = x->right; // x의 오른쪽 자식 y를 회전 축으로 올림
  x->right = y->left;

  if (y->left != t->nil)
  {
    y->left->parent = x; // y의 왼쪽자식의 부모를 x로 갱신해줌 왜?->부모가 바꼈기 때문
  }
  y->parent = x->parent; // y의 부모를 x의부모(조부모)로 설정

  if (x->parent == t->nil)
  {              // x가(초기 부모가)root였다면
    t->root = y; // 회전 후 y가 루트에 들어가야함
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y)
{
  node_t *x = y->left;
  y->left = x->right;

  if (x->right != t->nil)
  {
    x->right->parent = y;
  }
  x->parent = y->parent;

  if (y->parent == t->nil)
  {
    t->root = x;
  }
  else if (y == y->parent->right)
  {
    y->parent->right = x; // y가 조부모의 오른쪽 자식인지 확인후 조부모랑 x랑 연결
  }
  else
  {
    y->parent->left = x;
  }
  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree *t, node_t *new_node)
{
  while (new_node->parent->color == RBTREE_RED)
  {
    node_t *uncle;
    if (new_node->parent == new_node->parent->parent->left)
    {
      uncle = new_node->parent->parent->right;
      if (uncle != t->nil && uncle->color == RBTREE_RED)
      {
        uncle->color = RBTREE_BLACK;
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      else
      {
        if (is_left_right(new_node))
        {
          new_node = new_node->parent;
          left_rotate(t, new_node);
        }
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, new_node->parent->parent);
      }
    }
    else
    {
      // 부모가 오른쪽 자식일 때 대칭 처리
      uncle = new_node->parent->parent->left;
      if (uncle != t->nil && uncle->color == RBTREE_RED)
      {
        uncle->color = RBTREE_BLACK;
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }
      else
      {
        if (is_right_left(new_node))
        {
          new_node = new_node->parent;
          right_rotate(t, new_node);
        }
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        left_rotate(t, new_node->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = new_node->right = new_node->parent = t->nil;
  node_t *cur = t->root;
  node_t *parent = t->nil;

  if (t->root == t->nil)
  {
    t->root = new_node;
    new_node->color = RBTREE_BLACK;
    new_node->parent = t->nil;
    return t->root;
  }

  while (cur != t->nil)
  {
    parent = cur;
    if (key < cur->key)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }

  if (key < parent->key)
  {
    parent->left = new_node;
  }
  else
  {
    parent->right = new_node;
  }
  new_node->parent = parent;

  rbtree_insert_fixup(t, new_node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}
void erase_fixup(rbtree *t, node_t *p)
{
}
int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *parent = p->parent;
  if ((p->left == t->nil) || (p->right == t->nil))  //타겟의 자녀가 하나일때
  {
    if ((p->left == t->nil) && (p->right != t->nil))  //타겟의 자녀가 왼쪽일때
    {
      if (p == p->parent->left) // 타겟이 부모노드의 왼쪽 자식일 때
      {
        node_t *bro = parent->right;  // 형제는 오른쪽

        while ((p != t->root) && p->color == RBTREE_BLACK)  // 타겟이 루트가 아니고 블랙노드일때
        {
          // CASE 1
          if (bro->color == RBTREE_RED)
          {
            node_t *temp = bro->color;
            bro->color = parent->color;
            parent->color = temp;
          }
          /*// CASE 2
          if ((bro->color == RBTREE_BLACK) && ((bro->right->color == RBTREE_BLACK) && (bro->left->color == RBTREE_BLACK)))
          {
            bro->color=RBTREE_RED;
            erase_fixup(t,p);
          }
          else{
            if((bro->color==RBTREE_BLACK)&&(bro->left==RBTREE_RED)){
              
            } 이거 fixup에 넣었야 함*/
          }
        }
      }
      // else
      // {
      //   node_t *bro = p->parent->left;
      // }
    }
  }
  // else if ((p->left != t->nil) && (p->right != t->nil))
  // {

  // }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
