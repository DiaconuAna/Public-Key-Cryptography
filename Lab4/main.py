from functools import reduce
import random
from termcolor import colored
from sty import fg, rs

# ---------- Alphabet declaration ----------
#   A list where each letter corresponds to the index where it's placed in the list
alphabet = list(' ABCDEFGHIJKLMNOPQRSTUVWXYZ')


# ---------- Util functions ----------

def isPrime(x):
    """
    Checks if a number is prime
    :param x: int number
    :return: true | false
    """
    if x % 2 == 0:
        return False
    i = 3
    while i * i <= x:
        if x % i == 0:
            return False
        i += 2
    return True


def getPrime(interval):
    """
    Generates a prime number in the specified interval
    :return: prime number
    """
    lower_bound = interval[0]
    upper_bound = interval[1]
    starting_point = random.randint(lower_bound, upper_bound)
    potential_prime = starting_point * 2 + 1
    while not isPrime(potential_prime):
        potential_prime -= 2
    return potential_prime


def divideMessage(message, m):
    """
    Divides the message into blocks of size m. \n If last block is not a multiple of m, it pads the message with ' '. \n
    :param message: message to be divided
    :param m: size of a block
    :return: list of words divided
    """
    words = [message[i:i + m] for i in range(0, len(message), m)]  # Split the message into blocks of size m
    if len(words[-1]) != m:
        for i in range(0, m - len(words[-1])):
            words[-1] += ' '
    return words


