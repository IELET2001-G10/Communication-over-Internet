To run the server we have used a Raspberry Pi 3 model B, and if you want to do the same, follow these steps:
  1. If you plan to use our project over a period of time, we recommend setting up some firewall rules using Fail2Ban and IPTables to prevent attacks on your Pi.
  2. The Pi needs the JavaScript runtime environment "Node.js" to run the server-file. Use command "sudo apt-get install nodejs" to install.
  3. The Pi needs NPM to download several libraries needed in this project. Use command "sudo apt-get install npm" to download and install NPM. In addition you will need three
     libraries, which can be installed with commands "npm i file-system", "npm i express" and "npm i socket.io".
  4. To host the remote user interface (Nettside) on the Pi, the Pi needs a HTTP-server. Use command "sudo apt-get install nginx" to install.
  
  Now we recommend going to the README.txt in the "Nettside"-folder and follow the guide there before you proceed.
  
  5. Create a new folder (for example "server") in /var/www on the Pi. Add the files (server.js) in the folder "Server" to /var/www/server on your Pi.
  
  Again, be sure that you have read the README.txt in the "Nettside"-folder and followed the guide there before you proceed. If you have not read the README.txt in the "ESP32"
  folder yet, we recommend doing it now.
  
  6. Navigate to /var/www/server on your Pi, and type in the command "node server.js". This will start the server, and you can now navigate to the Pi's IP-address (we used         
     192.168.1.158) to access the remote user interface (Nettside).

Congrats! You should now have read and followed all the guides to set up the system. If you have any trouble, make sure you have placed all files and done the set-ups correctly.

Enjoy!
