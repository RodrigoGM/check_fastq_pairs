#include <iostream>
#include <fstream>
#include <string>
#include <zlib.h>
#include <cstring>

#define MAX_LINE 8192

// Extracts the comparable read ID from a FASTQ header
std::string extract_read_id(const std::string& header_line) {
    std::string header = header_line[0] == '@' ? header_line.substr(1) : header_line;
    std::size_t pos = header.find_first_of(" /");
    return header.substr(0, pos);
}

// Reads 4 lines from a gzipped FASTQ file, and returns the read ID
bool read_next_record(gzFile file, std::string& read_id) {
    char line[MAX_LINE];

    if (gzgets(file, line, MAX_LINE) == Z_NULL) return false;
    read_id = extract_read_id(std::string(line));

    for (int i = 0; i < 3; ++i) {
        if (gzgets(file, line, MAX_LINE) == Z_NULL) return false;
    }

    return true;
}

// Main checking logic
int check_fastq_pair(const std::string& file1, const std::string& file2) {
    gzFile f1 = gzopen(file1.c_str(), "rb");
    gzFile f2 = gzopen(file2.c_str(), "rb");

    if (!f1 || !f2) {
        std::cerr << "❌ Failed to open one or both files." << std::endl;
        if (f1) gzclose(f1);
        if (f2) gzclose(f2);
        return 1;
    }

    std::string id1, id2;
    size_t count1 = 0, count2 = 0;
    bool synchronized = true;

    while (true) {
        bool has1 = read_next_record(f1, id1);
        bool has2 = read_next_record(f2, id2);

        if (!has1 && !has2) break;

        if (has1) ++count1;
        if (has2) ++count2;

        if (has1 != has2 || (has1 && has2 && id1 != id2)) {
            synchronized = false;
        }
    }

    gzclose(f1);
    gzclose(f2);

    // Print tab-separated summary line (no header)
    std::cout << file1 << "\t" << count1 << "\t" << count2 << "\t"
              << (synchronized && count1 == count2 ? "True" : "False") << std::endl;

    return 0;
}

// Main wrapper
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " file1.fastq.gz file2.fastq.gz" << std::endl;
        return 1;
    }

    return check_fastq_pair(argv[1], argv[2]);
}
