#! /bin/python

import sys
import getopt

class Node:
	def __init__(self, ID=-1):
		self.links = [] # links node is connected to
		self.ID = ID
	def printNode(self):
		print 'Node: %d - (Neighbor:cost): ' % (self.ID)
		for link in self.links:
			print "%d:%d" % (link.destID, link.cost)
	def addLinkToNode(self, link):
		self.links.append(link)

class Link:
	def __init__(self, destID=-1, cost=0):
		self.destID = destID
		self.cost = cost

class Graph:
	def __init__(self):
		self.nodes = []
	def addNode(self, node):
		self.nodes.append(node)
		return node
	# finds nodes in graph matching nodeId.
	# if no match, creates new node and appends to graph
	# returns node(s) matched or created
	def findNode(self, nodeID):
		res = [x for x in self.nodes if x.ID == nodeID]
		for x in self.nodes:
			if x.ID == nodeID:
				return x
		temp = Node(nodeID)
		self.addNode(temp)
		return self.findNode(nodeID)
	# adds links between 2 nodes, which are found or created
	# comprehensive
	def addLink(self, sourceID, destID, cost):
		sourceNode = self.findNode(sourceID)
		destNode = self.findNode(destID)

		sourceLink = Link(destID, cost) #sourceID to destID
		sourceNode.addLinkToNode(sourceLink)

		destLink = Link(sourceID, cost) #distID to sourceid
		destNode.addLinkToNode(destLink)
	def printGraph(self):
		for node in self.nodes:
			node.printNode()

# file parsing and topology construction
def parse(filename, filetype, graph):
	f = open(filename, "r")
	if(filetype == 'TOPOLOGY_FILE'): # process the links
		print 'parsing topology file...'
		#format: <ID of a node> <ID of another node> <cost of link>
		lines = f.readlines()
		for line in lines:
			# e.g., currline 1 2 3 to ['1', '2', '3'] to currlink [1,2,3]
			currLink = map(int, line.split())
			graph.addLink(currLink[0], currLink[1], currLink[2])
	elif(filetype == 'TOPOLOGY_CHANGES_FILE'):
		print 'parisng topology changes file...'
	elif(filetype == 'MESSAGE_FILE'):
		print 'parsing message file...'
	else:
		print "Error: Invalid filetype in parse(filename, filetype"

def main():
	print "main"
	g = Graph()
	parse('topology.txt', 'TOPOLOGY_FILE', g)
	g.printGraph()

if __name__ == '__main__':
	main()
