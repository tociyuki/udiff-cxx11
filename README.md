udiff - Word based Unified diff Command Between Two Text Files
======================================================

udiff computes differences between two text files,
and reports results with word based color unified diff format.
The words in the line are splitted by Unicode-7.0 UCD Script
property.
It is written with C++11.

The engine is an implementation of the linear space variation
for Wu's O(NP) algorithm\[1\] based on the similar way of
Myers's bi-direction method\[2\]. The linear space variation
is introduced by MIZUTANI Tociyuki from scratch.

\[1\] S. Wu, U. Manber, G. Myers, W. Miller,
<q>An O(NP) Sequence Comparion Algorithm</q>,
Inf. Process. Lett. Volume 35 Issue6 (1990)

\[2\] E. W. Myers, <q>An O(ND) Difference Algorithm and Its Variations</q>,
Algorithmica 1 (1986)

VERSION
------

0.02

INSTALL
------

Current version works only POSIX environment and on the ANSI color
escape sequence terminals. udiff uses isatty (3) function.

    $ make
    $ ./udiff text-original text-new

OPTIONS
------

udiff is not compatible POSIX diff (3) command.

    --color     report with color word-based unified format.
    --nocolor   report normal unified format.
    -u, -U NUM, --unified\[=NUM\]
                set unified context lines (default 3).
    --help      show help.
    -v, --version   show version.

LICENSE
------

License: The BSD 3-Clause

Copyright (c) 2015, MIZUTANI Tociyuki
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

