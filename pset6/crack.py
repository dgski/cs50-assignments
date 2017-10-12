import sys, os, crypt

#Ensure proper usage
if len(sys.argv) != 2:
    sys.exit("Usage: crack.py hash")

#Get salt
salt = sys.argv[1][:2]
hash_val = sys.argv[1];

#Password variable
password = []

char_range = [i for i in range(ord('A'), ord('Z'))]
char_range += [i for i in range(ord('a'), ord('z'))]


for i in char_range:
    for x in char_range:
        for y in char_range:
            for z in char_range:

                password = chr(i) + chr(x) + chr(y) + chr(z)

                if hash_val == crypt.crypt(password, salt):
                    print(password)
                    sys.exit()


