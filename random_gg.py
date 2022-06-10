import random
import networkx as nx

G = nx.gnp_random_graph(100000, 0.001, seed=None, directed=True)

weights = {(w[0], w[1]) : round(random.uniform(0, 1), 1) for w in G.edges}
nx.set_edge_attributes(G, values = weights, name = 'weight')

nx.write_weighted_edgelist(G, 'test.txt', comments='#', delimiter=' ', encoding='utf-8')