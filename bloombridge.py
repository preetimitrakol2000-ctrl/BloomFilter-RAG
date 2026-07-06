import ctypes
import os
import sys

class BloomBridge:
    def __init__(self):
        if not os.path.exists("./libbloom.so") and not os.path.exists("./libbloom.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libbloom.dll bloom_filter.c")
                lib_path = "./libbloom.dll"
            else:
                os.system("gcc -shared -fPIC -o libbloom.so bloom_filter.c")
                lib_path = "./libbloom.so"
        else:
            lib_path = "./libbloom.dll" if sys.platform.startswith("win") else "./libbloom.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_bloom.restype = ctypes.c_void_p
        self.lib.register_token.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self.lib.check_token_presence.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        self.lib.check_token_presence.restype = ctypes.c_bool
        
        self.bloom_ptr = self.lib.init_bloom()

    def train_filter_keyword(self, word: str):
        self.lib.register_token(self.bloom_ptr, word.lower().encode('utf-8'))

    def evaluate_membership(self, word: str) -> bool:
        return self.lib.check_token_presence(self.bloom_ptr, word.lower().encode('utf-8'))
