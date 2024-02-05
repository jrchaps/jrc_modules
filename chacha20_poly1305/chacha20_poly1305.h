#include "../basic/basic.c"
#include "../slice/slice.c"
#include "../string/string.c"

typedef struct poly1305_ctx poly1305_ctx;

slice chacha20_poly1305_encrypt(slice, u8[32], u8[12], slice);
int chacha20_poly1305_decrypt(slice, u8[32], u8[12], slice, u8[16]);
void chacha20_poly1305_auth_tag(u8[16], slice, slice, u8[32], u8[12]);
void chacha20_cipher(slice, u8[32], u8[12], u32);
void chacha20_block(u8[64], u8[32], u8[12], u32);
void chacha_qr(u32*, u32*, u32*, u32*);

void poly1305(u8[16], slice, u8[32]);
poly1305_ctx poly1305_begin(u8[32]);
poly1305_ctx poly1305_run(poly1305_ctx, slice); 
void poly1305_end(poly1305_ctx, u8[16], u8[32]);
void poly1305_process(u32[17], u32[17], u32[17]);
void add_poly1305(u32 a[17], u32 b[17]);

struct poly1305_ctx {
    size_t block_length;
    u32 block[17], acc[17], r[17];
};