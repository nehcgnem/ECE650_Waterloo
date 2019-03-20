import sys
import re
from decimal import *
#import itertools


# YOUR CODE GOES HERE
def fullmatch(regex, string, flags=0):
    """Emulate python-3.4 re.fullmatch()."""
    return re.match("(?:" + regex + r")\Z", string, flags=flags)

def format_check(line):
    #pattern = r'((^a|^c) *"(?<= ")[A-Za-z ]*(?=")" (\((?:-)?(?:\+)?\d+(?:\.\d+)*,(?:-)?(?:\+)?\d+(?:\.\d+)*\)| )+ \n)|(^g \n$)|(^r +"(?<= ")[A-Za-z ]*(?=")" +\n)'
    pattern = r'((^ *a|^ *c) *"(?<= ")[A-Za-z ]+(?=")" (\( *(?:(?:-)|(?:\+))?\d+ *(?:\.\d+)*, *(?:(?:-)|(?:\+))?\d+ *(?:\.\d+)*\)| )+ \n)|(^ *g *\n$)|(^ *r +"(?<= ")[A-Za-z ]+(?=")" +\n)'
    #deal with -+ ++ -- case
    return fullmatch(pattern, line)

def parser(line):
    # format check promote error message 
    option =  re.findall(r'^ *a(?= )|^ *r(?= )|^ *g(?= )|^ *c(?= )', line)
    try: 
        option = option.replace(" ", "")
    except: 
        pass  
    #streetname =  re.search(r' "((?:\\.|[^"\\])*)" ', line)
    streetname =  re.search(r'(?<= ")[A-Za-z ]+(?=" )', line)
    # vertices = re.findall(r'\((?:-)?(?:\+)?\d+(?:\.\d+)*,(?:-)?(?:\+)?\d+(?:\.\d+)*\)',line)
    vertices = re.findall(r'\( *(?:(?:-)|(?:\+))?\d+ *(?:\.\d+)*, *(?:(?:-)|(?:\+))?\d+ *(?:\.\d+)*\)',line) #deal with -+ ++ -- case 
    return  option, streetname, vertices 

def convert_tuple(verticies):
    # convert vertices from string format to float tuples 
    coords = []
    for line in verticies:
        line = line.strip('()\n')  # Get rid of the newline and parentheses
        line = line.split(',')  # Split into two parts
        c = tuple(float(x) for x in line)  # Make the tuple
        coords.append(c)
    return coords 


def intersect(a, b, c, d):
    de1 = (Decimal(b[1] - a[1]) * Decimal(c[0] - d[0]) - Decimal(b[0] - a[0]) * Decimal(c[1] - d[1]))
    de2 = (Decimal(b[0] - a[0]) * Decimal(c[1] - d[1]) - Decimal(b[1] - a[1]) * Decimal(c[0] - d[0]))
    if de1 != 0 or de2 != 0:
        x = (Decimal(b[0] - a[0]) * Decimal(c[0] - d[0]) * Decimal(c[1] - a[1]) - Decimal(c[0]) * Decimal(b[0] - a[0]) * Decimal(c[1] - d[1]) + Decimal(a[0]) * Decimal(b[1] - a[1]) * Decimal(c[0] - d[0])) / de1
        y = (Decimal(b[1] - a[1]) * Decimal(c[1] - d[1]) * Decimal(c[0] - a[0]) - Decimal(c[1]) * Decimal(b[1] - a[1]) * Decimal(c[0] - d[0]) + Decimal(a[1]) * Decimal(b[0] - a[0]) * Decimal(c[1] - d[1])) / de2
        if (x - Decimal(a[0])) * (x - Decimal(b[0])) <= 0 and Decimal(x - Decimal(c[0])) * Decimal(x - Decimal(d[0])) <= 0 and Decimal(y - Decimal(a[1])) * Decimal(y - Decimal(b[1])) <= 0 and Decimal(y - Decimal(c[1])) * Decimal(y - Decimal(d[1])) <= 0:
            return (x, y)
        else:
            return None 
    # elif collinear(a,c,d) and collinear(b,c,d):
    #     print( "collinear true")
    #     if (c[0]<=a[0]<=d[0] or c[0]>=a[0]>=d[0]) and (c[0]<=b[0]<=d[0] or c[0]>=b[0]>=d[0]) and (c[1]<=a[1]<=d[1] or c[1]>=a[1]>=d[1]) and (c[1]<=b[1]<=d[1] or c[1]>=b[1]>=d[1]):
    #         print( "overlap true")
    #         return (a[0], a[1])
    #     else: 
    #         return None
    # else:
    #     return None


