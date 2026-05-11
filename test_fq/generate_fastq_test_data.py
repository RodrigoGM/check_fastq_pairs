#!/usr/bin/env python3

import random
import gzip
from pathlib import Path

def random_dna(length=100):
    return ''.join(random.choices('ACGT', k=length))

def random_quality(length=100):
    return ''.join(random.choices('IJKLMNOPQRSTUVWXYZ', k=length))  # fake qualities

def write_fastq(filename, records):
    with open(filename, 'w') as f:
        for rid, seq, qual in records:
            f.write(f"@{rid}\n{seq}\n+\n{qual}\n")

def generate_synchronized_reads(num_reads=10, read_len=100):
    records_R1 = []
    records_R2 = []
    for i in range(1, num_reads + 1):
        rid = f"SEQ{i:06d}"
        seq1 = random_dna(read_len)
        seq2 = random_dna(read_len)
        qual1 = random_quality(read_len)
        qual2 = random_quality(read_len)
        records_R1.append((f"{rid}/1", seq1, qual1))
        records_R2.append((f"{rid}/2", seq2, qual2))
    return records_R1, records_R2

def generate_unsynchronized_reads(records_R1, records_R2):
    shuffled_R2 = records_R2.copy()
    random.shuffle(shuffled_R2)
    return records_R1, shuffled_R2

def save_and_compress(filename, records):
    with gzip.open(filename, 'wt') as f:
        for rid, seq, qual in records:
            f.write(f"@{rid}\n{seq}\n+\n{qual}\n")

def main():
    Path("test_data").mkdir(exist_ok=True)
    num_reads = 10
    read_len = 50

    # Synchronized pair
    r1, r2 = generate_synchronized_reads(num_reads, read_len)
    save_and_compress("test_data/synced_R1.fastq.gz", r1)
    save_and_compress("test_data/synced_R2.fastq.gz", r2)

    # Unsynchronized pair
    ur1, ur2 = generate_unsynchronized_reads(r1, r2)
    save_and_compress("test_data/unsynced_R1.fastq.gz", ur1)
    save_and_compress("test_data/unsynced_R2.fastq.gz", ur2)

    print("✅ Test files generated in ./test_data")

if __name__ == "__main__":
    main()
