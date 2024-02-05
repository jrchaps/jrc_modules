This implementation is defined to work well for cases where the user needs to keep track of the hash state.
For example, in TLS the author needs to keep track of the hash state while constructing transcript hashes
of the handshake messages.

Useful documentation:

https://en.wikipedia.org/wiki/SHA-2
https://csrc.nist.gov/files/pubs/fips/180-2/final/docs/fips180-2.pdf