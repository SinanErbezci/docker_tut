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
docker run -d --rm -it -name looper-it ubuntu sh -c "while true; do date, sleep 1; done"
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

We can use the command docker build(opens in a new tab)(opens in a new tab) to turn the Dockerfile to an image.

By default docker build will look for a file named Dockerfile. Now we can run docker build with instructions where to build (.) and give it a name (-t <name>)
```
docker build -t hello-docker .
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