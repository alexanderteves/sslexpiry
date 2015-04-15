# sslexpiry

CLI tool to print expiration date ("not after") of PEM encoded certificates. Can show the default time string or Unix time. Useful for monitoring software calculating time diffs with Unix times.

## Usage

Make sure you have the OpenSSL development headers installed. Compile with `make` or `gcc -Wall main.c -o sslexpiry -lcrypto`.

`sslexpiry /path/to/certificate.pem` or `sslexpiry -u /path/to/certificate.pem`.
