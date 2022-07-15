#!/usr/bin/env python3

def nodeDepths(root):
    # Write your code here.
    pass


# This is the class of the input binary tree.
class BinaryTree:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None


def pr(root, depth=0):
  space = depth * "-"
  if root is None:
      return
  print(f'{space} {root.value}')
  pr(root.left,  depth + 1)
  pr(root.right,  depth + 1)


def dsum(root):
  total = 0
  def sm(root, depth):
    nonlocal total
    if root is None:
      return
    total += depth
    sm(root.left, depth +1)
    sm(root.right, depth +1)
  sm(root, 0)
  return total



def make_tree():
  root = BinaryTree(1)
  root.left = BinaryTree(2)
  root.right = BinaryTree(3)
  root.left.left = BinaryTree(4)
  root.left.right = BinaryTree(5)
  root.right.left = BinaryTree(6)
  root.right.right = BinaryTree(7)
  root.left.left.left = BinaryTree(8)
  root.left.left.right = BinaryTree(9)
  return root



  root.right.right = nd.BinaryTree(7)
  root.left.left.left = nd.BinaryTree(8)

