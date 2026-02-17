#### source: https://courses.mooc.fi/org/uh-cs/courses/devops-with-docker/
Check Version
```
docker -v
```
### 1. Docker Basics
1. Docker is a set of tools to deliver software in containers.
2. Containers are packages of software.
#### Benefits from containers
* Solves "works on my machine problem". App + Dependencies
* Isolated Enviroment. One app uses python 3.5 other one 2.7. 
* Development. Already installed services
* Scaling. Load balancing, up/down scaling
#### Container vs VMs
Containers offer faster startup, better resource utilization, and high portability across different environments, though their isolation is at the process level, which may not be as robust as that of VMs. Overall, VMs could be used for scenarios needing complete OS environments, while containers excel in lightweight, efficient, and consistent application deployment.
#### Running Sample Container
```
docker container run hello-world
```
```
docker run (is same with) docker container run
```
#### Container and Images
Containers are instances of images. Image is like a blueprint or template

You need an image and a container runtime (Docker engine) to create a container.

A Docker image is a file. An image never changes; you can not edit an existing file. Creating a new image happens by starting from a base image and adding new layers to it. We will talk about layers later, but you should think of images as immutable, they can not be changed after they are created.
```
docker image ls # List your all images
```
This image file is built from an instructional file named Dockerfile that is parsed when you run ```docker image build```.

Dockerfile is the instruction set for building an image.
```
docker container ls #List your running containers
```
```
docker container ls -a #List your all containers
```
```
docker container -> docker ps (same)
```
Containers are isolated environments in the host machine with the ability to interact with each other and the host machine itself via defined methods (TCP/UDP).
CLI client sends a request to the Docker daemon through the REST API. The Docker daemon takes care of images, containers and other resources.

So before removing images, you should have the referencing container removed first. 
#### Most used commands

```
docker container ls -a | grep hello-world
```

```
docker rm <image(id or name)>
```
```
docker stop
```
execute a command inside the container
```
docker exec 
```
Pull image from a docker registery 
```
docker pull # 
```
delete stopped containers
```
docker container prune 
```
delete unused images
```
docker images prune
````
clear almost everything
```
docker system prune
```
```
docker run nginx -d (detached)
```

For all of them, can be either the container id or the container name. Same for images

### 2. Running and Stopping Containers
Ubuntu with tty(-t) and interactive(-i)
```
docker run -it ubuntu 
```
```
docker run -d -it --name looper ubuntu sh -c "while true; do date; sleep 1; done"
```
check the output logs 
```
docker logs -f <container-name>
```
pausing the container
```
docker pause looper
```
unpausing the container
```
docker unpause looper
```
bring the containers process to foreground
```
docker attach looper
```
Control+C container will be terminated

Making sure dont closing the container from another terminal
```
docker attach --no-stdin looper
```

execute commands withing a running container
```
docker exec looper ls -la
```
execute bash in the container
```
docker exec -it looper bash
```

kill the container
```
docker kill looper
docker rm looper
# or just docker rm --force looper
```
remove it auto after it's exited.
```
docker run -d --rm -it --name looper-it ubuntu sh -c "while true; do date; sleep 1; done"
```
control+p, control+q to detach us from the STDOUT.

### 3. Indepth review of Images

We can search for images in the Docker Hub with ```docker search```

Pulling different version of an image
```
docker pull ubuntu:<version>
```
Taggin is also way to rename images.
```
docker tag ubuntu:25.04 ubuntu:example
```

#### Building Images
Dockerfile is simply a file that contains the build instructions for an image.

Alpine is  a small Linux distribution that is often used to create small images.

We will choose exactly which version of a given image we want to use. This guarantees that we don't accidentally update through a breaking change, and we know which images need updating when there are known security vulnerabilities in old images.

We can use the command docker build to turn the Dockerfile to an image.

By default docker build will look for a file named Dockerfile. Now we can run docker build with instructions where to build (.) and give it a name (-t <name>)
```
docker build -t hello-docker .
```
```
docker run -it hello-docker sh
```

It is also possible to manually create new layers on top of an image. Let us now create a new file called additional.txt and copy it inside a container.
```
docker cp ./addition.txt blissful_jepsen:/usr/src/app
```
Check out what changed in our container. (A -> added, C -> changed, D -> deleted)
```
docker diff blissful_jepsen
```
saving the changes. (or just change the dockerfile)
```
docker commit blissful_jepsen hello-docker-addition
```
Naming your dockerfiles
```
docker build -t tester -f Dockerfile.testing .
```

### Defining Start Conditions
This time we will open up an interactive session and test stuff before "storing" it in our Dockerfile.
```
docker run -it ubuntu:24.04
```
Install curl
```
apt-get update && apt-get install -y curl
```
Since we know that an && requires both expressions to be true, we can use this knowledge to chain bash commands that we only want to run in succession if the first command was successful.

```
curl -L https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp -o /usr/local/bin/yt-dlp
```
```
chmod a+rx /usr/local/bin/yt-dlp 
```
```
apt-get install -y python3 ffmpeg
```
Now add all these to dockerfile.

We should always try to keep the most prone-to-change rows at the bottom, by adding the instructions to the bottom we can preserve our cached layers.

The argument we give to docker run <> <argument> will replace the CMD command. Therefore we need an ENTRYPOINT. It will add our commands to it.

By default, the entrypoint in Docker is set as /bin/sh -c and this is passed if no entrypoint is set. This is why giving the path to a script file as CMD works.

There are two ways to set the ENTRYPOINT and CMD: exec form and shell form. We have been using the exec form, in which the command itself is executed. The exec form is generally preferred over the shell form, because in the shell form the command that is executed is wrapped with /bin/sh -c, which can result in unexpected behaviour. However, the shell form can be useful in certain situations, for example, when you need to evaluate environment variables in the command like $MYSQL_PASSWORD or similar.

In the shell form, the command is provided as a string without brackets. In the exec form the command and its arguments are provided as a list (with brackets)

shell form
```
ENTRYPOINT /bin/ping -c 3
```

Now we have two problems with the yt-dlp project:

Major: The downloaded files stay in the container
Minor: Our container build process creates many layers resulting in increased image size

Docker cp command to copy the file from the container to the host machine. We should use quotes now since the filename has spaces.
```
docker cp "naughty_kirch://mydir/Burden Of Dreams V17 Nalle Hukkataival [uTZSILGTskA].webm" .
```

### Volumes and Ports
We can use Docker volumes to make it easier to store the downloads outside the container's ephemeral storage.
With bind mount we can mount a file or directory from our own machine into the container.

We mount our current folder as /mydir in the container, overwriting everything that we have put in that folder in our Dockerfile.
```
# Binding a Folder
docker run -v "$(pwd):/mydir" yt-dlp https://www.youtube.com/watch?v=saEpkcVi1d4 
```
```
# Binding a File (File must exists)
docker run \
  -v "$(pwd)/text.log:/usr/src/app/text.log" \
  devopsdockeruh/simple-web-service
