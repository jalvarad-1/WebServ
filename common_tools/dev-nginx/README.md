# NGINX Testing Environment

## Overview
This repository contains a basic setup for deploying and testing NGINX within a Docker environment. The primary purpose of this setup is to facilitate the comparison and evaluation of NGINX functionalities with other projects, specifically to understand its behavior and configuration in different scenarios. This environment is especially useful for those developing web server projects and seeking to benchmark against NGINX's capabilities.

## Components
The environment comprises the following key components:

- basic.conf: An NGINX configuration file defining server behavior, such as the server name, listening port, document root, and specific location directives.

- docker-compose.yaml: A Docker Compose configuration file that sets up the NGINX service, including port mappings and volume bindings for the NGINX configuration and content.

- index.html: A simple HTML file that serves as the default index page for the NGINX server.

## Prerequisites
To use this testing environment, you must have the following installed:

- Docker
- Docker Compose

## Getting Started
Follow these steps to deploy the NGINX server:

1. Clone the Repository:

Ensure you have a local copy of this repository.

2. Navigate to the Repository Directory:

Open your terminal and change to this directory.
```bash
cd ./Webserv/common_tools/dev-nginx
```

3. Start the Nginx Container: Run the following command to start the Nginx container:
```bash
docker-compose -f docker-compose.yaml up
```

4. Accessing the Web Page: Once the container is running, you can access the default index page by navigating to http://localhost:8000 in your web browser (remember that the port access depends of config file).

5. Stopping the Container: To stop the Nginx container, use Ctrl+C in the terminal or run the following command in a new terminal window:
```bash
docker-compose -f docker-compose.yaml down
```

## Purpose

The purpose of this setup is to provide a simple and quick way to deploy an Nginx server for testing and comparison purposes. It can be used to understand how Nginx handles web traffic and to experiment with different Nginx configurations and behaviors.

## Customization

You can modify the basic.conf file to change the Nginx configuration. Additionally, you can replace the index.html file to serve different content.
