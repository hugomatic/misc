#!/usr/bin/env python3

import random

class Node:
  left  = None
  right = None

  def __init__(self, value):
    self.value = value

  def __repr__(self):
    leaf = ""
    if self.right is None and self.left is None:
      leaf = "!"
    return (f'{self.value}{leaf}')

def serialize(root):
  """
  Preorder: nlr
  root first
  """
  if not root:
    return "x,"
  left = serialize(root.left)
  right = serialize(root.right)
  return f'{root.value},' + left + right


def deserialize(s):

  def deser(q):
    print(q)
    assert(len(q))
    e = q.pop(0).strip()

    print(f'? [{e}]')
    if e == 'x':
      print('Empty')
      return None

    value = int(e)
    print(f'? root {value}')
    root = Node(value)
    print(f'left')
    root.left = deser(q)
    print(f'right')
    root.right = deser(q)
    print(f'!!{value}')
    return root

  q_list = s.strip(',').split(',')
  root = deser(q_list)
  return root






def make_tree():

  root = Node(1)
  root.left = Node(2)
  root.right = Node(3)
  root.left.left = Node(4)

  root.left.right = Node(5)
  root.left.right.left = Node(6)

  return root

if __name__ == "__main__":
  null = "x"
  assert(serialize(None) == null)
  assert(deserialize("x") == None)
  print('done')
