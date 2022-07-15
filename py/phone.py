#!/usr/bin/env python3

silent = False

def log(s):
  if not silent:
    print(f'# {s}')

# substitutions dictionary
subs = {   '0':[0],
           '1':[1],
           '2':['a','b','c'],
           '3':['d','e','f'],
           '4':['g','h','k'],
           '5':['j','k','l'],
           '6':['m','n','o'],
           '7':['p','q','r','s'],
           '8':['t','u','v'],
           '9':['w','x','y','z']
}

def generate_number(nb_list, prefix, substi):
  log(f'gen {nb_list} {prefix} {substi}')
  first = substi[0]
  rest = substi[1:]
  for n in substi[0]:
    nb = f'{prefix}{n}'
    if rest:
      generate_number(nb_list, nb, rest)
    else:
      nb_list.append(f'{nb}')

def phoneNumberMnemonics(phoneNumber):
  mnemonics = []

  # Write your code here.
  inputs = [n for n in phoneNumber]
  substi = []
  for n in phoneNumber:
      # print(f'{n}')
      # for l in subs[n]:
      alts = subs[n]
      substi.append(alts)
  log(f'{phoneNumber}: {substi}')

  print('-----')

  for n in substi[0]:
    generate_number(mnemonics, n, substi[1:])

  return mnemonics

def example(nb):
  mn = phoneNumberMnemonics(nb)
  print(f'the mnemonics for "{nb}" are:')
  print(mn)

if __name__ == "__main__":
  example('3411')



