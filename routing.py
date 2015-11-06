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
	def neighbors(self):
		result = []
		for link in self.links:
			result.append(link.destID)
		return result
	def setLink(self,destID,cost):
		if not destID in self.neighbors():
			return
		link = None
		for zelda in self.links:
			if zelda.destID == destID:
				link = zelda
		if cost==-999:
			self.links.remove(link)
		else:
			link.cost = cost
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
	def findNode(self, nodeID,autocreate=True):
		res = [x for x in self.nodes if x.ID == nodeID]
		for x in self.nodes:
			if x.ID == nodeID:
				return x
		if autocreate:
			temp = Node(nodeID)
			self.addNode(temp)
			return self.findNode(nodeID)
		else:
			return None
	# adds links between 2 nodes, which are found or created
	# comprehensive
	def addLink(self, sourceID, destID, cost):
		sourceNode = self.findNode(sourceID)
		destNode = self.findNode(destID)

		if cost==-999: #if cost is -999, don't do anything
			return

		sourceLink = Link(destID, cost) #sourceID to destID
		sourceNode.addLinkToNode(sourceLink)

		destLink = Link(sourceID, cost) #distID to sourceid
		destNode.addLinkToNode(destLink)
	#changes a link between sourceID and destID to cost C. 
	#if cost is -999, removes link
	#if no such link exists, create a link
	def changeLink(self, sourceID, destID, cost):
		sourceNode = self.findNode(sourceID,False)
		destNode = self.findNode(destID,False)
		if sourceNode==None or destNode==None:
			return
		if not destID in sourceNode.neighbors():
			self.addLink(sourceID,destID,cost)
		else:
			sourceNode.setLink(destID,cost)
			destNode.setLink(sourceID,cost)
		
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
		#format: <ID of a node> <ID of another node> <cost of link>
		lines = f.readlines()
		for line in lines:
			currLink = map(int, line.split())
			graph.changeLink(currLink[0], currLink[1], currLink[2])
			print 'affecting change: %d <-> %d with cost %d' % (currLink[0],currLink[1],currLink[2])
			graph.printGraph()
	elif(filetype == 'MESSAGE_FILE'):
		print 'parsing message file...'
	else:
		print "Error: Invalid filetype in parse(filename, filetype"

def main():
	print "main"
	g = Graph()
	parse('topology.txt', 'TOPOLOGY_FILE', g)
	g.printGraph()
	parse('changes.txt','TOPOLOGY_CHANGES_FILE',g)

if __name__ == '__main__':
	main()
