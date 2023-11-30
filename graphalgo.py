class Node:
    def __init__(self, data):
        self.next = None
        self.rep = None
        self.data = data

class Edge:
    def __init__(self):
        self.value1 = 0
        self.value2 = 0

class CC:
    def __init__(self):
        self.head = None
        self.tail = None
        self.next = None
        self.prev = None
        self.len = 0

class UnionFind:
    def __init__(self):
        self.start = CC()

    def __del__(self):
        temp = self.start
        while temp is not None:
            next_CC = temp.next
            node_temp = temp.head
            while node_temp is not None:
                next_node = node_temp.next
                del node_temp
                node_temp = next_node
            del temp
            temp = next_CC

    def make_set(self, x):
        temp = self.start.next

        new_CC = CC()
        new_CC.len = 1
        new_CC.next = temp
        self.start.next = new_CC
        new_CC.prev = self.start

        if temp is not None:
            temp.prev = new_CC

        new_CC.head = Node(x)
        new_CC.tail = new_CC.head
        new_CC.head.rep = new_CC

    def find_set(self, val):
        CC_iterator = self.start.next

        while CC_iterator is not None:
            node_iterator = CC_iterator.head

            while node_iterator is not None:
                if node_iterator.data == val:
                    return node_iterator.rep

                node_iterator = node_iterator.next
            CC_iterator = CC_iterator.next

        return None

    def link(self, x, y):
        if x != y:
            max_CC = x if x.len > y.len else y
            min_CC = y if x.len > y.len else x

            max_CC.len += min_CC.len

            changin_pt = min_CC.head

            max_CC.tail.next = min_CC.head
            max_CC.tail = min_CC.tail

            while changin_pt is not None:
                changin_pt.rep = max_CC
                changin_pt = changin_pt.next

            if min_CC.prev is not None:
                min_CC.prev.next = min_CC.next

            if min_CC.next is not None:
                min_CC.next.prev = min_CC.prev

            del min_CC

    def union_op(self, temp):
        if temp.value1 != temp.value2:
            self.link(self.find_set(temp.value1), self.find_set(temp.value2))

    def update_edge(self, given_edge, fin):
        if not fin.eof():
            line = fin.readline().split()
            given_edge.value1 = int(line[0])
            given_edge.value2 = int(line[1])

if __name__ == "__main__":
    num_vertex = 4039
    num_edges = 88234
    num_iter = 10
    file_path = "assignment 2/disjoint dataset/facebook_combined.txt"

    avg_time = 0

    for master in range(num_iter):
        start_time = time.time()

        base = UnionFind()

        for i in range(num_vertex):
            base.make_set(i)

        with open(file_path, 'r') as fin:
            temp = Edge()
            for i in range(num_edges):
                base.update_edge(temp, fin)
                base.union_op(temp)

                print(f"{(i / (num_edges - 1)) * 100}% done", end='\r')

        end_time = time.time()

        time_taken = (end_time - start_time) * 1000
        avg_time += time_taken
        print(f"Time Taken: {time_taken}ms")

    avg_time /= num_iter
    print(f"Average Time Taken: {avg_time}ms")
