
def mat():
  return [
   [1,0,0,1,0],
   [1,0,1,0,0],
   [0,0,1,0,1],
   [1,0,1,0,1],
   [1,0,1,1,0],
  ]



def neighbors(m, r, c):
  row_len = len(m[0])
  # north east west south
  ews = [ [r, c+1], [r, c-1], [r+1, c]]
  n_list = [x for x in ews if x[0] >= 0 and x[1] >= 0 and x[0] < row_len and x[1] < row_len ]
  return n_list

def has_water(m, x, y):
  return True if m[x][y]==1 else False


def water_neighbors(m,x,y):
  n_list = [e for e in neighbors(m,x,y) if has_water(m, e[0], e[1])]
  return n_list

def get_river(m,x,y):
  if not has_water(m,x,y):
    return []
  river = [ [x,y] ]
  m[x][y] = 'x'
  n_list = water_neighbors(m,x,y)
  while n_list:
    new_neighbors = []
    for x,y  in n_list:
      m[x][y] = 'x'
      river.append([x,y])
      new_neighbors += water_neighbors(m,x,y)
    n_list = new_neighbors
  return river

def get_rivers(m):
  if not len(m):
    return []
  if not len(m[0]):
    return []

  col_count = len(m[0])
  row_count = len(m)

  rivers = []
  for r in range(row_count):
    for c in range(col_count):
      river = get_river(m, r, c)
      if river:
        rivers.append(river)
  return rivers

def riverSizes(matrix):
    # Write your code here.
    rivers = get_rivers(matrix)
    river_sizes = [len(x) for x in rivers]
    return river_sizes


