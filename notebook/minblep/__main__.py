import argparse

from generate import generate_min_blep
from render import render_output


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('sample_rate',
            type=int,
            help="This is actualy labeled 'oversampling' in the "
                 "ExperimentalScene code, and I'm not actually sure what it "
                 "really represents.")
    parser.add_argument('zero_crossings',
            type=int,
            help='The number of zero crossings in the generated blep. '
                 'This is effectively the ring-out of the Gibbs ripple.')
    parser.add_argument('-f', '--output_format',
            default='list',
            help='The format of the minblep to be output')

    args = parser.parse_args()
    min_blep = generate_min_blep(args.zero_crossings, args.sample_rate)
    render_output(min_blep, args.output_format)


if __name__ == '__main__':
    main()
