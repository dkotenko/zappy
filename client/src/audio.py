import platform

class Audio:
	MUSIC=False
	SOUND=False
	IS_MAC=False

	def init(options):
		MUSIC = options.MUSIC
		SOUND = options.SOUND
		IS_MAC = platform.system().startswith('Darwin')

	def play(action):
		pass