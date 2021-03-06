#!/usr/bin/python

import sys
import getopt
import Queue

max_node = 1

class Node:
	def __init__(self, ID=-1):
		self.links = [] # links node is connected to
		self.ID = ID
		self.routeTable = []
	def init_route_table(self):
		global max_node
		for i in range(1,max_node+1):
			self.routeTable.append([i,i,self.getLinkCost(i)])
	def print_route_table(self):
		result = ""
		for n in sorted(self.routeTable,key=lambda t: t[0]):
			result += "%d %d %d" % (n[0],n[1],n[2])
			result += "\n"
		return result
	def printNode(self):
		print 'Node: %d - (Neighbor:cost): ' % (self.ID)
		for link in self.links:
			print "%d:%d" % (link.destID, link.cost)
	def neighbors(self):
		result = []
		for link in self.links:
			result.append(link.destID)
		return result
	def localUpdate(self):
		for i in range(len(self.routeTable)):
			self.routeTable[i][1] = self.routeTable[i][0] # next <- dest
			self.routeTable[i][2] = self.getLinkCost(self.routeTable[i][0]) #cost <- getLinkCost(dest)
	def getNextHop(self,destID):
		for n in self.routeTable:
			if n[0]==destID:
				return n
		return None
	def updateRouteTable(self,destID,nextHop,cost):
		for i in range(len(self.routeTable)):
			if self.routeTable[i][0]==destID:
				self.routeTable[i][1] = nextHop
				self.routeTable[i][2] = cost
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
	def getLinkCost(self,destID):
		if destID==self.ID:
			return 0
		if not destID in self.neighbors():
			return -999
		for link in self.links:
			if link.destID == destID:
				return link.cost
		return -999
	def addLinkToNode(self, link):
		self.links.append(link)
	def findRouteTableElem(self, id):
		for route in self.routeTable:
			if route[0] == id:
				return route
	# link state algorithm
	# node_list is nodes in graph
	# source: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Using_a_priority_queue
	def local_link_state(self, node_list, graph):
		# initialization phase
		self.routeTable = {} # clear route table for safety
		N = {} # nodes already found min for
		q = Queue.PriorityQueue() # priority queue sorted by distance from self node
		# self
		N.append(self)
		node_list.remove(self)
		# neighbors
		for neighbor in self.neighbors():
			# [dest, next, cost]
			self.routeTable.append([neighbor.ID, neighbor.ID, self.getLinkCost(neighbor.ID)])
			q.put((self.getLinkCost(neighbor.ID), neighbor.ID)) # (k,v) = (D(u), ID)
		# non-neighbors
		for non_neighbor in node_list:
			self.routeTable.append([non_neighbor.ID, None, -999]) #-999 for infinity
		# Dijkstra's phase
		while (!q.empty()):
			# find node w not in N s.t. D(w) is minimum
			u = q.get() # (D(u), id), access id by u[1], cost by u[0]
			u_node = graph.findNode(u[1])
			N.append(u_node) # add u to N
			dist_u = self.findRouteTableElem(u[1])
			for neighbor in u_node.neighbors() # neigbor is an id
				v_node = graph.findNode(neighbor)
				v_route = self.findRouteTableElem(v_node.ID)
				alt = dist_u + u_node.getLinkCost(neighbor)
				if alt < v_route[2]: # cost from self to v is v_route[2]
					v_route[2] = alt # todo: MAKE SURE THIS IS BY REFERENCE!!!
					v_route[1] = u_node.ID # prev
					# todo: q.decrease_priority(v, alt)
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
	def init_route_tables(self):
		for node in self.nodes:
			node.init_route_table()
	def route_path(self,source,destination):
		if source==destination:
			return []
		node = self.findNode(source)
		path = node.getNextHop(destination)
		if path[2]<0:
			return None
		route = self.route_path(path[1],destination)
		route.insert(0,source)
		return route
	def route_cost(self,source,destination):
		if source==destination:
			return 0
		node = self.findNode(source)
		path = node.getNextHop(destination)
		return path[2]
	def route_message_toString(self,source,destination,message):
		path = self.route_path(source,destination)
		#cost = self.route_path(source,destination)
		result = "from %d to %d hops " % (source,destination)
		if path==None:
			result+="unreachable "
		else:
			for p in path:
				result+=str(p)+" "
		result += "message "+message
		return result+"\n"
	def distance_vector(self):
		global max_node
		changed = True
		for node in self.nodes:
			node.localUpdate()
		while(changed):
			changed = False
			for node in self.nodes:
				for destID in range(1,max_node+1):
					currBest = node.getNextHop(destID)
					if destID==node.ID:
						continue
					for nextHop in node.neighbors():
						proposedBest = self.findNode(nextHop,False).getNextHop(destID)
						newCost = node.getLinkCost(nextHop) + proposedBest[2]
						if proposedBest[2]==-999: #if there is no route
							continue	  #don't use this neighbor
						if(newCost==currBest[2]): #if we have a tie
							if currBest[1] > nextHop:#break tie with whichever node has lowest nextHop
								currBest[1] = nextHop
								changed=True
								continue
							continue
						if(newCost<currBest[2] or currBest[2]<0):
							currBest[1] = nextHop
							currBest[2] = newCost
							changed=True
						#print "Current n"
						#print n
					node.updateRouteTable(currBest[0],currBest[1],currBest[2])
							
	def link_state(self):
		for node in self.nodes
			node.local_link_state(self.nodes, self)
	def route_tables_toString(self):
		result = ""
		for node in sorted(self.nodes,key=lambda n: n.ID):
			result+=node.print_route_table()
		return result
	# finds nodes in graph matching nodeId.
	# if no match, creates new node and appends to graph
	# returns node(s) matched or created
	def findNode(self, nodeID,autocreate=True):
		global max_node
		res = [x for x in self.nodes if x.ID == nodeID]
		for x in self.nodes:
			if x.ID == nodeID:
				return x
		if autocreate:
			temp = Node(nodeID)
			self.addNode(temp)
			if nodeID>max_node:
				max_node = nodeID
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
		#format: <ID of a node> <ID of another node> <cost of link>
		lines = f.readlines()
		for line in lines:
			# e.g., currline 1 2 3 to ['1', '2', '3'] to currlink [1,2,3]
			currLink = map(int, line.split())
			graph.addLink(currLink[0], currLink[1], currLink[2])
		return None
	elif(filetype == 'TOPOLOGY_CHANGES_FILE'):
		#format: <ID of a node> <ID of another node> <cost of link>
		lines = f.readlines()
		result = []
		for line in lines:
			currLink = map(int, line.split())
			result.append(currLink)
		return result
	elif(filetype == 'MESSAGE_FILE'):
		lines = f.readlines()
		result = []
		for line in lines:
			t = line.split()
			message_string = ""
			t_prime = t[2:]
			for word in t_prime:
				message_string+=word+" "
			message_string = message_string[:-1]
			result.append([int(t[0]),int(t[1]),message_string])
		return result
	else:
		return None

