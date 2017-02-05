from itertools import izip

import numpy as np


TAU = 2.0 * np.pi

def gen_naive_saw(osc_freq, sample_rate, num_samples, initial_phase=0):
    peak_amplitude = 1.0
    two_pi = 2.0 * np.pi
    phase = initial_phase
    for i in range(0, num_samples):
        out_amp = peak_amplitude - (peak_amplitude / np.pi * phase)
        phase = phase + ((2 * np.pi * osc_freq) / sample_rate)
        if phase >= two_pi:
            phase -= two_pi
        yield out_amp


def gen_additive_saw(osc_freq, sample_rate, num_samples):
    # thanks @wrl
    nyquist = sample_rate / 2.0
    harmonics_range = range(1, int(math.ceil(nyquist / osc_freq)))
    harmonics_freqs = (osc_freq * x for x in sine_range)

    end_phases = [((TAU * f) / sample_rate) * num_samples
                  for f in harmonics_freqs]
    sines = [np.sin(np.linspace(0, end_phase, num_samples)) / (i + 1.0)
             for (i, end_phase) in enumerate(end_phases)]
    return np.sum(sines, axis=0) / 2.0


def gen_bl_saw(blep_buffer, osc_freq, sample_rate, num_samples, initial_phase=0):
    blep_size = len(blep_buffer)
    blep_pointers = []
    
    peak_amplitude = 1.0
    two_pi = 2.0 * np.pi
    phase = initial_phase
    for i in range(0, num_samples):
        out_amp = peak_amplitude - (peak_amplitude / np.pi * phase)
        
        for ptr in blep_pointers:
            out_amp *= blep_buffer[ptr]
        blep_pointers = [(ptr + 1) for ptr in blep_pointers if ptr + 1 < blep_size]
        
        phase = phase + ((2 * np.pi * osc_freq) / sample_rate)
        if phase >= two_pi:
            phase -= two_pi
            blep_pointers.append(0)

        yield out_amp


def generate_min_blep(zero_crossings, sample_rate):
    size = (2 * zero_crossings * sample_rate) + 1
    signal = list(windowed_sinc(blackman_window, size, zero_crossings))
    cep = cepstrum(signal, size)
    min_phase = minimum_phase(cep, size)
    min_blep = normalize(integrate(min_phase))
    return min_blep


def blackman_window(length):
    for i in range(0, length):
        f = 2.0 * np.pi * i / length
        val = (0.42 - (0.5 * np.cos(f)) + (0.08 * np.cos(2.0 * f)))
        yield val


def sinc_buffer(size, zero_crossings=10):
    for i in range(0, size):
        r = float(i) / (float(size) - 1.0)
        val = np.sinc(-zero_crossings + (r * 2.0 * zero_crossings))
        yield val


def windowed_sinc(win_func, size, zero_crossings=10):
    sinc_gen = sinc_buffer(size, zero_crossings)
    for sinc, window in izip(sinc_gen, win_func(size)):
        windowed = sinc * window
        yield windowed


def cepstrum(signal, size):
    fft_res = np.fft.fft(signal, size)
    log_abs = np.log(abs(fft_res))
    cep = np.fft.ifft(log_abs)
    return cep


def minimum_phase(signal, size):
    half_size = size / 2
    real_time_domain = []

    for i in range(0, half_size):
        real_time_domain.append(2.0 * signal[i])

    zero_start = half_size
    if size % 2 == 1:
        real_time_domain.append(2.0 * signal[half_size])
        zero_start += 1

    for i in range(zero_start, size):
        real_time_domain.append(0.0)

    fft_res = np.fft.fft(real_time_domain, size)
    exp_freq = np.exp(fft_res)
    time_res = np.fft.ifft(exp_freq)
    return time_res.real


def integrate(signal):
    running_val = 0.0
    for val in signal:
        running_val += val
        yield running_val


def normalize(signal):
    sig = list(signal)
    scale = 1.0 / sig[-1]
    normalized = [x * scale for x in sig]
    return normalized


def polyblep_2sample(phase, phase_step):
    if phase < phase_step:
        t = phase / phase_step
        return (t*2) - (t**2) - 1.0
    
    if phase > (1.0 - phase_step):
        t = (phase - 1.0) / phase_step
        return (t**2) + (t*2) + 1.0
    
    return 0.0


def gen_polyblep_saw(osc_freq, sample_rate, nsamples):
    end_phase = (osc_freq / float(sample_rate)) * nsamples
    
    phases = np.mod(np.linspace(0, end_phase, nsamples), 1.0)
    
    step = phases[1] - phases[0]
    polyblep = [polyblep_2sample(x, step) for x in phases]
    
    return (1.0 - (2.0 * phases)) + polyblep
