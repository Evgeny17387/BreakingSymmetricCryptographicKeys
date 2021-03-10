
def rotate_left(word, n, word_size=64):

    mask = 2 ** word_size - 1

    return ((word << n) & mask) | ((word >> (word_size - n)) & mask)


def linear(word):

    return rotate_left(word, 15) ^ rotate_left(word, 32) ^ word


def apply_sbox(word):

    sbox = [0x2, 0x4, 0x5, 0x6, 0x1, 0xA, 0xF, 0x3, 0xB, 0xE, 0x0, 0x7, 0x9, 0x8, 0xC, 0xD]

    word_new = 0

    for i in range(16):

        nibble = (word >> (i * 4)) & 0xF

        word_new |= sbox[nibble] << i * 4

    return word_new


def round_function(word, key):

    word ^= key

    word = apply_sbox(word)

    word = linear(word)

    return word, linear(key) ^ 0x3


def encrypt(word, key, rounds=20):

    for r in range(rounds):
        word, key = round_function(word, key)

    return word


if __name__ == '__main__':

    plain_text = 512345
    key_main = 512345

    cipher = encrypt(plain_text, key_main)

    print(cipher)
