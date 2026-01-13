# Start from alpine image. Small but no fancy tools
FROM alpine:3.21

# Use /usr/src/app as workdir. The following instructions will be executed in this workdir.
WORKDIR /usr/src/app

# Copy the hello.sh file into our workdir
COPY hello.sh .

# You can add execution permission here 
RUN chmod +x hello.sh

RUN touch addition.txt

# When running docker run command will be ./hello.sh
# CMD is executed when we call docker run, unless we overwrite it.
CMD ["./hello.sh"]