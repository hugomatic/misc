
def partition(array, left, right, pivot):

  if left >= right:
    return

  while left <= right:
    while array[left] < pivot:
      left += 1

    while array[right] > pivot:
      right -= 1

    if left <= right:
      left_v = array[left]
      right_v = array[right]
      # print(f'SWAP  pivot {pivot},  left [{left}] {left_v} right [{right}] {right_v}')
      array[left] = right_v
      array[right] = left_v
      left += 1
      right -= 1

  return left

def qsort(array, left, right):

  print(f'qsort {array[left:right]}')

  if not len(array) > 1:
     return

  pivot_index = int (len(array) / 2)
  pivot = array[pivot_index]

  left = 0
  right = len(array) -1

  index = partition(array, left, right, pivot)
  print(f'PART {array} {index}')
  qsort(array, left, index -1)
  qsort(array, index, right)

  return array


def sort(array):
  left = 0
  right = len(array) -1
  qsort(array, left, right)
