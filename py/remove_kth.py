#!/usr/bin/env python3

"""
Input:
k = 2
1 -> 2 -> 5 -> 6 -> 7 -> X

Output: 1 -> 2 -> 5 -> 7 -> X

Explanation: Element 6 is the 2nd to last element, therefore removed.
"""

class Node:
  next = None

  def __init__(self, value):
    self.value = value

  def __repr__(self):

    r = ''
    n = self
    while(n):
      x = '!' if n.next is None else '->'
      r += f'{n.value}{x}'
      n = n.next
    return r


def make_list(l):
  assert(len(l))
  start = Node(l[0])
  x = start
  for e in l[1:]:
    x.next = Node(e)
    x = x.next
  return start

def remove_next(node):
  node.next = node.next.next


