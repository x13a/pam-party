#include <stdio.h>

#include <openssl/evp.h>

int
calc_hash(const char *digestname, const void *data, size_t cnt, char *out)
{
	const EVP_MD *md = EVP_get_digestbyname(digestname);
	if (!md) {
		return 0;
	}
	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	if (!ctx) {
		return 0;
	}
	if (!EVP_DigestInit_ex(ctx, md, NULL)) {
		return 0;
	}
	if (!EVP_DigestUpdate(ctx, data, cnt)) {
		return 0;
	}

	unsigned char value[EVP_MAX_MD_SIZE];
	unsigned int length;

	if (!EVP_DigestFinal_ex(ctx, value, &length)) {
		return 0;
	}
	EVP_MD_CTX_free(ctx);

	for (unsigned int i = 0; i < length; i++) {
		if (sprintf(&out[i << 1], "%02x", value[i]) < 0) {
			return 0;
		}
	}
	return 1;
}
