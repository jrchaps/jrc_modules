This implementation doesn't provide a truncation option.
I'm considering not allowing key sizes larger than 32 bytes
as it's generally not needed and will simplify the implementation.

References:

https://datatracker.ietf.org/doc/html/rfc2104
https://datatracker.ietf.org/doc/html/rfc4231
