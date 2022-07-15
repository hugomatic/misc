#!/usr/bin/env python3

silent = False
def log(s):
  if not silent:
    print(f'# {s}')

def isValidSubsequence(array, sequence):
      # Write your code here.
      array_index = 0
      for sec_index, sequence_value in enumerate(sequence):
          log(f'looking for {sequence[sec_index:]} {sequence_value} in {array[array_index:]}')
          matched = False
          while not matched:
              if array_index >= len(array):
                  log('end of array!')
                  return False
              array_value = array[array_index]
              # match?
              if array_value == sequence_value:
                  log(f'  match s[{sec_index}] {sequence_value} with a[{array_index}] {array_value}')
                  matched = True
              array_index += 1
      return True

def example(array, seq):
  isornot = "is a" if isValidSubsequence(array, seq) else "is not a"
  print(f'{seq} {isornot} valid subsequence of {array}')

if __name__ == "__main__":
  example([1,2,2,2,3,3,3,4,4,4,5,5,5,6,6], [2,3,4])
  example([1,2,2,2,3,3,3,4,4,4,5,5,5,6,6], [2,42,4])
