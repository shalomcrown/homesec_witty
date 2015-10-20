#set remotebreak 1
set $connected=0
shell sleep 5
target remote 192.168.2.51:1234
set $connected=1
continue # optional...
