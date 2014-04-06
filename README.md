Abaddon HTTP Server
=======

Abaddon is HTTP server written in C using thread pool pattern.
Currently the only supported method is GET. Main thread listens for connections
and queues the clients for handling after the connection has been accepted
while the thread pool threads handle the queued requests.

Installation
=======
```bash
git clone https://github.com/tamertas/abaddon
make abaddon_compile
```

Usage
=======
Place the website you want to host in htdocs folder.
Then, use the command below to start Abaddon.
```bash
make abaddon PORT={Port_of_Choice}
```
Errors encountered during the execution will be logged
in bin/error_log.txt
To use Abaddon in your code all you have to do is call
ad_server_listen with a valid port number argument.

