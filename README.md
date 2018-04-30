# A GUI version of the P2PSP #

### Linux 
```
$ sudo apt-get install qt5-default
$ sudo apt-get install libvlc-dev
$ sudo apt-get install cmake
$ sudo apt-get install libboost-all-dev
```

## Cloning
```
git clone https://github.com/P2PSP/core.git
git clone https://github.com/P2PSP/p2psp-console.git
git clone https://github.com/P2PSP/Qt.git
```

## Building from Sources

Before building the P2PSP GUI make sure that you have P2PSP Console and P2PSP Core cloned in to the system.
The directory structure should be 
--core 
--p2psp-console
--p2pspgui

```
cd core
./make.py
cd ../p2psp-console
./make.py

cd ../p2pspgui
Create a build directory and run
cmake ..
make
```

##Testing

Go to P2PSP-Console directory and run the following commands to create a source and a local team
```
cd bin
wget https://upload.wikimedia.org/wikipedia/commons/7/79/Big_Buck_Bunny_small.ogv
cvlc Big_Buck_Bunny_small.ogv --sout "#http{mux=ogg,dst=:8080/BBB.ogv}" :sout-keep &
./splitter --source_addr 127.0.0.1 --source_port 8080 --splitter_port 8001 --channel BBB.ogv --header_size 30000 > /dev/null &
./monitor --splitter_addr 127.0.0.1 --splitter_port 8001 > /dev/null &
vlc http://localhost:9999 &
```

Now go to the P2PSP-GUI folder and run it

In the Menu bar click on Add Channels and give a Name,Description,Thumbnail,Source address and Port.
Give Source address as 127.0.0.1 and Source Port as 8080(Which ever you mention while creating the splitter)

Now go to Menu bar again and click on Run Peer and give a Player port and select the above created channel from the channel list.
Once you click on Run, the Stream will be displayed.



