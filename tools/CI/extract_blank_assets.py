from PIL import Image

import json

import os
import struct
import random
import av
import numpy as np


def create_blank_aiff(filename, duration_seconds=0.1, sample_rate=44100, channels=1):
    """
    Creates a blank AIFF file of a specified duration.
    """
    # Parameters for the audio stream
    codec_name = 'pcm_s16be' # AIFF typically uses big-endian signed 16-bit PCM
    sample_format = 's16'    # Sample format for numpy array
    layout = 'stereo' if channels == 2 else 'mono'

    # Open output container
    output = av.open(filename, mode='w')
    
    # Add an audio stream
    stream = output.add_stream(codec_name, rate=sample_rate)
    stream.layout = layout
    stream.format = sample_format
    
    # Calculate the total number of samples needed
    total_samples = int(duration_seconds * sample_rate)
    
    # Define frame size (e.g., 1024 samples per frame for processing in chunks)
    frame_size = 1024
    
    # Generate and write silent frames
    for i in range(0, total_samples, frame_size):
        samples_to_write = min(frame_size, total_samples - i)
        # Create a numpy array of zeros (silence) with int16 dtype
        silence = np.zeros((channels, samples_to_write), dtype='int16')
        
        # Create an AudioFrame from the numpy array
        frame = av.AudioFrame.from_ndarray(silence, format=sample_format, layout=layout)
        frame.sample_rate = sample_rate
        
        # Set the presentation timestamp (PTS) for the frame
        # PTS is needed for proper timing
        frame.pts = i

        # Encode and mux the packet
        for packet in stream.encode(frame):
            output.mux(packet)

    # Flush the encoder
    for packet in stream.encode(None):
        output.mux(packet)

    # Close the file to write the header and finalize the file
    output.close()

    print(f"Created blank AIFF file: {filename} ({duration_seconds} seconds)")


with open("assets.json") as assets:
    j = json.loads(assets.read())

    all_assets = []
    any_missing_assets = False
    for asset, data in j.items():
        if asset.startswith("@"):
            continue
        all_assets.append((asset, data, False))

    # Create work list
    todo = []
    for (asset, data, exists) in all_assets:
        # Leave existing assets alone if they have a compatible version.
        if exists and not (local_version == new_version or asset_needs_update(asset, local_version)):
            continue

        meta = data[:-2]
        size, positions = data[-2:]
        for lang, pos in positions.items():
            mio0 = None if len(pos) == 1 else pos[0]
            pos = pos[-1]
            todo.append((asset, pos, size, meta))

    for (asset, pos, size, meta) in todo:
        if ".png" in asset:
            if len(meta) == 0:
                if "_eu" in asset:
                    meta = [320, 224]
                else:
                    meta = [316, 228]
            im = Image.new("RGB", meta, 0xFF)

            os.makedirs(os.path.dirname(asset), exist_ok = True)
            im.save(asset)
        elif ".aiff" in asset:
            os.makedirs(os.path.dirname(asset), exist_ok = True)
            create_blank_aiff(asset)





