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

    def __init__(self, msg):
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
    state = State.INTRODUCING
    name = str(os.getpid())
    my_info = PlayerInfo()
    players_info = {}
    next_state = State.MOVING

    def __init__(self, server):
        self.server = server

    def play(self, msg):
        self.check_message(self, msg)

        if self.state == self.State.INTRODUCING:
            self._introduce(msg)
        if self.state == self.State.MOVING:
            self._move(msg)
        if self.state == self.State.MEETING:
            self.f_meet(msg)
        if self.state == self.State.INVOKATION:
            self._invokate(msg)

    def check_message(self, msg):
        splited = msg.split()
        if splited[0] == 'mort':
            print("I'm dead")
            exit(0)
        if splited[0] == 'message' and splited[3] == 'hi':
            self.players_info[splited[2]](PlayerInfo(splited[4]))
            self._introduce('')
        pass

    def _introduce(self, msg):
        if msg == '':
            self.server.send('broadcast ' + self.name + ' hi ' + self.my_info)
        if msg == 'ok':
            self.state = self.next_state
            self.next_state = None

    def _move(self, msg):
        print('move')

    def _meet(self, msg):
        pass

    def _invokate(self, msg):
        pass
