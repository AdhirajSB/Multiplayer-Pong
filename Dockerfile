# Use a lightweight Linux distribution
FROM ubuntu:22.04

# Install g++ and make
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Use your Makefile to build ONLY the server target
RUN make server

EXPOSE 5555/udp

CMD ["./bin/server"]