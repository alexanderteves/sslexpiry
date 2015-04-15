// Copyright (c) 2015 Alexander Teves <alexander.teves@gmail.com>
// Released under the terms of the MIT license

#define _XOPEN_SOURCE
#define BUFLEN 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <time.h>

int convertAsn1ToString(ASN1_TIME *notAfter, char buffer[]) {
	BIO *bio = BIO_new(BIO_s_mem());
	ASN1_TIME_print(bio, notAfter);
	BIO_gets(bio, buffer, BUFLEN);
	BIO_free(bio);
	return 0;
}

int convertAsn1ToTimestamp(ASN1_TIME *notAfter, char buffer[]) {
	convertAsn1ToString(notAfter, buffer);
	struct tm timestruct;
	memset(&timestruct, 0, sizeof(struct tm));
	strptime(buffer, "%b %d %H:%M:%S %Y", &timestruct);
	strftime(buffer, BUFLEN, "%s", &timestruct);
	return 0;
}

int main(int argc, char* args[]) {
	if(argc == 1) {
		printf("Usage: %s [OPTIONS] [FILE]\n\n", args[0]);
		printf("Options:\n\t-u\t Print UNIX timestamp\n\n");
		return 1;
	}

	FILE *pemFile = fopen(args[argc-1], "r");
	if(!pemFile) {
		fprintf(stderr, "Could not open file \"%s\"\n", args[argc-1]);
		return 1;
	}

	X509 *cert = PEM_read_X509(pemFile, NULL, NULL, NULL);
	if(!cert) {
		fprintf(stderr, "Could not read PEM format\n");
		return 1;
	}

	char notAfterStr[BUFLEN];

	ASN1_TIME *notAfter = X509_get_notAfter(cert);

	if(strcmp(args[1], "-u") == 0) {
		convertAsn1ToTimestamp(notAfter, notAfterStr);
	} else {
		convertAsn1ToString(notAfter, notAfterStr);
	}

	printf("%s\n", notAfterStr);

	return 0;
}
