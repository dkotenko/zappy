import platform

def is_mac_os_x():
	return platform.system() == 'Darwin'
