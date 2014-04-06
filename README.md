Abaddon HTTP Server
=======

Abaddon is HTTP server written in C using thread pool pattern.
Currently only GET method is supported. Main thread listens for connections
and queues the client for handling after the connection has been accepted
while the thread pool threads handle the queued requests.

Installation
=======
'''bash
git clone https://github.com/TamerTas/Abaddon
make abaddon_compile
'''

Usage
=======
Place the website you want to host in htdocs folder.
Then, use the command below to start Abaddon.
'''bash
make abaddon PORT={Port_of_Choice}
'''
errors encountered during the execution will be logged
in bin/log.txt

To use Abaddon in your code all you have to do is call
ad_server_listen with a valid port number argument.

