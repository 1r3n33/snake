import argparse
from pathlib import Path

# Map char to tile id.
# Lower case conversion in handled in program loop.
# Char not found in mapping will default to 64.
convert = {
    '0':72,
    '1':73,
    '2':74,
    '3':75,
    '4':76,
    '5':77,
    '6':78,
    '7':79,
    '8':80,
    '9':81,
    ' ':82,
    'a':83,
    'b':84,
    'c':85,
    'd':86,
    'e':87,
    'f':88,
    'g':89,
    'h':90,
    'i':91,
    'j':92,
    'k':93,
    'l':94,
    'm':95,
    'n':96,
    'o':97,
    'p':98,
    'q':99,
    'r':100,
    's':101,
    't':102,
    'u':103,
    'v':104,
    'w':105,
    'x':106,
    'y':107,
    'z':108,
    '!':109,
    '.':110,
}

# Parse args
parser = argparse.ArgumentParser(prog='gentxt')
parser.add_argument('file', type=argparse.FileType('r'))
args = parser.parse_args()

# Open file
with args.file as f:
    print('const uint8_t text_' + Path(args.file.name).stem + '[] = {')

    for line in f:
        lf = line[-1] == '\n'

        print('    // ' + line, end='' if lf else '\n')
        print('    ', end='')

        # Remove LF
        for c in line[:-1] if lf else line:
            # Print 1 char (Only support lowercase).
            # Print 64 for unmapped char.
            c = c.lower()
            if c in convert:
                print(convert[c], end='')
            else:
                print(64, end='')
            print(', ', end='')

        # End line (with 0)
        print('0,')

    # End file (with 0)
    print('    // EOF')
    print('    0')
    print('};')
