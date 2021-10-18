import os
from enum import Enum


class PlayerInfo:
    lvl = 1
    li = 0
    de = 0
    si = 0
    me = 0
    ph = 0
    th = 0

    def __init__(self, msg = ''):
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
        MOVING = 1
        MEETING = 2
        INVOKATION = 3

    server = None
    state_queue = [State.INTRODUCING, State.MOVING]
    name = str(os.getpid())
    my_info = PlayerInfo()
    players_info = {}

    def __init__(self, server):
        self.server = server

    def play(self, msg):
        self._check_message(msg)
        state = self.state_queue[0]
        r = False
        if state == self.State.INTRODUCING:
            r = self._introduce(msg)
        if state == self.State.MOVING:
            r = self._move(msg)
        if state == self.State.MEETING:
            r = self._meet(msg)
        if state == self.State.INVOKATION:
            r = self._invokate(msg)
        if r == True:
            state_queue.pop(0)
            self.play('')

    def _check_message(self, msg):
        if msg == '':
            return 
        splited = msg.split()
        if splited[0] == 'mort':
            print("I'm dead")
            exit(0)
        if splited[0] == 'message' and splited[3] == 'hi':
            self.players_info[splited[2]](PlayerInfo(splited[4]))
            self.state_queue.append(self.State.INTRODUCING)

    def _introduce(self, msg):
        print('introduce')
        if msg == '':
            self.server.send('broadcast ' + self.name + ' hi ' + str(self.my_info))
        if msg == 'ok':
            return True
        return False

    def _move(self, msg):
        print('move')
        return False

    def _meet(self, msg):
        print('meet')
        return False

    def _invokate(self, msg):
        print('invokate')
        return False


    # [MOVING]
