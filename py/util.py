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


def make_tree():

  root = Node(1)
  root.left = Node(2)
  root.right = Node(3)
  root.left.left = Node(4)

  root.left.right = Node(5)
  root.left.right.left = Node(6)

  return root

def ptree_x(root, depth=-1, st=''):
   if not root:
     return

   prefix = "│  " * depth if depth >0 else ''

   print(f'{prefix}{st}{root.value}')
   ptree(root.left, depth+1, '├──')
   # print(f'{prefix}├──{root.left.value}')
   ptree(root.right, depth+1, '└──' )
   # print(f'{prefix}└── {root.right.value}')



def ptree(root, prefix=None, is_left=True, has_right_sibling=False):
   if not root:
     return

   if prefix is None:
     st = ""
   else:
     if is_left and has_right_sibling:
       st = f'{prefix}├──'
       # print(f'# {root.value} l {root.left} r {root.right}')
     else:
       st = f'{prefix}└──'

   print(f'{st}{root.value}')

   if prefix is None:
     prefix = ""
   else:
     prefix += "│  " if root.right else "   "
   ptree(root.left, prefix, True, root.right)
   ptree(root.right, prefix, False, False)





