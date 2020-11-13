#!/usr/bin/python3
import os
import sys
import argparse


FREQ_H = """#ifndef freq_h
#define freq_h

const uint8_t val_a_list[40] = {{
{a}
}};

const uint16_t val_n_list[40] = {{
{n}
}}; //these are not real values but fine-tuned tweaked ones

#endif //freq_h
"""
FREQ_H_PATH = os.path.join(os.curdir, 'inc', 'freq.h')
VTX_FREQ = (
    5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725,  # A
    5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866,  # B
    5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945,  # E
    5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880,  # Air
    5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917   # Race
)

parser = argparse.ArgumentParser()
parser.add_argument("offset",type=int, default=0, help="offset in MHz")

def generate_pretty_list(nums):
    string = ""
    for n, i in enumerate(nums):
        if n % 8 == 0:
            string += "\t"
        string += str(i)
        if n != len(nums) - 1:
            string += ","
        string += "\n" if (n + 1) % 8 == 0 else " "
    return string

if __name__ == "__main__":
    args = parser.parse_args()
    
    a_list = list()
    n_list = list()

    for n, freq in enumerate(VTX_FREQ):
        freq += args.offset
        n = int(freq // (2 * 64 * 0.02))
        a = int(freq / (0.02 * 2) - n * 64)
        a_list.append(a)
        n_list.append(n)

    a_string = generate_pretty_list(a_list)
    n_string = generate_pretty_list(n_list)

    with open(FREQ_H_PATH, "w") as freqh:
        freqh.write(FREQ_H.format(a=a_string, n=n_string))
    print("File saved to {}".format(FREQ_H_PATH))