# Find square root in Zp where p = 3 mod 4
# x = a^((p+1)/4) (mod p)
def sqrt_p_3_mod_4(a, p):
    r = pow(a, (p + 1) // 4, p)
    return r


# Find square root in Zp where p = 5 mod 8
def sqrt_p_5_mod_8(a, p):
    """
    Let d = a^(p-1)/4 (mod p)

    if d = 1 => x = a^((p+3)/8) (mod p)
    else     => x = 2*a*(4*a)^((p-5)/8) (mod p)
    """
    d = pow(a, (p - 1) // 4, p)
    r = 0
    if d == 1:
        r = pow(a, (p + 3) // 8, p)
    elif d == p - 1:
        r = 2 * a * pow(4 * a, (p - 5) // 8, p) % p

    return r


# Extended Euclid
def extended_euclid(a: int, b: int) -> tuple[int, int]:
    """
    Extended Euclidean algorithm finds integer coefficients x and y such that: ax + by = gcd(a, b)
    >>> extended_euclid(10, 6)
    (-1, 2)
    >>> extended_euclid(7, 5)
    (-2, 3)
    """
    if b == 0:
        return 1, 0
    (x, y) = extended_euclid(b, a % b)
    k = a // b
    return y, x - k * y


# This function find the inverses of a i.e., a^(-1)
def invert_modulo(a: int, n: int) -> int:
    """
    >>> invert_modulo(2, 5)
    3
    >>> invert_modulo(8,7)
    1
    """
    (b, x) = extended_euclid(a, n)
    if b < 0:
        b = (b % n + n) % n
    return b


# ---------- Key generation ----------

#  Generate two distinct prime numbers such that p = 3 (mod 4) or p = 5 (mod 8)

p = getPrime((10, 80))

while p % 8 == 1:
    p = getPrime((10, 80))

q = getPrime((10, 80))

while q % 8 == 1:
    q = getPrime((10, 80))

#  Compute n = p * q
#   Public key: n; Private key: (p,q)
n = p * q


# ---------- Encryption ---------

def encrypt(plaintext):
    """
    Encrypts the given plaintext
    :param plaintext: message to be encrypted
    :return: ciphertext
    """
    # Divide the message into blocks of k=2 length
    blocks = divideMessage(plaintext, 2)
    ciphertext = ''

    # For each block
    for block in blocks:
        # Compute the numerical value
        numerical_value = 27 * alphabet.index(block[0]) + alphabet.index(block[1])  # m
        print(f"Block [{block}]")
        print(f"Numerical value [{numerical_value}]".rjust(25))

        # Encrypt the block: m^2 mod n
        encrypted_numerical_value = numerical_value ** 2 % n  # c
        print(f"Encrypted numerical value [{encrypted_numerical_value}]".rjust(36))

        # Get the readable version of the block
        encrypted_block = alphabet[int(encrypted_numerical_value / (27 ** 2))] + \
                          alphabet[int((encrypted_numerical_value % (27 ** 2)) / 27)] + \
                          alphabet[int((encrypted_numerical_value % (27 ** 2)) % 27)]
        print(f"Encrypted block [{encrypted_block}]".rjust(25))
        ciphertext += encrypted_block

    return ciphertext


# ---------- Encryption ---------

def decryption(ciphertext):
    """
    c - ciphertext
    p, q - private key
    """
    # Divide the message into blocks of l=3 length
    blocks = divideMessage(ciphertext, 3)
    plaintext = ''

    for block in blocks:
        # Compute the numerical equivalent of the encrypted block
        numerical_value = (27 ** 2) * alphabet.index(block[0]) + 27 * alphabet.index(block[1]) + alphabet.index(
            block[2])
        print(f"Block [{block}]")
        print(f"\tNumerical value [{numerical_value}]")

        # Solve x^2 = a (mod p) and x^2 = a (mod q) where a is the numerical value of the block
        r, s = 0, 0
        if p % 4 == 3:
            r = sqrt_p_3_mod_4(numerical_value, p)
        elif p % 8 == 5:
            r = sqrt_p_5_mod_8(numerical_value, p)
        if q % 4 == 3:
            s = sqrt_p_3_mod_4(numerical_value, q)
        elif q % 8 == 5:
            s = sqrt_p_5_mod_8(numerical_value, q)
        print(f"    r ={r}, s={s}")

        # Solve x^2 = a (mod n) where a is the numerical value of the block
        # Use the Chinese Remainder Theorem to get a solution modulo n
        n1 = p
        n2 = q
        N = n1 * n2  # N = n1 * n2
        N1 = N / n1  # N1 = N/n1
        N2 = N / n2  # N2 = N/n2
        K1 = invert_modulo(N1, n1)  # K1 = invert_modulo(N1, n1)
        K2 = invert_modulo(N2, n2)  # K2 = invert_modulo(N2, n2)

        print(f"\tn1 = {n1}")
        print(f"\tn2 = {n2}")
        print(f"\tN1 = {N1}")
        print(f"\tN2 = {N2}")
        print(f"\tK1 = {K1}")
        print(f"\tK2 = {K2}")
        print(fg.da_cyan + "\n***********************\n" + fg.rs)

        option_list = [] # we add all the valid decrypted blocks to the list

        # x = a1N1K1 + a2N2K2 (mod N)

        # Case 1: a1 > 0, a2 > 0
        a1 = r
        a2 = s
        print(f"\tCase 1: a1 = {a1}, a2= {a2}".rjust(25))
        x = (a1 * N1 * K1 + a2 * N2 * K2) % N
        if x >= (27 ** 2):
            print(fg.red + f"\tx = {x} is not valid".rjust(25) + fg.rs)
        else:
            literal_equivalent = alphabet[int(x / 27)] + alphabet[int(x % 27)]  # write the literal equivalents
            print(fg.li_green + f"\tLiteral equivalent : {literal_equivalent}".rjust(24) + fg.rs)
            option_list.append(literal_equivalent)

        print("\n***********************\n")

        # Case 2: a1 > 0, a2 < 0
        a1 = r
        a2 = s * (-1)
        print(f"\tCase 2: a1 ={a1}, a2={a2}")
        x = (a1 * N1 * K1 + a2 * N2 * K2) % N
        if x > (27 ** 2):
            print(fg.red + f"\tx = {x} is not valid" + fg.rs)
        else:
            literal_equivalent = alphabet[int(x / 27)] + alphabet[int(x % 27)]
            print(fg.li_green + f"\tLiteral equivalent : {literal_equivalent}" + fg.rs)
            option_list.append(literal_equivalent)

        print("\n***********************\n")

        # Case 3: a1 < 0, a2 > 0
        a1 = r * (-1)
        a2 = s
        print(f"\tCase 3: a1 ={a1}, a2={a2}")
        x = (a1 * N1 * K1 + a2 * N2 * K2) % N
        if x > (27 ** 2):
            print(fg.red + f"\tx = {x} is not valid" + fg.rs)
        else:
            literal_equivalent = alphabet[int(x / 27)] + alphabet[int(x % 27)]
            print(fg.li_green + f"\tLiteral equivalent : {literal_equivalent}" + fg.rs)
            option_list.append(literal_equivalent)

        print("\n***********************\n")

        # Case 4: a1 < 0, a2 < 0
        a1 = r * (-1)
        a2 = s * (-1)
        print(f",\tCase 4: a1 ={a1}, a2={a2}")
        x = (a1 * N1 * K1 + a2 * N2 * K2) % N
        if x > (27 ** 2):
            print(fg.red + f"\tx = {x} is not valid" + fg.rs)
        else:
            literal_equivalent = alphabet[int(x / 27)] + alphabet[int(x % 27)]
            print(fg.li_green + f"\tLiteral equivalent : {literal_equivalent}" + fg.rs)
            option_list.append(literal_equivalent)

        print("\n***********************\n")

        groupPicked = False

        while not groupPicked:
            print("Current plaintext: " + plaintext)
            print("Pick a group from the option list: ")

            index = 0
            for option in option_list:
                print(str(index) + '-> ' + str(option))
                index += 1

            choice = int(input())
            if choice < 0 or choice >= len(option_list):
                print("Try again")
            else:
                plaintext += option_list[choice]
                groupPicked = True

    return plaintext


def validation(plaintext):
    for character in plaintext:
        if character not in alphabet:
            return False

    return True

if __name__ == '__main__':
    print(f"p= {p}; q= {q}; n= {n}")
    # plaintext = "MADRID"
    print("Input plaintext >>>>")
    plaintext = input()
    plaintext = plaintext.upper()
    if(validation(plaintext)):
        print(f"Plaintext: {plaintext}")
        # ciphertext = encrypt("MADRID")
        ciphertext = encrypt(plaintext)
        print(f"\nCiphertext: {ciphertext}\n")
        plaintext2 = decryption(ciphertext)
        print(f"Plaintext: {plaintext2}")
    else:
        print("Invalid text")
