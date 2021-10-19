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

    def __init__(self, server):
        self.server = server

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

    def _collect(self, result, messages, context={}):
        print('collect')

        command_list = self._generate_collect_command_list()

        return False

    def _generate_collect_command_list(self, radius):
        cmd_list = []
                
        
        return cmd_list

    def _meet(self, result, messages):
        print('meet')
        return False

    def _invokate(self, result, messages):
        print('invokate')
        return False
