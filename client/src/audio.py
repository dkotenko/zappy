import platform
import os

class Audio:
	
	MUSIC_MODE=False
	SOUND_MODE=False
	IS_MAC=False


	class Sound:
		_PATH = './sound/'
		MORT = _PATH + 'mort.wav'
		FORK = _PATH + 'fork.wav'
		GNG = _PATH + 'ghost_n_goblins.mp3'
		INCAN = _PATH + 'incantation.wav'
		MOVE = _PATH + 'move.wav'
		GET = _PATH + 'get.wav'
		PUT = _PATH + 'put.wav'
		ALL = [MORT, FORK, GNG, INCAN, MOVE, GET, PUT]
	

	def init(options):
		
		Audio.MUSIC_MODE = options.music
		Audio.SOUND_MODE = options.sound
		Audio.IS_MAC = platform.system().startswith('Darwin')
		Audio.play(Audio.Sound.GNG)

	def play(action):
		if not Audio.IS_MAC:
			return
		if action not in Audio.Sound.ALL:
			return
		if not Audio.MUSIC_MODE and action == Audio.Sound.GNG:
			return
		if not Audio.SOUND_MODE and action != Audio.Sound.GNG:
			return
		os.system('afplay ' + action + '&')
		
			

	