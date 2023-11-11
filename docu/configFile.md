[Go back to repo](https://github.com/jalvarad-1/WebServ)

# Config file documentation

For our WebServ to work, its necessary to execute it, passing it a configuration file as an argument.

Here, you can design your oun config file  as u like. In our case we decide to make it as similar possible to what we would use for an nginx server.

## Subject specifications

Having the [subject PDF](https://github.com/jalvarad-1/WebServ) as a reference, our config file must work as this:

- Choose the port and host each 'server'.
- Setup the server_name or not.
- The first server for a host:port will be the default for this host:port (that means it will answer to all request that don't belong to an other server).
- Setup default error apges.
- Limit client body size.
- Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):
    - Define a list of accepted HTTP methods for the route.
    - Define a HTTP redirection.
    - Define a directory or a file where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/titi/pouet is /tmp/www/pouic/toto/pouet).
    - Turn on or off directory listing.
    - Set a default file to answer if the request is a directory.
    - Execute [CGI](https://en.wikipedia.org/wiki/Common_Gateway_Interface) based on certaing file extension (for example .php).
    - Make the route able to accept uploaded files and configure where they should be saved.

There is more information about [CGI](https://en.wikipedia.org/wiki/Common_Gateway_Interface) for config file but for now just knowing this is enough.

## Our config file

At first we wanted out file to be as similar to an nginx configuration file as possible, currently out config file looks like this:

```
server {
    listen 80;
    server_name example.com;

    root /var/www/html; 

    error_page 404 /custom_404.html;                
    error_page 500 502 503 504 /custom_error.html;

    client_max_body_size 10M;

    location /kapouet {
        if ($request_method !~ ^(GET|POST|HEAD)$ ) {
            return 405;
        }

        if ($arg_redirect = "yes") {
            return 301 https://example.com/new_location;
        }

        alias /tmp/www;

        autoindex on;

        index index.html;

        location ~ \.php$ {
            fastcgi_pass unix:/var/run/php-fpm.sock;
            fastcgi_param SCRIPT_FILENAME $request_filename;
            include fastcgi_params;
        }

        client_body_temp_path /var/www/uploads;
        client_body_in_file_only on;
        client_body_buffer_size 128K;
        client_max_body_size 10M;
        client_body_timeout 60s;

        client_body_temp_path /var/www/uploads;
        upload_store /var/www/uploads;
    }
}

server {
    listen 222;
    server_name _; 

    ...
}
```

## Parser logic

All the ":sparkles: Magic :sparkles:" of reading and parsing the config file happens in the [ConfigParser folder](https://github.com/jalvarad-1/WebServ/tree/master/src/ConfigParser) inside src.

Here you will see the class "ConfigParser". The first thing our server will do is create a class of this type. In its constructor it will carry out the necessary steps to check the integrity of the file, read it and store the relevant data for the execution of the server.

The information of each server is stored in a "ServerConfig" class. The "ConfigParser" class will contain a vector that will store as many classes of type "ServerConfig" as there are declared.

The same logic exist for the location, each server may have more than one location, in each location there may be a lot of information. For all this, we have a class called "LocationRules" that will be stored in a vector inside the class that will sotre as many classes of type "LocationRules" as there are declared on the server.