def updateGraph(currLink,graph):
	graph.changeLink(currLink[0],currLink[1],currLink[2])

#Important functions:
#	parse(filename,[TOPOLOGY_FILE/TOPOLOGY_CHANGES_FILE/MESSAGE_FILE], [graph object])
#	g.init_route_tables()
#	g.distance_vector()
#	g.route_tables_toString()
#	g.
def main():
	if len(sys.argv) != 4:
		print "Usage: %s [topology file] [messages file] [topology changes file]" % (sys.argv[0])
		sys.exit(1)
	topo_file = sys.argv[1]
	topo_changes_file = sys.argv[3]
	messages_file = sys.argv[2]
	g = Graph()
	parse(topo_file, 'TOPOLOGY_FILE', g)
	changes = parse(topo_changes_file,'TOPOLOGY_CHANGES_FILE',g)
	messages = parse(messages_file,'MESSAGE_FILE',g)
	g.init_route_tables()
	g.distance_vector()
	result = ""
	result+= g.route_tables_toString()
	for message in messages:
		result+= g.route_message_toString(message[0],message[1],message[2])
	for change in changes:
		updateGraph(change,g)
		g.distance_vector()
		result+= g.route_tables_toString()
		for message in messages:
			result+= g.route_message_toString(message[0],message[1],message[2])
	f = open("output.txt","w")
	f.write(result)
	f.close()


	#g.printGraph()
	#print g.route_tables_toString()
	#print g.route_path(1,2)
	#print g.route_cost(1,2)
	#print g.route_message_toString(2,1,"here is a message from 2 to 1")

if __name__ == '__main__':
	main()
