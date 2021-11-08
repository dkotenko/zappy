import os
from enum import Enum
from server import Command, Message


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

    def __repr__(self):
        return self.__str__()

    def add_stone(self, name):
        if name == 'linemate':
            self.li += 1
        elif name == 'deraumere':
            self.de += 1
        elif name == 'sibur':
            self.si += 1
        elif name == 'mendiane':
            self.me += 1
        elif name == 'phiras':
            self.ph += 1
        elif name == 'thystame':
            self.th += 1


class Player:
    class State(Enum):
        INTRODUCING = 0
        COLLECTING = 1
        MEETING = 2
        INCANTATION = 3

    state = State.INTRODUCING
    name = str(os.getpid())
    my_info = PlayerInfo()
    players_info = {}
    world_x = 0
    world_y = 0
    command_list = []
    last_cmd = ''
    meet_target = None

    def __init__(self, world_size):
        self.world_x = int(world_size[0])
        self.world_y = int(world_size[1])

    def play(self, result, messages):
        self._handle_messages(messages)
        if self.state == self.State.INTRODUCING:
            return self._introduce(result, messages)
        if self.state == self.State.COLLECTING:
            return self._collect(result, messages)
        if self.state == self.State.MEETING:
            return self._meet(result, messages)
        if self.state == self.State.INCANTATION:
            return self._incantate(result, messages)

    def _handle_messages(self, messages):
        while messages:
            m = messages.pop(0)
            print("got message: " + str(m.t) + ' ' + m.data)
            if m.t == Message.Type.VOICE:
                data_splited = m.data.split()
                if data_splited[1] == 'hi' or data_splited[1] == 'took':
                    self.players_info[data_splited[0]] = PlayerInfo(
                        data_splited[2])
                if data_splited[1] == 'hi':
                    self.command_list.insert(
                        0,
                        Command(Command.Type.SAY,
                                self.name + ' hi ' + str(self.my_info)))
