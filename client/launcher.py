#!/usr/bin/env python3
from optparse import OptionParser


def parse_args():
    usage = "Usage: %prog -c <count> -n <team> -p <port> [-h <hostname>]"
    parser = OptionParser(add_help_option=False, usage=usage)
    parser.add_option('-c', dest='count', type='int')
    parser.add_option('-n', dest='team', type='str')
    parser.add_option('-p', dest='port', type='int')
    parser.add_option('-h', dest='hostname', type='str', default='127.0.0.1')
    (options, args) = parser.parse_args()
    if options.team is None \
       or options.port is None \
       or options.count is None:
        parser.print_usage()
        exit(1)
    return options


def run(port: int, team: str):
    pass


def main(options):
    for i in range(options.count):
        run(options.port, options.team)


if __name__ == '__main__':
    main(parse_args())
