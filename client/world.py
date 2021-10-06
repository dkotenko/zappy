class Cell:
    content = []
    
    def __init__(self, content):
        self.content = content

        
    def add(self, data):
        self.content.append(data)

    def __str__(self):
        s = ''
        for c in self.content:
            s += c[0]
        return s
        
class World:
    height = 0
    widht = 0
    m = []

    def __init__(self, world_size):
        self.width = world_size[0]
        self.height = world_size[1]
        for i in range(0, self.height):
            row = []
            for j in range(0, self.width):
                row.append(Cell([]))
            self.m.append(row)

    def __str__(self):
        s = ''
        for row in self.m:
            for cell in row:
                s += '|{5s}|'.format(cell)
            s += '\n'
        return s

    def set(self, x, y, what):
        self.m[x][y].add(what)
