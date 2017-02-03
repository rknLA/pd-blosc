# Python helpers / R&D for blosc

To better visualize and explore how MinBLEPs work and should be generated, I've created an iPython Notebook port of [the MinBLEP generator from experimentalscene](http://www.experimentalscene.com/articles/minbleps.php)

The resulting code from those experiments is put into a module, `minblep`, which can then be used to generate a minblep look-up table in C for the pd external.

While the current version is not yet outputting valid C code, it *is* outputting the values that will go into the look-up table.

You can run it from the command line:

```bash
# initial setup
cd $THIS_GIT_REPO/notebook
virtualenv venv && source venv/bin/activate
pip install -r requirements.txt

# actually run the script
python minblep 4 50
```

The `minblep` command takes the oversample factor, and the number of zero crossings as arguments, and the results are output to `stdout`.

Very soon, there will be a `-f` option (or similar) to specify an output format, or output template.
