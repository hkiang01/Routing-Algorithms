import sys
import getopt

class Node:
	def __init__(self, ID=-1):
		self.links = [] # links node is connected to
		self.ID = ID
	def printNode(self):
		print 'Node: %d - (Neighbor:cost): '
		for link in links:
			print "%d:%d" % (link.destID, link.cost)

class Link:
	def __init__(self, destID=-1, cost=0):
		self.destID = destID
		self.cost = cost

class Graph:
	def __init__(nodes):
		self.nodes = []
	def addNode(self, node):
		self.nodes.append(node)
		return Node
	# finds nodes in graph matching nodeId.
	# if no match, creates new node and appends to graph
	# returns node(s) matched or created
	def findNode(self, nodeID):
		res = filter(lamda x: x.ID == nodeID, self.nodes)
		if not res:
			temp = Node(nodeID)
			res = self.addNode(temp)
		return res
	# adds links between 2 nodes, which are found or created
	# comprehensive
	def addLink(self, sourceID, destID, cost):
		sourceNode = self.findNode(sourceID)
		destNode = self.findNode(destID)

		sourceLink = Link(destId, cost) #sourceID to destID
		sourceNode.links.append(sourceLink)

		destLink = Link(sourceId, cost) #distID to sourceid
		destNode.links.append(destLink)
	def printGraph(self):
		for node in self.nodes:
			printNode(node)

# file parsing and topology construction
def parse(filename, filetype, graph):
	f = open(filename, "r")
	if(filetype == 'TOPOLOGY_FILE'): # process the links
		#format: <ID of a node> <ID of another node> <cost of link>
		num_nodes = file_len(filenam)
		for i in xrange(0, num_nodes):
			currLine = f.readline()
			# e.g., currline 1 2 3 to ['1', '2', '3'] to currlink [1,2,3]
			currLink = map(int, currLine.split())
			graph.addLink(currLink[0], currLink[1], currLink[2])
	elif(filetype == 'TOPOLOGY_CHANGES_FILE'):

	elif(filetype == 'MESSAGE_FILE'):
	
	else:
		print "Error: Invalid filetype in parse(filename, filetype"

def main():
	print "main"
	g = Graph()
	parse('topology.txt', 'TOPOLOGY_FILE', g)
	g.printGraph()

