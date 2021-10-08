#!/usr/bin/env python3

import sys
import socket
import select
from enum import Enum
from optparse import OptionParser

class State(Enum):
    BIENVENUE = 0
    CLIENT_NB = 1
    WORLD_SIZE = 2
    GAME = 3

def sock_readline(s):
    msg = ''
    while True:
        c = s.recv(1).decode('ascii')
        if c == '\n':
            break
        msg += c
    return msg


def sock_send(s, msg):
    s.send(bytes(msg + '\n', 'ascii'))


def connect(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    client_nb = -1
    state = State(0)
    while True:
        r = select.select([s], [], [])
        msg = sock_readline(s)
        if msg == '':
            break
        if state == State.BIENVENUE and msg == 'BIENVENUE':
            sock_send(s, sys.argv[2])
            state = State.CLIENT_NB
        elif state == State.CLIENT_NB:
            client_nb = int(msg);
            print('client_nb: ' + str(client_nb))
            state = State.WORLD_SIZE
        elif state == State.WORLD_SIZE:
            world_size = msg.split(' ')
            print('world_size' + str(world_size))
            break 
    return s, world_size


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
    

def main(options):
    s, world_size = connect(options.hostname, options.port)
    r = ''
    last_cmd = ''
    if options.dev:
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
                sock_send(s, 'avance')
            elif c == 'l':
                sock_send(s, 'droite')
            elif c == 'r':
                sock_send(s, 'gauche')
            elif c == 's':
                sock_send(s, 'voir')
            elif c == 'i':
                sock_send(s, 'inventaire')
            elif c == 't':
                sock_send(s, 'prend')
            elif c == 'd':
                sock_send(s, 'pose')
            elif c == 'k':
                sock_send(s, 'expulse')
            elif c == 'b':
                sock_send(s, 'broadcast')
            elif c == 'x':
                sock_send(s, 'incantation')
            elif c == 'f':
                sock_send(s, 'fork')
            elif c == 'c':
                sock_send(s, 'connect_nbr')
            else:
                continue 
            r = sock_readline(s)
            last_cmd = c
    else:
        pass
            
    

    s.close()

if __name__ == '__main__':
    try:
        main(parse_args())
    except Exception as e:
        print(e)
