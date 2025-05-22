# Check FASTQ Pairs

## Disclosure:
Code written using ChatGTP

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
./check_fastq_pairs test/synced_R{1,2}.fastq.gz
test_fq/synced_R1.fastq.gz	10	10	True

./check_fastq_pairs test_fq/unsynced_R{1,2}.fastq.gz
test_fq/unsynced_R1.fastq.gz	10	10	False
```

### Output Description

Four column table on `stdout` 
| R1 file name | number of R1 reads | number of R2 reads | synchronized reads True/False|


### Tested on 
* CentOS Linux 7
* macOS 13.6.9
