import sys
from enum import Enum, auto
import select
import socket
import fcntl
import termios
import array


def canBeNumber(n):
    try:
        int(n)
        return True
    except ValueError:
        return False


class Command:

    class Type(Enum):
        WAIT = auto()
        GO = auto()
        TURN_LEFT = auto()
        TURN_RIGHT = auto()
        SEE = auto()
        INVENTORY = auto()
        TAKE_OBJECT = auto()
        DROP_OBJECT = auto()
        KICK = auto()
        SAY = auto()
        INCANTATE = auto()
        FORK = auto()
        CONNECT_NBR = auto()

    t = None
    arg = ''

    def __init__(self, t, arg=''):
        self.t = t
        self.arg = arg

    def __str__(self):
        return str('[' + str(self.t) + ' "' + self.arg + '"]')


class Message:
    class Type(Enum):
        VOICE = auto()
        DEPLACEMENT = auto()
        ACTUAL_LEVEL = auto()

    t = None
    source = 0
    data = ''

    def __init__(self, type_, source, data):
        self.t = type_
        self.source = source
        self.data = data


class Server:

    s = None  # socket
    messages = []

    class State(Enum):
        BIENVENUE = auto()
        CLIENT_NB = auto()
        WORLD_SIZE = auto()
        PLAYER_IDLE = auto()
        PLAYER_BUSY = auto()

    def connect(self, host, port):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((host, port))
        client_nb = -1
        state = self.State.BIENVENUE
        while True:
            select.select([self.s], [], [])
            msg = self._read()
            if msg == '':
                break
            if state == self.State.BIENVENUE and msg == 'BIENVENUE':
                self._send(sys.argv[2])
                state = self.State.CLIENT_NB
            elif state == self.State.CLIENT_NB:
                client_nb = int(msg)
                print('client_nb: ' + str(client_nb))
                state = self.State.WORLD_SIZE
            elif state == self.State.WORLD_SIZE:
                world_size = msg.split(' ')
                print('world_size' + str(world_size))
                state = self.State.PLAYER_IDLE
                break
        return world_size

    def exec_command(self, cmd: Command):
        """
        Send command and wait for valid answer. If answer is invalid,
        return 'invalid'. All messages are stored at self.messages. If cmd ==
        WAIT, wait for next message from server, return 'ok' on
        success.
        """
        s = ''
        expected_reply = ['ok']
        if cmd.t == Command.Type.WAIT:
            pass
        if cmd.t == Command.Type.GO:
            s = 'avance'
        if cmd.t == Command.Type.TURN_LEFT:
            s = 'droite'
        if cmd.t == Command.Type.TURN_RIGHT:
            s = 'gauche'
        if cmd.t == Command.Type.SEE:
            s = 'voir'
            expected_reply = ['{}']
        if cmd.t == Command.Type.INVENTORY:
            s = 'inventaire'
            expected_reply = ['{}']
        if cmd.t == Command.Type.TAKE_OBJECT:
            s = 'prend ' + cmd.arg
            expected_reply.append('ko')
        if cmd.t == Command.Type.DROP_OBJECT:
            s = 'pose ' + cmd.arg
            expected_reply.append('ko')
        if cmd.t == Command.Type.KICK:
            s = 'expulse'
            expected_reply.append('ko')
        if cmd.t == Command.Type.SAY:
            s = 'broadcast ' + cmd.arg
        if cmd.t == Command.Type.INCANTATE:
            s = 'incantation'
            expected_reply = ['elevation en cours']
        if cmd.t == Command.Type.FORK:
            s = 'fork'
        if cmd.t == Command.Type.CONNECT_NBR:
            s = 'connect_nbr'
            expected_reply = ['number']
        if cmd.t != Command.Type.WAIT:
            self._send(s)
        while True:
            select.select([self.s], [], [])
            buf = array.array('i', [0])
            fcntl.ioctl(self.s, termios.FIONREAD, buf, 1)
            if buf[0] == 0:
                print('server error')
                exit(0)
            r = self._read()
            for i in expected_reply:
                if ((r == i) or (i == '{}' and r.startswith('{'))
                        or (i == 'number' and canBeNumber(r))):
                    return r
            if r == 'mort':
                print("I'm dead")
                exit(0)
            elif r.startswith('message'):
                splited = r.split(',')
                self.messages.append(Message(Message.Type.VOICE,
                                             source=splited[0].split(' ')[1],
                                             data=splited[1].strip()))
            elif r.startswith('deplacement'):
                self.messages.append(Message(Message.Type.DEPLACEMENT,
                                             source=r.split(' ')[1]))
            elif r.startswith('niveau actuel:'):
                self.messages.append(Message(Message.Type.ACTUAL_LEVEL,
                                             data=r.split(':')[1].strip()))
            else:
                print('unknown message: "' + r + '"')
                break
            if cmd == Command.Type.WAIT:
                return 'ok'
        return 'invalid'

    def _read(self):
        msg = ''
        while True:
            c = self.s.recv(1).decode('ascii')
            if c == '\n':
                break
            msg += c
        return msg

    def _send(self, msg):
        self.s.send(bytes(msg + '\n', 'ascii'))
