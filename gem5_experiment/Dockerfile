# Use Ubuntu 20.04 or 22.04
FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    scons \
    build-essential \
    python3 \
    python3-dev \
    libprotobuf-dev \
    protobuf-compiler \
    libgoogle-perftools-dev \
    git \
    ca-certificates \
    wget \
    m4 \
    && rm -rf /var/lib/apt/lists/*

# Create a working directory
WORKDIR /root

# Clone gem5
RUN git clone https://gem5.googlesource.com/public/gem5

WORKDIR /root/gem5
# Check out a known stable tag or branch, e.g., v22.0.0.2
RUN git checkout v22.0.0.2

# Build gem5 for X86 in SE mode
RUN scons build/X86/gem5.opt -j 1

# Copy local files into container
WORKDIR /root
COPY gem5_config.py .
COPY run_experiments.py .
COPY programs/ ./programs/

# Compile your programs statically for SE mode
WORKDIR /root/programs
RUN for f in *.c; do gcc -static -O2 -o "${f%.c}" "$f"; done

# We go back to /root as final
WORKDIR /root

# Default command: drop into bash, or you can just do nothing
CMD ["/bin/bash"]