def collinear(p0, p1, p2):
    x1, y1 = p1[0] - p0[0], p1[1] - p0[1]
    x2, y2 = p2[0] - p0[0], p2[1] - p0[1]
    return abs(x1 * y2 - x2 * y1) < 1e-12

def to_matrix(l, n):
    return [l[i:i+n] for i in range(0, len(l), n)]





class Edge(object):
    def __init__(self,vertice={},edge=[], number = 1):
        self.vertice = vertice
        self.inv_map_vert = vertice
        self.history = vertice
        self.inv_map_hist = vertice
        self.edge = edge
        self.number = number

    def reset(self):
        self.__init__()

    def invert_vertice(self):
        self.inv_map_vert = {v: k for k, v in self.vertice.iteritems()}

    def add_vertice(self, vertice, option='g'):
    # backup 
        if option == 'b':
            #self.history = self.vertice
            self.history = dict(self.history.items() + self.vertice.items())
            self.vertice = {}
            self.inv_map_hist = {v: k for k, v in self.history.iteritems()}
            if self.history:
                self.number = self.history.keys()[-1]+1 # find previous largest key
            else:
                self.number = 1 # without anykey 

        if vertice in self.history.values() and vertice not in self.vertice.values():
            self.vertice[self.inv_map_hist[vertice]] = vertice # reuse previous key
            return 1
        else:
            if vertice in self.vertice.values():
                #print('Error: vertice \"{0}\" already exists'.format(vertice))
                #print(self.vertice) #debug
                return None
            elif vertice not in self.vertice.values() and vertice: 
                self.vertice[self.number] = vertice
                self.number +=1
                #print(self.vertice) # debug 
                return 1 
       
    def add_edge(self, number1, number2):
        self.edge.append(number1)
        self.edge.append(number2)

    def clear_edge(self):
        self.edge = []

    def print_vertice(self):
        print("V = {")
        for index, value in self.vertice.iteritems():
            value1 = "{0:.2f}".format(float(value[0]))
            value2 = "{0:.2f}".format(float(value[1]))
            print(" {}: ({},{})".format(index, value1, value2))
            #print(" {}: {}".format(index, "{0:.2f}".format(float(value[0])),))
        print("}")

    def print_edge(self):

        edgematrix= to_matrix(self.edge,2)



        print( "E = {")
        it = iter(self.edge)
        for x in it:
            print(" <{},{}>,".format(x, next(it)))
        # for i in range(self.edge.size):
        #     self.edge.get(i), yourList.get(i+1)
        #     print(" <{}, {}>,".format(self.edge.get(i), self.edge.get(i+1)))
        #     i+=2
        print("}")

    def check_vertice(self):
        for index, value in self.vertice.iteritems():
            # xA=convert_tuple(value)[0]
            # yA=convert_tuple(value)[1]
            xA=Decimal(value[0])
            yA=Decimal(value[1])
            it = iter(self.edge)
            counter = -1
            for i in it:
                counter = counter + 1
                j = next(it)
                x1 = Decimal(self.vertice[i][0])
                y1 = Decimal(self.vertice[i][1])
                x2 = Decimal(self.vertice[j][0])
                y2 = Decimal(self.vertice[j][1])
                v1 = [x2-x1, y2-y1]   # Vector 1
                v2 = [x2-xA, y2-yA]   # Vector 1
                xp = v1[0]*v2[1] - v1[1]*v2[0] 
                if round(xp, 2) ==0 and not((xA == x1 or xA == x2) and (yA == y1 or yA == y2)) and (((x1 <=xA) and (xA <= x2)) or ((x1 >= xA) and (xA >= x2))) and (((y1 <=yA) and (yA <= y2)) or ((y1 >= yA) and (yA >= y2))):
                    # print("found it")
                    # print("i,j:",i,j)
                    # print("counter:", counter)
                    # print("index:", index)
                    # print("xA:",xA, "yA:", yA )
                    # print("x1:",x1, "y1:", y1 )
                    # print("x2:",x2, "y2:", y2 )
                    #if i != index:
                    # it1 = iter(self.edge)
                    # bool add = true 
                    # for x in it1:
                    #     print(" <{},{}>,".format(x, next(it1)))
                    #     if x == index and next(it1)==:
                    #         bool add = False


                    self.edge.insert(2*counter+1, index)
                    self.edge.insert(2*counter+1, index)
                    self.check_vertice()
                    return None 


