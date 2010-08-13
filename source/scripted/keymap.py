import string

KEYMAP = {}

def make_keymap():
    """Generate the keymap. It is not exhaustive, but should be good
    enough."""
    kmap = {}
    for i in range(10):
        kmap[i+48] = i # Decimals 0-9

	for i in range(26):
		kmap[i+65] = string.ascii_uppercase[i]	

	for i in range(26):
		kmap[i+97] = string.ascii_lowercase[i]	

    return kmap

def get_key(keycode):
    if keycode not in KEYMAP:
        return False
    return KEYMAP[keycode]

if not KEYMAP:
    KEYMAP = make_keymap()

