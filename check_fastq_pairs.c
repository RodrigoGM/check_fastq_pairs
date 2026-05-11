#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define MAX_LINE 8192

// Extracts the read ID up to the first space or slash suffix (/1 or /2)
void extract_read_id(const char *header_line, char *out_id) {
    const char *start = header_line[0] == '@' ? header_line + 1 : header_line;
    size_t i = 0;
    while (start[i] != '\0' && start[i] != ' ' && start[i] != '/' && start[i] != '\n') {
        out_id[i] = start[i];
        i++;
    }
    out_id[i] = '\0';
}

// Reads 4 lines from a FASTQ file and returns 1 if successful, 0 at EOF
int read_fastq_record(gzFile file, char *id_buf) {
    char line[MAX_LINE];
    if (gzgets(file, line, MAX_LINE) == NULL) return 0;
    extract_read_id(line, id_buf);

    for (int i = 0; i < 3; i++) {
        if (gzgets(file, line, MAX_LINE) == NULL) return 0;
    }
    return 1;
}

int check_fastq_pair(const char *file1, const char *file2) {
    gzFile f1 = gzopen(file1, "rb");
    gzFile f2 = gzopen(file2, "rb");

    if (!f1 || !f2) {
        fprintf(stderr, "❌ Failed to open one or both files.\n");
        if (f1) gzclose(f1);
        if (f2) gzclose(f2);
        return 1;
    }

    char id1[MAX_LINE], id2[MAX_LINE];
    size_t count1 = 0, count2 = 0;
    int synchronized = 1;

    while (1) {
        int has1 = read_fastq_record(f1, id1);
        int has2 = read_fastq_record(f2, id2);

        if (!has1 && !has2) break; // both finished

        if (has1) count1++;
        if (has2) count2++;

        if (has1 != has2 || (has1 && has2 && strcmp(id1, id2) != 0)) {
            synchronized = 0;
        }
    }

    gzclose(f1);
    gzclose(f2);

    printf("%s\t%zu\t%zu\t%s\n", file1, count1, count2,
           (synchronized && count1 == count2) ? "True" : "False");

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1.fastq.gz file2.fastq.gz\n", argv[0]);
        return 1;
    }

    return check_fastq_pair(argv[1], argv[2]);
}
