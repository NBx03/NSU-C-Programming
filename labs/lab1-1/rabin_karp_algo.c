#include <stdio.h>
#include <string.h>

#define PAGE_LEN 1024

int ComputeHash(const unsigned char *string, int sample_len, int *hash);

int main() {
    unsigned char sample[17];
    if(!fscanf(stdin, "%16[^\n]", sample))
        return 0;
    fgetc(stdin);

    int sample_len = strlen((char*)sample);
    int sampleHash = 0;
    ComputeHash(sample, sample_len, &sampleHash);
    printf("%d ", sampleHash);

    unsigned char string[PAGE_LEN];
    int string_len = fread(string, 1, PAGE_LEN, stdin);
    int stringHash = 0;
    int power = ComputeHash(string, sample_len, &stringHash);
    int cur_shift = 0;

    for(int shift = sample_len; shift <= string_len; shift++){
        if(stringHash == sampleHash){
            for(int j = 0; j < sample_len; j++) {
                printf("%d ", j + shift - sample_len + 1);
                if(string[cur_shift + j] != sample[j])
                    break;
            }
        }

        if(shift == string_len) {
            memmove(string, string + PAGE_LEN - sample_len, sample_len);
            string_len += fread(&string[sample_len], 1, PAGE_LEN - sample_len, stdin);
            cur_shift = 0;
            if(shift == string_len)
                break;
        }
        stringHash = (stringHash - string[cur_shift] % 3) / 3 + (string[cur_shift + sample_len] % 3) * power;
        cur_shift++;
    }
    return 0;
}

int ComputeHash(const unsigned char *string, int sample_len, int *hash) {
    int power = 1;
    for(int i = 0; i < sample_len; i++){
        *hash += (string[i] % 3) * power;
        power *= 3;
    }
    return power / 3;
}
