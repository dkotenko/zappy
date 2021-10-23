import os
from enum import Enum
from server import Command


class PlayerInfo:
    lvl = 1
    li = 0
    de = 0
    si = 0
    me = 0
    ph = 0
    th = 0

    def __init__(self, msg=''):
        if msg == '':
            return
        splited = msg.split('_')
        self.lvl = splited[0]
        self.li = splited[1]
        self.de = splited[2]
        self.si = splited[3]
        self.me = splited[4]
        self.ph = splited[5]
        self.th = splited[6]

    def __str__(self):
        r = str(self.lvl) + '_' + \
            str(self.li) + '_' + \
            str(self.de) + '_' + \
            str(self.si) + '_' + \
            str(self.me) + '_' + \
            str(self.ph) + '_' + \
            str(self.th)
        return r


class Player:
    class State(Enum):
        INTRODUCING = 0
        COLLECTING = 1
        MEETING = 2
        INVOKATION = 3

    state = State.INTRODUCING
    name = str(os.getpid())
    my_info = PlayerInfo()
    players_info = {}
    world_x = 0
    world_y = 0
    collect_context = {}

    def __init__(self, world_size):
        self.world_x = int(world_size[0])
        self.world_y = int(world_size[1])

    def play(self, result, messages):
        if self.state == self.State.INTRODUCING:
            return self._introduce(result, messages)
        if self.state == self.State.COLLECTING:
            return self._collect(result, messages)
        if self.state == self.State.MEETING:
            return self._meet(result, messages)
        if self.state == self.State.INVOKATION:
            return self._invokate(result, messages)

    def _introduce(self, result, messages):
        print('introduce')
        if result == '':
            return Command(Command.Type.SAY,
                           self.name + ' hi ' + str(self.my_info))
        return self._collect('', [])

    def _collect(self, result, messages):
        print('collect')
        if result == '' or not self.collect_context['command_list']:
            self.collect_context['command_list'] = self._generate_collect_command_list()
        if result.startswith('{'):
            striped = result.strip('{}')
            splited = striped.split(',')
            for i in range(0, len(splited)):
                content = splited[i].strip()
                content_splited = content.split(' ')
                for c in content_splited:
                    if c != 'player':
                        self._take(i, c)
                                        
                    
            
        return self.collect_context['command_list'].pop(0)

    def _take(self, pos, content):
        borders = [0, 3, 8, 15, 24, 35, 48, 63, 80]
        i = 0
        x = 0
        for i in range(0, len(borders)):
            if borders[i] >= pos:
                
                break
        y = i
        
        


    def _generate_collect_command_list(self):
        cmd_list = [
            Command(Command.Type.SEE),
            Command(Command.Type.GO),
            Command(Command.Type.SEE),
            Command(Command.Type.GO),
            Command(Command.Type.SEE),
            Command(Command.Type.TURN_LEFT),
            Command(Command.Type.SEE)
        ]
        radius = 3
        turned = False
        while radius < min(self.world_x, self.world_y):
            for i in range(0, radius):
                cmd_list.append(Command(Command.Type.GO))
                cmd_list.append(Command(Command.Type.SEE))
            cmd_list.append(Command(Command.Type.TURN_LEFT))
            cmd_list.append(Command(Command.Type.SEE))
            radius += self.my_info.lvl
            if turned:
                radius += 1
                turned = False
            else:
                turned = True
                
        return cmd_list

    def _meet(self, result, messages):
        print('meet')
        return False

    def _invokate(self, result, messages):
        print('invokate')
        return False
