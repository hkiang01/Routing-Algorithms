import sys
import getopt

class Graph:
	def __init__(nodes):
		self.nodes = []
	def addNode(node):
		self.nodes.append(node)

class Node:
	def __init__(self, ID=-1):
		self.links = [] # links node is connected to
		self.ID = ID
	def addLink(self, newlink):
		self.links.append(newlink)

class Link:
	def __init__(self, sourceID=-1, destID=-1, cost=0):
		self.sourceID = sourceID
		self.destID = destID
		self.cost = cost

links = [] # list of links

# file parsing and topology construction
def parse(filename, filetype, graph):
	if(filetype == 'TOPOLOGY_FILE'): # process the links
		num_nodes = file_len(filenam)
		for i in xrange(0, num_nodes):
			links.
	elif(filetype == 'TOPOLOGY_CHANGES_FILE'):

	elif(filetype == 'MESSAGE_FILE'):
	
	else:
		print "Error: Invalid filetype in parse(filename, filetype"

def main():
	print "main"
	g = Graph()
	
