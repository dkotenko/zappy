#!/usr/bin/env python3

import sys
import socket
import select
import os
from enum import Enum
from optparse import OptionParser

class State(Enum):
    BIENVENUE = 0
    CLIENT_NB = 1
    WORLD_SIZE = 2
    PLAYER_IDLE = 3
    PLAYER_BUSY = 4


class Server:

    def connect(self, host, port):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((host, port))
        client_nb = -1
        state = State(0)
        while True:
            r = select.select([self.s], [], [])
            msg = self.read()
            if msg == '':
                break
            if state == State.BIENVENUE and msg == 'BIENVENUE':
                self.send(sys.argv[2])
                state = State.CLIENT_NB
            elif state == State.CLIENT_NB:
                client_nb = int(msg);
                print('client_nb: ' + str(client_nb))
                state = State.WORLD_SIZE
            elif state == State.WORLD_SIZE:
                world_size = msg.split(' ')
                print('world_size' + str(world_size))
                state = State.PLAYER_IDLE
                break
        return world_size

    def read(self):
        msg = ''
        while True:
            c = self.s.recv(1).decode('ascii')
            if c == '\n':
                break
            msg += c
        return msg


    def send(self, msg):
        self.s.send(bytes(msg + '\n', 'ascii'))


class PlayerInfo:
    lvl = 1
    li = 0
    de = 0
    si = 0
    me = 0
    ph = 0
    th = 0
    
class Activity:

    server = Server()
    players = {}

    def __init__(self, server):
        self.server = server

    def check_message(self, msg):
        splited = msg.split()
        if splited[0] == 'mort':
            print("(x_x)")
            print("   \___I'm dead")
            exit(0)
        if splited[0] == 'message' and splited[3] == 'hi':
            players[splited[2]](PlayerInfo())
        


    def perform(self, msg=''):
        """
        Returns None if activity in progress, otherwise next Activity object
        cmd - server's message. Empty on first call.
        """
        pass


class ActivityAcquaintance(Activity):

    def __init__(self, s):
        super().__init__(s)

    def perform(self, msg=''):

        if msg == '':
            self.server.send('broadcast ' + str(os.getpid()) + ' hi')
            return None
        if msg == 'ok':
            return ActivityMoving(self.server)

class ActivityMoving(Activity):

    def __init__(self, s):
        super().__init__(s)

    def perform(self, msg=''):
        print("moving")
        return None

class ActivityMeeting(Activity):

    def __init__(self, s):
        super().__init__(s)

    def perform(self, msg=''):
        return None

class ActivityInvokation(Activity):

    def __init__(self, s):
        super().__init__(s)

    def perform(self, msg=''):
        return None

    

class Player:

    activity = Activity(None)

    def __init__(self, server):
        self.activity = ActivityAcquaintance(server)
    

def parse_args():
    usage = 'Usage: %prog -n <team> -p <port> [-h <hostname>] [-d]'
    parser = OptionParser(add_help_option=False, usage=usage)
    parser.add_option('-n', dest='team', type='str')
    parser.add_option('-p', dest='port', type='int')
    parser.add_option('-h', dest='hostname', type='str', default='127.0.0.1')
    parser.add_option('-d', dest='dev', action='store_true', default=False, help='Enable development mode')
    (options, args) = parser.parse_args()
    if options.team == None or options.port == None:
        parser.print_usage()
        exit(1)
    return options

def dev_mode(server):
    r = ''
    last_cmd = ''
    
    while True:
        print('\033c')
        print('last command: {} -> {}'.format(last_cmd, r))
        print('-------------------------------------------------------------')
        # print voir
        print('f - forward (avance), l - left (droite), r - right (gauche),')
        print('s - see (voir), i - inventory (inventaire), t - take <obj> (prend),')
        print('d - drop <obj> (pose), k - kick (expulse), b - broadcast <text>,')
        print('x - incantation, f - fork, c - connect_nbr')
        c = input('-> ')
        if c == 'f':
            server.send('avance')
        elif c == 'l':
            server.send('droite')
        elif c == 'r':
            server.send('gauche')
        elif c == 's':
            server.send('voir')
        elif c == 'i':
            server.send('inventaire')
        elif c == 't':
            server.send('prend')
        elif c == 'd':
            server.send('pose')
        elif c == 'k':
            server.send('expulse')
        elif c == 'b':
            server.send('broadcast')
        elif c == 'x':
            server.send('incantation')
        elif c == 'f':
            server.send('fork')
        elif c == 'c':
            server.send('connect_nbr')
        else:
            continue 
        r = server.read()
        last_cmd = c

def prod_mode(server, world_size):
    player = Player(server)
    player.activity.perform()
    while True:
        select.select([server.s], [], [])
        msg = server.read()
        activity = player.activity.perform(msg)
        if activity != None:
            player.activity = activity
            player.activity.perform()

def main(options):
    server = Server()
    world_size = server.connect(options.hostname, options.port)
    if options.dev:
        dev_mode(server)
    else:
        prod_mode(server, world_size)
            
    s.close()

if __name__ == '__main__':
    # try:
    #     main(parse_args())
    # except Exception as e:
    #     print(e)

    main(parse_args())
