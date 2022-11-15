#!/usr/bin/env python3

from graphviz import Digraph

example = {
  "nodes": [
    {"id":"1", "label":"a", "data": 1.0, "grad": 0.1, "op": "+" },
    {"id":"2", "label":"b", "data": 2.0, "grad": 0.2, "op": "*"},
    {"id":"3", "label":"c", "data": 3.0, "grad": 0.3, "op": ""},
  ],
  "edges": [
    ["1","2"],
    ["2","3"]
  ]
}


def draw_dot(root):
    dot = Digraph(format='svg', graph_attr={'rankdir': 'LR'})
    nodes, edges = root["nodes"], root["edges"]
    print('==== nodes =====')
    for n in nodes:
        print(' ', n)
    print('=== edges ====')
    for e in edges:
        print(' ',e, e[0] , e[1])
    nodeMap = {}
    for n in nodes:
        name = n["id"]
        nodeMap[name] = n
        label='{ %s | data %.4f | grad %.4f }' % (n["label"], n["data"], n["grad"])
        print('node:', 'name: "' + name + '"', 'label: "'+label+'"')
        dot.node(name=name, label=label, shape='record')
        if n['op']:
            dot.node(name = n["id"] + n["op"], label = n["op"])
            dot.edge(name + n["op"], name)
    for n1, n2 in edges:
        e1 = n1
        e2 = n2 + nodeMap[n2]["op"]
        dot.edge(e1, e2)
    return dot

if __name__ == "__main__":
  import sys
  import argparse
  import json

  parser = argparse.ArgumentParser()
  parser.add_argument('src', default='test', type=str, help='JSON input file (or test)')
  parser.add_argument('dst', default='out', type=str, help='output file names (dot and svg)')
  args = parser.parse_args()

  if args.src == 'test':
    root = example
  else:
    with open(args.src, 'r') as f:
      root = json.load(f)

  dot = draw_dot(root)
  dot.render(args.dst)
