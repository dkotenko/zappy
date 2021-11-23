import os
from typing import Dict, List
from enum import Enum
from server import Command, Message
from stones import StonesPack, GameRules


class MessageVoice:
    source = ''
    command = ''
    data = ''

    def __init__(self, source, command, data):
        self.source = source
        self.command = command
        self.data = data

    def fromStr(raw: str):
        splited = raw.split()
        return MessageVoice(splited[0], splited[1], splited[2])

    def toCommand(self) -> Command:
        return Command(Command.Type.SAY,
                       self.source + ' ' +
                       self.command + ' ' +
                       self.data)


class PlayerInfo:
    lvl = 1
    stones_pack = StonesPack()

    def __init__(self, msg=''):
        if msg == '':
            return
        splited = msg.split('_')
        self.lvl = int(splited[0])
        self.stones_pack = StonesPack(int(splited[1]), int(splited[2]),
                                      int(splited[3]), int(splited[4]),
                                      int(splited[5]), int(splited[6]))

    def __str__(self):
        return str(self.lvl) + '_' + str(self.stones_pack)

    def __repr__(self):
        return self.__str__()


class Player:
    class State(Enum):
        INTRODUCING = 0
        COLLECTING = 1
        MEETING = 2
        INCANTATION = 3

    state = State.INTRODUCING
    name = str(os.getpid())
    my_info = PlayerInfo()
    players_info = {}           # Dict[str, PlayerInfo]
    world_x = 0
    world_y = 0
    command_list = List[Command]
    last_cmd = ''
    meet_list = []       # list of who I want to meet
    met_list = []        # list of who I has met

    # class MeetContext:
    #     is_initiator = False
    #     players = {}  # Dict[str, int]  # {player name, direction}
    #     initiator_direction = 0
    #     meet_confirm_heard = False
    # meet_context = MeetContext()

    def __init__(self, world_size):
        self.world_x = int(world_size[0])
        self.world_y = int(world_size[1])

    def play(self, result: str, messages: List[Message]):
        self._handle_messages(messages)
        print('state', self.state)
        if self.state == self.State.INTRODUCING:
            return self._introduce(result)
        if self.state == self.State.COLLECTING:
            return self._collect(result)
        if self.state == self.State.MEETING:
            return self._meet(result, messages)
        if self.state == self.State.INCANTATION:
            return self._incantate(result, messages)

    def _handle_messages(self, messages: List[Message]) -> Command:
        """
        List of messages:
        '<name> hi <data>' - one-time message on connection to server
        '<name> took <data>' - say if stone picked or lvlupd
        '<name> meet <target_name>' - say <target_name> that we should meet
        '<name> meet_confirm <target_name>' - reply to the previous message

        <name> - name of player (pid)
        <data> - PlayerInfo.__str__() result
        """
        for m in messages:
            if m.t == Message.Type.VOICE:
                mv = MessageVoice.fromStr(m.data)
                if mv.command == 'hi':
                    reply = MessageVoice(self.name, 'took',
                                         str(self.my_info)).toCommand()
                    self.command_list.insert(0, reply)
                if mv.command == 'hi' or mv.command == 'took':
                    self.players_info[mv.source] = PlayerInfo(mv.data)
                    messages.remove(m)
                if mv.command == 'meet' and mv.data == self.name:
                    if self.state == self.State.COLLECTING:
                        self.command_list.clear()
                        self.state = self.State.MEETING
            if m.t == Message.Type.ACTUAL_LEVEL:
                self.state = self.State.INCANTATION
            if m.t == Message.Type.ELEVATION:
                self.state = self.State.INCANTATION
                messages.remove(m)

    def _introduce(self, result: str) -> Command:
        if result == '':
            return Command(Command.Type.SAY,
                           self.name + ' hi ' + str(self.my_info))
        self.state = self.State.COLLECTING
        return self._collect('')

    def _collect(self, result: str) -> Command:
        # return Command(Command.Type.WAIT)
        if result == '' or not self.command_list:
            self.command_list = self._generate_collect_command_list()
            # say about lvlup
            if result == '' and self.my_info.lvl != 1:
                self.command_list.insert(
                    0,
                    Command(Command.Type.SAY,
                            self.name + ' took ' + str(self.my_info)))

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
                    print("(I want to take {} from {})".format(take_list, i))
                    self._take(i, take_list)
                    break

        if (self.last_cmd and self.last_cmd.t == Command.Type.TAKE_OBJECT
                and self.last_cmd.arg != 'nourriture'):
            if result == 'ok':
                self.my_info.stones_pack.add_stone(self.last_cmd.arg)
                self.command_list.insert(
                    0,
                    Command(Command.Type.SAY,
                            self.name + ' took ' + str(self.my_info)))
            self.meet_list = self._can_incantate()
            if self.meet_list:
                if self.meet_list[0] == self.name:
                    self.command_list = []
                    self.state = self.State.INCANTATION
                    return self._incantate('', [])
                self.state = self.State.MEETING
                self.command_list.clear()
                for name in self.meet_list:
                    self.command_list.append(
                        MessageVoice(self.name, 'meet', name).toCommand())
                return self._meet('')

        cmd = self.command_list.pop(0)
        self.last_cmd = cmd
        return cmd

    def _do_i_need_this(self, res: str) -> bool:
        # print('my_info: ' + str(self.my_info))
        if res == 'nourriture':
            return True
        if self.state == self.State.MEETING:
            return False
        stones_need = GameRules.stones_need_for_level(self.my_info.lvl)
        if res == 'linemate' and self.my_info.stones_pack.li < stones_need.li:
            return True
        if res == 'deraumere' and self.my_info.stones_pack.de < stones_need.de:
            return True
        if res == 'sibur' and self.my_info.stones_pack.si < stones_need.si:
            return True
        if res == 'mendiane' and self.my_info.stones_pack.me < stones_need.me:
            return True
        if res == 'phiras' and self.my_info.stones_pack.ph < stones_need.ph:
            return True
        if res == 'thystame' and self.my_info.stones_pack.th < stones_need.th:
            return True
        return False

    # stones_need: 1 1 1 0 0 0
    # my_stones:   2 2 0 0 2 2

    # TODO I waste too much time for walking. Need to upgrade _take to support multiple pos
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
        # go forward
        for i in range(0, y):
            commands.append(Command(Command.Type.GO))
        # go left or right if need
        if x != 0:
            commands.append(
                Command(Command.Type.TURN_LEFT if x > 0 else Command.Type.
                        TURN_RIGHT))
        for i in range(0, abs(x)):
            commands.append(Command(Command.Type.GO))

        # take items from current cell
        for c in contents:  # TODO don't take linamte on level 1
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

    def _check_met(self, see_result: str) -> bool:
        striped = see_result.strip('{}')
        splited_comma = striped.split(',')
        splited_space = splited_comma[0].split()
        players_count = 1       # I am first
        for i in splited_space:
            if i == 'player':
                players_count += 1
        if players_count == GameRules.players_need_for_level(self.my_info.lvl):
            if set(self.meet_list) == set(self.met_list):
                print('ready to incantate')
                return True
        return False

    def _meet(self, result: str, messages: List[Message] = []) -> Command:
        for m in messages:
            if m.t == Message.Type.VOICE:
                mv = MessageVoice.fromStr(m.data)
                if mv.command == 'meet' and mv.data == self.name:
                    self.command_list.clear()
                    reply = ''
                    if self._move_to_target(m.source):
                        self.met_list.append(mv.source)
                        reply = 'has_met'
                        self.state = self.State.INCANTATION
                    else:
                        reply = 'meet'
                    meet_cmd = MessageVoice(self.name, reply,
                                            mv.source).toCommand()
                    self.command_list.append(meet_cmd)
                    messages.remove(m)
                    if self.state == self.State.INCANTATION:
                        return self._incantate('', [])
                if mv.command == 'has_met' and mv.data == self.name:
                    self.met_list.append(mv.source)
                    messages.remove(m)

        if self.last_cmd.t == Command.Type.SEE:
            if self._check_met(result):
                self.state = self.State.INCANTATION
                return self._incantate('', [])

        if not self.command_list:
            self.command_list.append(Command(Command.Type.SEE))
        cmd = self.command_list.pop(0)
        self.last_cmd = cmd
        return cmd

    def _move_to_target(self, t: int) -> bool:
        if t == 0:
            return True
        if t == 1:
            self.command_list.append(Command(Command.Type.GO))
        elif t == 2:
            self.command_list.append(Command(Command.Type.GO))
            self.command_list.append(Command(Command.Type.TURN_LEFT))
            self.command_list.append(Command(Command.Type.GO))
        elif t == 3:
            self.command_list.append(Command(Command.Type.TURN_LEFT))
            self.command_list.append(Command(Command.Type.GO))
        elif t == 4:
            self.command_list.append(Command(Command.Type.TURN_LEFT))
            self.command_list.append(Command(Command.Type.GO))
            self.command_list.append(Command(Command.Type.TURN_LEFT))
            self.command_list.append(Command(Command.Type.GO))
        elif t == 5:
            self.command_list.append(Command(Command.Type.TURN_LEFT))
            self.command_list.append(Command(Command.Type.TURN_LEFT))
            self.command_list.append(Command(Command.Type.GO))
        elif t == 6:
            self.command_list.append(Command(Command.Type.TURN_RIGHT))
            self.command_list.append(Command(Command.Type.GO))
            self.command_list.append(Command(Command.Type.TURN_RIGHT))
            self.command_list.append(Command(Command.Type.GO))
        elif t == 7:
            self.command_list.append(Command(Command.Type.TURN_RIGHT))
            self.command_list.append(Command(Command.Type.GO))
        elif t == 8:
            self.command_list.append(Command(Command.Type.GO))
            self.command_list.append(Command(Command.Type.TURN_RIGHT))
            self.command_list.append(Command(Command.Type.GO))
        else:
            print('unknown target: ' + str(t))
        return False

    def _can_incantate(self) -> List[str]:
        '''
        return list of names to meet with, my name if lvl 1, empty if cannot
        '''
        print('my_info: ' + str(self.my_info) + ', ' + 'players: ' +
              str(self.players_info))

        if self.my_info.lvl == 1 and self.my_info.stones_pack.li >= 1:
            return [self.name]

        result = []
        stones_need = GameRules.stones_need_for_level(self.my_info.lvl)
        players_need = GameRules.players_need_for_level(self.my_info.lvl)
        stones_collected = self.my_info.stones_pack
        for name in self.players_info.keys():
            player = self.players_info[name]
            if player.lvl == self.my_info.lvl:
                stones_collected += player.stones_pack
                result.append(name)
                if (stones_need <= stones_collected and
                        len(result) + 1 == players_need):
                    return result
        return []

        # if self.my_info.lvl == 2:
        #     for name in self.players_info.keys():
        #         player = self.players_info[name]
        #         li = self.my_info.stones_pack.li
        #         de = self.my_info.stones_pack.de
        #         si = self.my_info.stones_pack.si
        #         if player.lvl == 2:
        #             li = li + player.stones_pack.li
        #             de = de + player.stones_pack.de
        #             si = si + player.stones_pack.si
        #             if li >= 1 and de >= 1 and si >= 1:
        #                 return [name]
        # return []

    def _drop_stones(self):
        stones_need = GameRules.stones_need_for_level(self.my_info.lvl)
        my_stones_copy = self.my_info.stones_pack
        while stones_need.li and my_stones_copy.li:
            self.command_list.append(
                Command(Command.Type.DROP_OBJECT, 'linemate'))
            my_stones_copy.li -= 1
        while stones_need.de and my_stones_copy.de:
            self.command_list.append(
                Command(Command.Type.DROP_OBJECT, 'deraumere'))
            my_stones_copy.de -= 1
        while stones_need.si and my_stones_copy.si:
            self.command_list.append(Command(Command.Type.DROP_OBJECT,
                                             'sibur'))
            my_stones_copy.si -= 1
        while stones_need.me and my_stones_copy.me:
            self.command_list.append(
                Command(Command.Type.DROP_OBJECT, 'mendiane'))
            my_stones_copy.me -= 1
        while stones_need.ph and my_stones_copy.ph:
            self.command_list.append(
                Command(Command.Type.DROP_OBJECT, 'phiras'))
            my_stones_copy.ph -= 1
        while stones_need.th and my_stones_copy.th:
            self.command_list.append(
                Command(Command.Type.DROP_OBJECT, 'thystame'))
            my_stones_copy.th -= 1
        # print('schedule dropping: ' + str(self.command_list))

    def _incantate(self, result: str, messages: List[Message]) -> Command:
        for m in messages:
            if m.t == Message.Type.ACTUAL_LEVEL:
                self.my_info.lvl = m.data
                print('new level: ' + str(self.my_info.lvl))
                self.state = self.State.COLLECTING
                messages.remove(m)
                return self._collect('')
        if result == '':
            self.command_list.append(Command(Command.Type.INVENTORY))
            self._drop_stones()
            self.command_list.append(Command(Command.Type.INCANTATE))
        if self.command_list:
            return self.command_list.pop(0)  # drop stones and incantate

        return Command(Command.Type.WAIT)

        # if result.startswith('niveau actuel'):
        #     splited = result.split(':')
        #     self.my_info.lvl = int(splited[1].strip())
        #     self._remove_stones()
        # self.state = self.state.COLLECTING
        # return self._collect('')