class Graph(object):
    def __init__(self,state={},vertices={},edges=set([]),intersec=set([])):
        self.state = state
        self.vertices = vertices
        self.edges = edges
        self.intersec = intersec

    def add_street(self, streetname, vertices):
        if vertices and streetname:
            if streetname.lower() in self.state:
                print('Error: street \"{0}\" already exists'.format(streetname))
            else: 
                self.state[streetname.lower()] = convert_tuple(vertices)
                #print(self.state) # debug 
        else: 
            print('Error: Mising streetname and/or vertices')

    def change_street(self, streetname,vertices):
        if vertices and streetname:
            if streetname.lower() in self.state:
                self.state[streetname.lower()] = convert_tuple(vertices)
                #print(self.state) # debug 
            else:
                print('Error: Street \"{0}\" does not exist'.format(streetname))
        else:
            print('Error: Missing streetname or vertices')

    def remove_street(self,streetname):
        if streetname.lower() in self.state:
            del self.state[streetname.lower()]
            #print(self.state) # debug 
        elif streetname:
            print('Error: street \"{0}\" does not exist'.format(streetname))
        else:
            print('Error: Please specify the street to remove.')

    def output(self,edge):
        edge.clear_edge()
        edge.add_vertice(None,'b') # backup 
        local_state = self.state 
        for street1 in local_state:
            for street2 in local_state:
                if (street2 != street1):
                    #print(street1,street2)
                    for a, b in zip(local_state[street1], local_state[street1][1:]):
                        #itertools.izip
                        #print (street1, a, b)
                        for c, d in zip (local_state[street2], local_state[street2][1:]):
                            inter=intersect(a,b,c,d)
                            overlap = collinear(a,c,d) and collinear(b,c,d)
                            if inter:
                                #print('INTER:',street1, street2, inter)
                                is_a = edge.add_vertice(a)#intercept 
                                is_b = edge.add_vertice(b)
                                is_c = edge.add_vertice(c)
                                is_d = edge.add_vertice(d)
                                is_i = edge.add_vertice(inter)
                                #inter_x = convert_tuple(inter)[0]
                                #inter_y = convert_tuple(inter)[1]
                                # TBD: last intersect
                                # if (inter) not in local_state[street1]: 
                                #     local_state[street1].append(inter)
                                # if (inter) not in local_state[street1]: 
                                #     local_state[street1].append(inter)

                                # if (inter) not in street2:
                                #     local_state[street2].append(inter)
                            # if (intersection.x, intersection.y) not in streets[i].points_pool:
                            # streets[i].points.insert(a + 1, intersection)
                            # streets[i].points_pool.add((intersection.x, intersection.y))
                            elif overlap:
                                if not((c[0] > a[0] and c[0] > b[0] and d[0] > a[0] and d[0] > b[0]) or (a[0] > c[0] and a[0] > d[0] and b[0] > c[0] and b[0] > d[0])):
                                    if not((c[1] > a[1] and c[1] > b[1] and d[1] > a[1] and d[1] > b[1]) or (a[1] > c[1] and a[1] > d[1] and b[1] > c[1] and b[1] > d[1])):
                                        ol_a = edge.add_vertice(a) #overlap 
                                        ol_b = edge.add_vertice(b)
                                        ol_c = edge.add_vertice(c)
                                        ol_d = edge.add_vertice(d)
                                        edge.invert_vertice()

                                        coords = [list(a)]+[list(b)]+[list(c)]+[list(d)]
                                        coords = sorted(coords , key=lambda k: [k[1], k[0]])
                                        try:
                                        	smallest = tuple(coords[0])
                                        	biggest = tuple(coords[-1])
                                        	if ol_a or ol_b or ol_c or ol_d:
                                        		edge.add_edge(edge.inv_map_vert[smallest], edge.inv_map_vert[biggest])
                                        except:
                                        	pass 

                            try:
                                if is_a or is_b and (is_i or inter == a or inter ==b):
                                    edge.invert_vertice()
                                    if edge.inv_map_vert[a] is not edge.inv_map_vert[inter]:
                                        edge.add_edge(edge.inv_map_vert[a], edge.inv_map_vert[inter])
                                    #print(edge.inv_map_vert[a], edge.inv_map_vert[inter])
                                    edge.invert_vertice()
                                    if edge.inv_map_vert[inter] is not edge.inv_map_vert[b]:
                                        edge.add_edge(edge.inv_map_vert[inter], edge.inv_map_vert[b])
                                    #print(edge.inv_map_vert[inter], edge.inv_map_vert[b])

                                if is_c or is_d and (is_i or inter == c or inter ==d):
                                    edge.invert_vertice()
                                    if edge.inv_map_vert[c] is not edge.inv_map_vert[inter]:
                                        edge.add_edge(edge.inv_map_vert[c], edge.inv_map_vert[inter])
                                    #print(edge.inv_map_vert[c], edge.inv_map_vert[inter])
                                    edge.invert_vertice()
                                    if edge.inv_map_vert[inter] is not edge.inv_map_vert[d]:
                                        edge.add_edge(edge.inv_map_vert[inter], edge.inv_map_vert[d])
                                    #print(edge.inv_map_vert[inter], edge.inv_map_vert[d])
                            except: 
                                pass 

        edge.check_vertice()
        edge.print_vertice()
        edge.print_edge()
