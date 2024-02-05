    This implementation of the advanced encryption standard (AES) in galois/counter mode (GCM) is specifically
designed for use in TLS 1.3. Although this function could support larger streams, I limited the plaintext length
to u32_max, essentially preventing counter overuse at compile time. There is both a software implementation and a
version using x86-64 intrinsics. For maximum speed and security, the intrinsics version should be used, as the
software version is not only much slower, but is also vulnerable to timing-attacks. This vulnerability could be
fixed with a constant-time implementation at the cost of probably worse performance. https://github.com/bitcoin-core/ctaes

References:

https://www.rfc-editor.org/rfc/rfc5116#section-5.1
https://csrc.nist.rip/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-spec.pdf
https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf
https://www.intel.com/content/dam/develop/external/us/en/documents/10tb24-breakthrough-aes-performance-with-intel-aes-new-instructions-final-secure-165940.pdf
https://www.intel.com/content/dam/develop/external/us/en/documents/clmul-wp-rev-2-02-2014-04-20.pdf
https://www.iacr.org/archive/ches2009/57470001/57470001.pdf
http://www.herongyang.com/Cryptography/AES-Rijndael-Encryption-Algorithm.html