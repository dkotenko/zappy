class StonesPack:
    li = 0
    de = 0
    si = 0
    me = 0
    ph = 0
    th = 0

    def __init__(self, li, de, si, me, ph, th):
        self.li = li
        self.de = de
        self.si = si
        self.me = me
        self.ph = ph
        self.th = th

    def __str__(self):
        r = str(self.li) + '_' + \
            str(self.de) + '_' + \
            str(self.si) + '_' + \
            str(self.me) + '_' + \
            str(self.ph) + '_' + \
            str(self.th)
        return r

    def __repr__(self):
        return self.__str__()

    def add_stone(self, s):
        if s == 'linemate':
            self.li += 1
        if s == 'deraumere':
            self.de += 1
        if s == 'sibur':
            self.si += 1
        if s == 'mendiane':
            self.me += 1
        if s == 'phiras':
            self.ph += 1
        if s == 'thystame':
            self.th += 1

    def __le__(self, other):
        if (self.li <= other.li and
                self.de <= other.de and
                self.si <= other.si and
                self.me <= other.me and
                self.ph <= other.ph and
                self.th <= other.th):
            return True
        return False


class GameRules:

    def stones_need_for_level(level):
        if level == 1:
            return StonesPack(1, 0, 0, 0, 0, 0)
        elif level == 2:
            return StonesPack(1, 1, 1, 0, 0, 0)
        elif level == 3:
            return StonesPack(2, 0, 1, 0, 2, 0)
        elif level == 4:
            return StonesPack(1, 1, 2, 0, 1, 0)
        elif level == 5:
            return StonesPack(1, 2, 1, 3, 0, 0)
        elif level == 6:
            return StonesPack(1, 2, 3, 0, 1, 0)
        elif level == 7:
            return StonesPack(2, 2, 2, 2, 2, 1)
        print('unknown level')

    def players_need_for_level(level):
        if level == 1:
            return 1
        if level == 2 or level == 3:
            return 2
        if level == 4 or level == 5:
            return 4
        if level == 6 or level == 7:
            return 6
        print('unknown level')

