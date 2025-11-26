from PIL import Image

import json

import aifc
import struct
import random

# Audio parameters
sampleRate = 44100.0  # hertz
duration = 0.1        # seconds
frequency = 440.0     # hertz (not strictly used here, but for context)
nchannels = 1         # mono
sampwidth = 2         # 2 bytes (16-bit)
nframes = int(sampleRate * duration)

with open("assets.json") as assets:
    j = json.loads(assets.read())
    for (asset, pos, size, meta) in j:
        if ".png" in asset:
            im = Image.new("RGB", size, 0xFF)
            im.save(asset)
        elif ".aiff" in asset:
            # Open the AIFF file in write mode ('w')
            try:
                with aifc.open('sound.aiff', 'w') as obj:
                    # Set all parameters at once as a tuple
                    # (nchannels, sampwidth, framerate, nframes, comptype, compname)
                    # 'NONE' is the compression type for uncompressed AIFF
                    obj.setparams((nchannels, sampwidth, sampleRate, nframes, 'NONE', 'not compressed'))

                    # Generate and write random sample data
                    for i in range(nframes):
                        # Generate a random 16-bit signed integer sample
                        value = random.randint(-32767, 32767)
                        # Pack the integer into a 2-byte binary string (AIFF uses big-endian byte order)
                        data = struct.pack('>h', value)
                        obj.writeframesraw(data)
            except aifc.Error as e:
                print(f"An error occurred: {e}")




