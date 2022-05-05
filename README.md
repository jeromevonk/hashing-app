# hashing-app

#ashing is a freeware Windows application for performing cryptograhic hashes. Quickly perform MD5, SHA-1, SHA-2.

It relies on [OpenSSL](https://www.openssl.org) for calculation hashes.

Links:

* Article about Cryptographic Hashes and explanation of this app on [Code Project](https://www.codeproject.com/Articles/1044042/Cryptographic-Hashes-What-They-Are-and-Why-You-Sho)
* App [home-page](https://hashingapp.github.io/)

## Source code and building

App was written in [C++/CLI](https://en.wikipedia.org/wiki/C%2B%2B/CLI) and built with Visual Studio 2015.

Folders:

* *inc* contains headers from openssl
* *lib* contains compiled openssl code
* *source* contains the source code from the application itself (UI + logic)
* *released* contains compiled versions of the app

Developed by [Jerome Vonk](https://jeromevonk.github.io/).
