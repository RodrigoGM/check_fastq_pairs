# Check FASTQ Pairs

[![check_fastq_pairs v0.2.0](https://img.shields.io/badge/version-0.2.0-D40000.svg)](https://github.com/RodrigoGM/check_fastq_pairs/)
[![License: BSD-3](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/bsd-3-clause)

## Summary of Dependencies for GCC 11.2.0

| Dependency      | Required For           | Minimum Version     | Installed By         |
|-----------------|------------------------|----------------------|----------------------|
| **g++**         | C++11+ compilation     | GCC 11.2.0           | GCC build/module     |
| **zlib**        | GZIP file reading      | Any (≥ 1.2.3)        | `zlib-devel` package |
| **libstdc++**   | Standard C++ library   | Matches GCC 11.2.0   | Comes with GCC       |


## Compilation
```bash
conda deactivate
module load gcc/11.2.0

g++ -O3 check_fastq_pairs.cpp -o check_fastq_pairs -lz

```

## Usage

```bash
./check_fastq_pairs test_fq/synced_R{1,2}.fastq.gz
test_fq/synced_R1.fastq.gz	10	10	True

./check_fastq_pairs test_fq/unsynced_R{1,2}.fastq.gz
test_fq/unsynced_R1.fastq.gz	10	10	False
```

### Output Description

Four column table on `stdout` 

|--------------|--------------------|--------------------|-------------------------------|
| R1 file name | number of R1 reads | number of R2 reads | synchronized reads True/False |


### Extended version:

| Check                                              | Detects                      |
|----------------------------------------------------|------------------------------|
| R1 header matches R2 header                        | R1/R2 read pair header       |
| Header starts with @                               | Malformed or shifted records |
| Header has content after @                         | Empty read IDs               |
| Sequence contains only valid nucleotide characters | Corrupted sequence data      |
| Third line starts with +                           | Structural corruption        |
| Quality scores are ASCII 33–126                    | Invalid Phred encoding       |
| Sequence length == Quality length                  | Truncated or corrupted lines |
| All 4 lines present per record                     | File truncation mid-record   |

### Exit codes

| Code | Meaning                                   |
|------|-------------------------------------------|
| 0    | Files are well-formed and pass all checks |
| 1    | Usage error or file open failure          |
| 2    | Malformed or truncated FASTQ detected     |


### Tested on 
* CentOS Linux 7
* macOS 13.6.9

## Disclosures
This tool was created with support from ChatGPT and Claude.
