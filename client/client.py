#!/usr/bin/env python3

import select

from optparse import OptionParser
from player import Player
from server import Server


def parse_args():
    usage = 'Usage: %prog -n <team> -p <port> [-h <hostname>] [-d]'
    parser = OptionParser(add_help_option=False, usage=usage)
    parser.add_option('-n', dest='team', type='str')
    parser.add_option('-p', dest='port', type='int')
    parser.add_option('-h', dest='hostname', type='str', default='127.0.0.1')
    parser.add_option('-d',
                      dest='dev',
                      action='store_true',
                      default=False,
                      help='Enable development mode')
    (options, args) = parser.parse_args()
    if options.team is None or options.port is None:
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
        print('s - see (voir), i - inventory (inventaire),')
        print('t - take <obj> (prend), d - drop <obj> (pose),')
        print('k - kick (expulse), b - broadcast <text>,')
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
    player = Player(world_size)
    result = ''
    while True:
        cmd = player.play(result, server.messages)
        result = server.exec_command(cmd)


def main(options):
    server = Server()
    world_size = server.connect(options.hostname, options.port)
    if options.dev:
        dev_mode(server)
    else:
        prod_mode(server, world_size)


if __name__ == '__main__':
    # try:
    #     main(parse_args())
    # except Exception as e:
    #     print(e)

    main(parse_args())