# -> 1 meet 2, (2 go to 1)
# -> 2 meet_confirm 1, (1 go to 2)
                if (data_splited[1] == 'meet' and
                        data_splited[2] == self.name):
                    self.state = self.State.MEETING
                    self.meet_target = data_splited[0]
                    self.command_list.insert(0,
                                             Command(Command.Type.SAY,
                                                     self.name +
                                                     ' meet_confirm ' +
                                                     data_splited[0]))
                if (data_splited[1] == 'meet_confirm' and
                        data_splited[2] == self.name):
                    self.state = self.State.MEETING
                    self.meet_target = data_splited[0]

            # if m.t == Message.Type.ACTUAL_LEVEL:
            #     self.my_info.lvl = int(m.data)
            #     self.state = self.State.COLLECTING

    def _introduce(self, result, messages):
        if result == '':
            return Command(Command.Type.SAY,
                           self.name + ' hi ' + str(self.my_info))
        self.state = self.State.COLLECTING
        return self._collect('', [])

    def _collect(self, result, messages):
        # return Command(Command.Type.WAIT)
        if result == '' or not self.command_list:
            self.command_list = self._generate_collect_command_list()
            # say about lvlup
            if result == '' and self.my_info.lvl != 1:
                self.command_list.insert(0, Command(Command.Type.SAY,
                                                    self.name + ' took ' +
                                                    str(self.my_info)))

        if self.last_cmd and self.last_cmd.t == Command.Type.SEE \
           and result.startswith('{'):
            striped = result.strip('{}')
            splited = striped.split(',')
            for i in range(0, len(splited)):
                content = splited[i].strip()
                content_splited = content.split(' ')
                take_list = []
                for c in content_splited:
                    if self._do_i_need_this(c):
                        take_list.append(c)
                if take_list:
                    self._take(i, take_list)
                    break

        if (self.last_cmd and self.last_cmd.t == Command.Type.TAKE_OBJECT
                and self.last_cmd.arg != 'nourriture'):
            if result == 'ok':
                self.my_info.add_stone(self.last_cmd.arg)
                self.command_list.insert(0, Command(Command.Type.SAY,
                                                    self.name + ' took ' +
                                                    str(self.my_info)))
            can_incantate = self._can_incantate()
            if can_incantate:
                if can_incantate[0] == self.name:
                    self.state = self.State.INCANTATION
                    return self._incantate('', messages)
                self.state = self.State.MEETING
                self.command_list = []
                for name in can_incantate:
                    self.command_list.append(Command(Command.Type.Say,
                                                     self.name + ' meet ' +
                                                     name))

        cmd = self.command_list.pop(0)
        self.last_cmd = cmd
        return cmd

    def _do_i_need_this(self, res):
        if res == 'nourriture':
            return True
        if self.state == self.State.MEETING:
            return False

        if self.my_info.lvl == 1:
            if res == 'linemate' and self.my_info.li < 1:
                return True
            return False
        if self.my_info.lvl == 2:
            if res == 'linamate' and self.my_info.li < 1:
                return True
            if res == 'deraumere' and self.my_info.de < 1:
                return True
            if res == 'sibur' and self.my_info.si < 1:
                return True
        if self.my_info.lvl == 3:
            if res == 'linamate' and self.my_info.li < 2:
                return True
            if res == 'sibur' and self.my_info.de < 1:
                return True
            if res == 'phiras' and self.my_info.si < 2:
                return True
        if self.my_info.lvl == 4:
            if res == 'linamate' and self.my_info.li < 1:
                return True
            if res == 'deraumere' and self.my_info.li < 1:
                return True
            if res == 'sibur' and self.my_info.li < 2:
                return True
            if res == 'phiras' and self.my_info.li < 1:
                return True
        if self.my_info.lvl == 5:
            if res == 'linamate' and self.my_info.li < 1:
                return True
            if res == 'deraumere' and self.my_info.li < 2:
                return True
            if res == 'sibur' and self.my_info.li < 1:
                return True
            if res == 'mendiane' and self.my_info.li < 3:
                return True
        if self.my_info.lvl == 6:
            if res == 'linamate' and self.my_info.li < 1:
                return True
            if res == 'deraumere' and self.my_info.li < 2:
                return True
            if res == 'sibur' and self.my_info.li < 3:
                return True
            if res == 'phiras' and self.my_info.li < 1:
                return True
        if self.my_info.lvl == 7:
            if res == 'linamate' and self.my_info.li < 2:
                return True
            if res == 'deraumere' and self.my_info.li < 2:
                return True
            if res == 'sibur' and self.my_info.li < 2:
                return True
            if res == 'mendiane' and self.my_info.li < 2:
                return True
            if res == 'phiras' and self.my_info.li < 2:
                return True
            if res == 'thystame' and self.my_info.li < 1:
                return True
        return False

    def _take(self, pos, contents):
        borders = [0, 3, 8, 15, 24, 35, 48, 63, 80]
        centers = [0, 2, 6, 12, 20, 30, 42, 56, 72]
        i = 0
        x = 0
        for i in range(0, len(borders)):
            if borders[i] >= pos:
                break
        y = i
        x = centers[i] - pos  # positive = left, negative = right
        commands = []
        for i in range(0, y):
            commands.append(Command(Command.Type.GO))
        if x != 0:
            commands.append(
                Command(Command.Type.TURN_LEFT if x > 0 else Command.Type.
                        TURN_RIGHT))
        for i in range(0, abs(x)):
            commands.append(Command(Command.Type.GO))
        for c in contents:
            commands.append(Command(Command.Type.TAKE_OBJECT, c))
        if x != 0:
            commands.append(
                Command(Command.Type.TURN_LEFT if x < 0 else Command.Type.
                        TURN_RIGHT))
        self.command_list = commands + self.command_list

    def _generate_collect_command_list(self):
        cmd_list = [
            Command(Command.Type.SEE),
            Command(Command.Type.GO),
            Command(Command.Type.SEE),
            Command(Command.Type.GO),
            Command(Command.Type.SEE),
            Command(Command.Type.TURN_LEFT),
            Command(Command.Type.SEE)
        ]
        radius = 3
        turned = False
        while radius < min(self.world_x, self.world_y):
            for i in range(0, radius):
                cmd_list.append(Command(Command.Type.GO))
                cmd_list.append(Command(Command.Type.SEE))
            cmd_list.append(Command(Command.Type.TURN_LEFT))
            cmd_list.append(Command(Command.Type.SEE))
            radius += self.my_info.lvl
            if turned:
                radius += 1
                turned = False
            else:
                turned = True

        return cmd_list

    def _meet(self, result, messages):
        if self.command_list:
            return self.command_list.pop(0)
        if not self.meet_target:
            return Command(Command.Type.WAIT)
        print('meet target = ' + self.meet_target)
        return Command(Command.Type.WAIT)

    def _can_incantate(self):
        '''
        return list of names to meet with, my name if lvl 1, empty if cannot
        '''
        print('my_info: ' + str(self.my_info) + ', ' +
              'players: ' + str(self.players_info))
        if self.my_info.lvl == 1 and self.my_info.li >= 1:
            return [self.name]
        if self.my_info.lvl == 2:
            for name in self.players_info.keys():
                player = self.players_info[name]
                li = self.my_info.li
                de = self.my_info.de
                si = self.my_info.si
                if player.lvl == 2:
                    li = li + player.li
                    de = de + player.de
                    si = si + player.si
                    if li >= 1 and de >= 1 and si >= 1:
                        return [name]
        return []

    def _incantate(self, result, messages):
        if result == '':
            return Command(Command.Type.INCANTATE)
        if result.startswith('niveau actuel'):
            splited = result.split(':')
            self.my_info.lvl = int(splited[1].strip())
        self.state = self.state.COLLECTING
        return self._collect('', messages)