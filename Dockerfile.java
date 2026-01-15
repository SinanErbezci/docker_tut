FROM amazoncorretto:8-alpine3.20

EXPOSE 8080

WORKDIR /usr/src/app

RUN apk add --no-cache bash dos2unix

COPY ./material-applications/spring-example-project/ .

RUN dos2unix mvnw && bash ./mvnw package

CMD [ "java", "-jar", "./target/docker-example-1.1.3.jar" ]

# docker build -t java-project -f Dockerfile.java . && docker run -p 8080:8080 java-project