```
A Docker volume is essentially a shared directory or 
file between the host machine and the container.

* The changes are preserverd.
* File sharing between containers. 

It is possible to map a port on your host machine to a container port. For example, if you map port 1000 on your host machine to port 2000 in the container, and then send a message to http://localhost:1000 on your computer, the container will receive that message if it's listening to its port 2000.

Opening a connection from the outside world to a Docker container happens in two steps:

* Exposing port (Container listens to a port)
* Publishing port (mapping host ports to container ports)

To expose a port, add the line EXPOSE <port> in your Dockerfile

```
# publish a port. <host-port>:<container-port>
# If you leave out host port, it auto select available port
docker run -p 4567 app-in-port
```

Limit connections to certain protocol EXPOSE <port>/udp

```
docker run -p 8080:8080 devopsdockeruh/simple-web-service server 
```

### Utilizing Tools from Registry
```
docker build . -t rails-project && docker run -p 3000:3000 rails-project
```
Here we did a quick trick to separate installing dependencies from the part where we copy the source code in. The COPY will copy both files Gemfile and Gemfile.lock to the current directory. This will help us by caching the dependency layers if we ever need to make changes to the source code.

docker build -t java-project -f Dockerfile.java . && docker run -p 8080:8080 java-project

### Docker Hub and Pushing
```
# Login
docker login
```
you will need to rename the image to include your username, and then you can push it:
```
docker tag yt-dlp <username>/<repository>
```
```
docker push <username>/<repository>
```

# Docker Compose
Simplifies running multiple containers.

```
docker compose [-f <arg>...] [options] [COMMAND] [ARGS...]
```

```
services:
  frontend:
    image: example/webapp
    # Either giving directory where dockerfile exists
    build: ./webapp

  backend:
    image: example/database
    # Or giving full details
    build:
      context: backend
      dockerfile: ../backend.Dockerfile
    volumes:
    # location-in-host:location-in-container
      - .:/mydir
    container_name: yt-dlp
```

## Using Ready Image with Docker Compose

This way "build" is not needed.
```
services:
  nginx:
    image: nginx:1.27
  database:
    image: postgres:17
```

## Key Commands in Docker Compose
To start all services defined in docker-compose.yaml file
```
docker compose up
```
To stop and remove services
```
docker compose down
```
view logs
```
docker compose logs
```
list services
```
docker compose ps
```