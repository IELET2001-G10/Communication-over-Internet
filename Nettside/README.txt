If you have not read the README.txt-files in the "ESP32" and "Server"-folders, do that before you proceed.

To host the remote user intercade (Nettside) we have used a Raspberry Pi 3 model B, and if you want to do the same, follow these steps:
  1. Navigate to /var/www/html on the Pi and remove the index.html-file from the folder.
  2. Transfer all the folders and files in the "Nettside"-folder from the .zip-folder into /var/www/html.
  3. Navigate to /var/www/html/script and use command "nano socket.js". Change the IP-address to the Pi's IP-address (we used IP-address 192.168.1.158), save and exit.
     This is the line that tells the UI (which is a client) where the server is so the UI-client can connect to it.
     
You have now provided the necessary files to run the UI hosted on the Pi, and can continue the set-up guide for the server in the server's README.txt.
