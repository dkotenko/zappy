#!/usr/bin/env python3

import datetime

from optparse import OptionParser
from player import Player
from server import Server, Command


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
    parser.add_option('-m',
                      dest='music',
                      action='store_true',
                      default=False,
                      help='Enable ghost-n-goblin music')
    parser.add_option('-s',
                      dest='sound',
                      action='store_true',
                      default=False,
                      help='Enable sound effects')
    (options, args) = parser.parse_args()
    if options.team is None or options.port is None:
        parser.print_usage()
        exit(1)
    return options


def dev_mode(server):
    last_cmd = ''
    print('g - go (avance), l - left (gauche), r - right (droite),')
    print('s - see (voir), i - inventory (inventaire),')
    print('t - take <obj> (prend), d - drop <obj> (pose),')
    print('k - kick (expulse), b - broadcast <text>,')
    print('x - incantation, f - fork, c - connect_nbr')
    print('w - wait (check for messages)')
    print('-------------------------------------------------------------')
    while True:
        inp = input('-> ')
        splited = inp.split(' ')
        c = splited.pop(0)
        arg = ' '.join(splited)
        if c == 'g':
            r = server.exec_command(Command(Command.Type.GO))
        elif c == 'l':
            r = server.exec_command(Command(Command.Type.TURN_LEFT))
        elif c == 'r':
            r = server.exec_command(Command(Command.Type.TURN_RIGHT))
        elif c == 's':
            r = server.exec_command(Command(Command.Type.SEE))
        elif c == 'i':
            r = server.exec_command(Command(Command.Type.INVENTORY))
        elif c == 't':
            r = server.exec_command(Command(Command.Type.TAKE_OBJECT, arg))
        elif c == 'd':
            r = server.exec_command(Command(Command.Type.DROP_OBJECT, arg))
        elif c == 'k':
            r = server.exec_command(Command(Command.Type.KICK))
        elif c == 'b':
            r = server.exec_command(Command(Command.Type.SAY, arg))
        elif c == 'x':
            r = server.exec_command(Command(Command.Type.INCANTATE))
        elif c == 'f':
            r = server.exec_command(Command(Command.Type.FORK))
        elif c == 'c':
            r = server.exec_command(Command(Command.Type.CONNECT_NBR))
        elif c == 'w':
            r = server.exec_command(Command(Command.Type.WAIT))
        if r and r != 'w':
            print('response: ' + r)
        if server.messages:
            print('messages:')
            while server.messages:
                m = server.messages.pop(0)
                print(m.t, m.data)


def prod_mode(server, world_size):
    player = Player(world_size)
    result = ''
    while True:
        cmd = player.play(result, server.messages)
        print('[' + str(datetime.datetime.now().time()) +
              '] exec', cmd.t, cmd.arg)
        result = server.exec_command(cmd)
        print('[' + str(datetime.datetime.now().time()) +
              '] result', '"' + result + '"')


def main(options):
    server = Server(options)
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
