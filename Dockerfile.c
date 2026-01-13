FROM ubuntu:24.04

WORKDIR /mydir

RUN apt-get update && apt-get install -y curl python3 ffmpeg

RUN curl -L https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp -o /usr/local/bin/yt-dlp

RUN chmod a+x /usr/local/bin/yt-dlp
# your commands will be added to this.
# example -> /usr/local/bin/yt-dlp <your-argument>
ENTRYPOINT ["/usr/local/bin/yt-dlp"]

# now this is default arguments. If no arguments are
# given then this will be used.
CMD ["https://www.youtube.com/watch?v=uTZSILGTskA"]