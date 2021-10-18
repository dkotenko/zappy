from enum import Enum, auto
from select import select

def canBeNumber(n):
    try:
        int(n)
        return True
    except ValueError:
        return False


class Command(Enum):
    WAIT        = auto()
    GO_FORWARD  = auto()
    TURN_LEFT   = auto()
    TURN_RIGHT  = auto()
    SEE         = auto()
    INVENTORY   = auto()
    TAKE_OBJECT = auto()
    DROP_OBJECT = auto()
    KICK        = auto()
    SAY         = auto()
    INCANTATE   = auto()
    FORK        = auto()
    CONNECT_NBR = auto()

    
class Message:

    class Type(Enum):
        VOICE        = auto()
        DEPLACEMENT  = auto()
        ACTUAL_LEVEL = auto()
    
    t = None
    source = 0
    data = ''

    def __init__(self, type_, source, data):
        self.t = type_
        self.source = source
        self.data = data


class Server:

    s = None                    # socket
    messages = []

    class State(Enum):
        BIENVENUE   = auto()
        CLIENT_NB   = auto()
        WORLD_SIZE  = auto()
        PLAYER_IDLE = auto()
        PLAYER_BUSY = auto()

    def connect(self, host, port):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((host, port))
        client_nb = -1
        state = State(0)
        while True:
            select.select([self.s], [], [])
            msg = self.read()
            if msg == '':
                break
            if state == State.BIENVENUE and msg == 'BIENVENUE':
                self.send(sys.argv[2])
                state = State.CLIENT_NB
            elif state == State.CLIENT_NB:
                client_nb = int(msg)
                print('client_nb: ' + str(client_nb))
                state = State.WORLD_SIZE
            elif state == State.WORLD_SIZE:
                world_size = msg.split(' ')
                print('world_size' + str(world_size))
                state = State.PLAYER_IDLE
                break
        return world_size

    def exec_command(cmd: Command, arg=''):
        """
        Send command and wait for valid answer. If answer is invalid, return ''
        All messages are stored at self.messages.
        If cmd == WAIT, wait for next message from server, return 'ok' on success.
        """
        s = ''
        exepted_reply = ['ok']
        if cmd == WAIT:
            pass
        if cmd == GO_FORWARD:
            s = 'avance'
        if cmd == TURN_LEFT:
            s = 'droite'
        if cmd == TURN_RIGHT:
            s = 'gauche'
        if cmd == SEE:
            s = 'voir'
            expected_reply = ['{}']
        if cmd == INVENTORY:
            s = 'inventaire'
            expected_reply = ['{}']
        if cmd == TAKE_OBJECT:
            s = 'prend ' + arg
            expected_reply.append('ko')
        if cmd == DROP_OBJECT:
            s = 'pose ' + arg
            expected_reply.append('ko')
        if cmd == KICK:
            s = 'expulse'
            expected_reply.append('ko')
        if cmd == SAY:
            s = 'broadcast ' + arg
        if cmd == INCANTATE:
            s = 'incantation'
            expected_reply = ['elevation en cours']
        if cmd == FORK:
            s = 'fork'
        if cmd == CONNECT_NBR:
            s = 'connect_nbr'
            expected_reply = ['number']
        if cmd != WAIT:
            self._send(s)
        while True:   
            select([self.s], [], [])
            r = self._read()
            for i in expected_reply:
                if (r == i) or
                (i == '{}' and r.startswith('{')) or
                (i == 'number' and canBeNumber(r)):
                    return r
            if r == 'mort':
                print("I'm dead")
                exit(0)
            elif r.startswith('message'):
                splited = r.split(',')
                self.messages.append(Message.Type.VOICE,
                                     source=splited[0].split(' ')[1],
                                     data=splited[1].strip())
            elif r.startswith('deplacement'):
                self.messages.append(Message.Type.DEPLACEMENT,
                                         source=r.split(' ')[1])
            elif r.startswith('niveau actuel:'):
                self.messages.append(Message.Type.ACTUAL_LEVEL,
                                         data=r.split(':')[1].strip())
            else:
                print('unknown message: "' + r + '"')
                break
            if cmd == WAIT:
                return 'ok'
        return ''

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
