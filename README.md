rorqual (finner)
================

TL;DR: Like Apache OpenNLP (TokenNameFinder) but faster and actually working.

Detects all kinds of entities in financial news texts, like ISINs,
FIGIs (BBGIDs), monetary values, date and time stamps, etc.

Unlike the NERs from the big NLP packages this is rule based and hence
the false positive rate is low while the false negative rate is zero.


Red tape
--------

+ Licence: [BSD-3cl](http://directory.fsf.org/wiki/License:BSD_3Clause)
+ Project: <https://www.fresse.org/finner/>
+ Downloads: <https://github.com/hroptatyr/finner/releases>
+ Issues: <https://github.com/hroptatyr/finner/issues>


Technicalities
--------------

The command-line tool `finner` comes with a built-in tokeniser.  This is
mostly due to the `finner anno` subcommand which would otherwise be hard
to implement.  However, if interactive annotation is not one of your use
cases anyway and the built-in tokeniser has proved to be problematic for
your input, it's highly advised to pre-tokenise the input and pass it to
finner.

A somewhat more comprehensive tokeniser, based on Unicode's character
classes (and therefore with full UTF8 support) is `terms(1)` from the
[glod project](http://www.fresse.org/glod/).
