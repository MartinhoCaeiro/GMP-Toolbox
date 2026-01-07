#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

// Function to generate a random mpz_t number with given bits using /dev/urandom
void random_mpz(mpz_t rop, int bits) {
    int bytes = (bits + 7) / 8;
    unsigned char *buf = malloc(bytes);
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) { perror("Erro /dev/urandom"); exit(1);}
    if (read(fd, buf, bytes) != bytes) { perror("Erro leitura"); close(fd); exit(1);}
    close(fd);

    mpz_import(rop, bytes, 1, 1, 0, 0, buf);
    mpz_setbit(rop, bits - 1);
    free(buf);
}

// RSA encryption: c = m^e mod n
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    mpz_powm(c, m, e, n);
}

// RSA decryption: m = c^d mod n
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    mpz_powm(m, c, d, n);
}

int main() {
    int key_sizes[] = {128, 256, 512, 1024}; 
    int num_sizes = sizeof(key_sizes)/sizeof(key_sizes[0]);
    printf("===== Medição de desempenho do RSA e visualização de gráfico =====\n");
    FILE *csv = fopen("rsa_times.csv", "w");
    if (!csv) { perror("Erro ao criar csv"); return 1; }
    fprintf(csv, "bits,keygen_time_sec,encrypt_time_sec,decrypt_time_sec\n");

    for (int i = 0; i < num_sizes; i++) {
        int bits = key_sizes[i];
        clock_t start, end;

        // Initialize mpz_t variables
        mpz_t p, q, n, phi, e, d, gcd;
        mpz_t m, c, decrypted;
        mpz_inits(p,q,n,phi,e,d,gcd,NULL);
        mpz_inits(m,c,decrypted,NULL);

        // Key Generation 
        start = clock();
        do {
            random_mpz(p, bits/2); mpz_nextprime(p, p);
            random_mpz(q, bits/2); mpz_nextprime(q, q);
        } while (mpz_cmp(p,q)==0);

        mpz_mul(n,p,q);
        mpz_t p1, q1;
        mpz_inits(p1, q1, NULL);
        mpz_sub_ui(p1,p,1); mpz_sub_ui(q1,q,1);
        mpz_mul(phi,p1,q1);

        mpz_set_ui(e,65537);
        mpz_gcd(gcd,e,phi);
        if (mpz_cmp_ui(gcd,1)!=0) { fprintf(stderr,"Erro: e não é co-primo\n"); return 1;}
        mpz_invert(d,e,phi);

        end = clock();
        double keygen_time = ((double)(end-start))/CLOCKS_PER_SEC;

        // Encryption
        random_mpz(m, bits-2); 
        start = clock();
        rsa_encrypt(c,m,e,n);
        end = clock();
        double encrypt_time = ((double)(end-start))/CLOCKS_PER_SEC;

        // Decryption
        start = clock();
        rsa_decrypt(decrypted,c,d,n);
        end = clock();
        double decrypt_time = ((double)(end-start))/CLOCKS_PER_SEC;

        // Save results
        fprintf(csv,"%d,%.6f,%.6f,%.6f\n", bits, keygen_time, encrypt_time, decrypt_time);
        printf("Bits: %d | Keygen: %.6fs | Encrypt: %.6fs | Decrypt: %.6fs\n", bits, keygen_time, encrypt_time, decrypt_time);

        // Free memory
        mpz_clears(p,q,n,phi,e,d,gcd,p1,q1,m,c,decrypted,NULL);
    }

    fclose(csv);

    printf("\nCSV 'rsa_times.csv' gerado. Armazenando gráfico em 'rsa_times.png'...\n");

    // Plot using gnuplot
    FILE *gp = popen("gnuplot -persistent", "w");
    if (!gp) { perror("Erro ao abrir gnuplot"); return 1; }

    fprintf(gp, "set title 'RSA Performance'\n");
    fprintf(gp, "set xlabel 'Tamanho da chave (bits)'\n");
    fprintf(gp, "set ylabel 'Tempo (segundos)'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set term pngcairo size 900,600\n");
    fprintf(gp, "set output 'rsa_times.png'\n");
    fprintf(gp, "set datafile separator ','\n");  // Read CSV
    fprintf(gp, "plot 'rsa_times.csv' using 1:2 with linespoints lw 2 lt rgb 'red' title 'Geração de chaves', \\\n");
    fprintf(gp, "     'rsa_times.csv' using 1:3 with linespoints lw 2 lt rgb 'blue' title 'Cifra', \\\n");
    fprintf(gp, "     'rsa_times.csv' using 1:4 with linespoints lw 2 lt rgb 'green' title 'Decifra'\n");

    pclose(gp);

    return 0;
}
