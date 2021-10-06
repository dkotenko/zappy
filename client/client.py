import sys
import socket
import select
from enum import Enum

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
    

def main(dev=False):
    s, world_size = connect('127.0.0.1', int(sys.argv[1]))
    r = ''
    last_cmd = ''
    if dev:
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
                sock_send(c, 'connect_nbr')
            else:
                continue 
            r = sock_readline(s)
            last_cmd = c
            
    if sys.stdin in r:
        pass

    s.close()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: python3 {} <port> <team_name> [dev]'.format(sys.argv[0]))
        exit(1)
    if len(sys.argv) >= 4 and sys.argv[3] == 'dev':
        main(dev=True)
    # try:
    #     main()
    # except Exception as e:
    #     print(e)