#def __init__(self):

def main():
    try:
        edge = Edge()
        graph=Graph()   

        ### YOUR MAIN CODE GOES HERE    

        ### sample code to read from stdin.
        ### make sure to remove all spurious print statements as required
        ### by the assignment
        # options = {'a' : 'add_street','c' : 'change_street','r' : 'remove_street',}
        #print ('Please use with the following options: a(add), r(remove), c(change), g(graph)')
        #print('Command format: a|c|r "Weber Street" | (2,-1) (2,2) (5,5) (5,6) (3,8)|g')
        while True:
            # print('Command format:a "King Street S" (4,2) (4,8)')
            # print('Command format:a "Davenport Road" (1,4) (5,8)')
            # print('Command format:c "Weber Street" (2,1) (2,2)')
            line = sys.stdin.readline().replace('\n',' \n')
            if format_check(line):
                #try:
                option, streetname, vertices = parser(line)
                if option[0].replace(" ", "") != 'g':
                    streetname = streetname.group(0)
                if option[0].replace(" ", "") == 'a':  
                    graph.add_street(streetname, vertices)
                elif option[0].replace(" ", "") == 'c':
                    graph.change_street(streetname, vertices)
                    #graph.change(edge)
                elif option[0].replace(" ", "") == 'r':
                    graph.remove_street(streetname)
                elif option[0].replace(" ", "") == 'g':
                    graph.output(edge)
                    # TBD final output
                else:
                    pass 
                #except: print('Error: wrong format 1') 
            else:
                print('Error: wrong format')    
    

            #except: print('failed')    

        #print 'Finished reading input'
        # return exit code 0 on successful termination
        sys.exit(0)

    except:
        pass 


if __name__ == '__main__':
    main()
