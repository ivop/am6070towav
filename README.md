# AM6070TOWAV

With this program you can convert ROM dumps from old drum machines that used the _am6070_ Companding D-to-A Converter.
It can be used both as a GUI application, and as a command line utility for batch conversion.

## GUI

## Command Line

When used as a command line utility, you can specify the following command line options, which all mimic the GUI equivalents:

```
usage: am6070towav [options] input.rom [input.rom]*

   Run without arguments to start the GUI version.

   When using the command line version, you can specify up to eight ROMs.

   -h              print this help message
   -o filename     output filename [default: output.wav]
   -r rate         output sample rate [default: 20833]
   -d time         decay to zero in [0-500] ms [default: off]
   -t type         type of conversion [default: all banks]
                       0 - Convert Bank 0-7
                       1 - Convert Bank 0 Only
                       2 - Convert Bank 0, first 50%
                       3 - Convert Bank 0, second 50%
                       4 - Convert Bank 0, first 25%
                       5 - Convert Bank 0, second 25%
                       6 - Convert Bank 0, third 25%
                       7 - Convert Bank 0, fourth 25%
                       8 - Convert Bank 0, first 75%
                       9 - Convert Bank 0, last 75%
```

This can be useful if you want to do batch conversion of several ROMs, or want a bunch of different tunings.
For example:

```
for i in `seq 10000 1000 30000` ; do
    am6070towav -r $i -o clap-$i.wav clp1.bin clp2.bin
done
```
