from math import ceil

import numpy as np
from matplotlib.pyplot import plot, xlabel, ylabel, ylim

from generate import generate_min_blep
from generate import gen_bl_saw


def plot_spectrum(signal, sample_rate):
    # http://samcarcagno.altervista.org/blog/basic-sound-processing-python/
    size = len(signal)
    fft_res = np.fft.fft(signal)
    fft_unique_points = int(ceil((size+1)/2.0))
    p = abs(fft_res[0:fft_unique_points])
    p = p / float(size)
    p = p ** 2
    if size % 2 == 1:
        p[1:len(p)] = p[1:len(p)] * 2
    else:
        p[1:len(p) - 1] = p[1:len(p) - 1] * 2
    freq_axis_array = np.arange(0,
            fft_unique_points, 1.0) * (sample_rate / size)
    plot(freq_axis_array/1000, 10 * np.log10(p))
    xlabel('Frequency (kHz)')
    ylabel('Power (dB)')
    ylim(ymin=-100)


def listen(zero_crossings, oversampling, frequency, duration_seconds):
    mb_gen = generate_min_blep(zero_crossings, oversampling)
    the_blep = [val * 2 - 1 for val in mb_gen]
    oversample_rate = oversampling * SAMPLE_RATE
    saw_gen = gen_bl_saw(the_blep,
                         frequency,
                         oversample_rate,
                         int(duration_seconds * oversample_rate),
                         np.pi)
    the_wave = np.array(list(saw_gen))
    return Audio(the_wave, rate=oversample_rate)

